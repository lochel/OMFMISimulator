/*
 * This file is part of OpenModelica.
 *
 * Copyright (c) 1998-CurrentYear, Open Source Modelica Consortium (OSMC),
 * c/o Linköpings universitet, Department of Computer and Information Science,
 * SE-58183 Linköping, Sweden.
 *
 * All rights reserved.
 *
 * THIS PROGRAM IS PROVIDED UNDER THE TERMS OF GPL VERSION 3 LICENSE OR
 * THIS OSMC PUBLIC LICENSE (OSMC-PL) VERSION 1.2.
 * ANY USE, REPRODUCTION OR DISTRIBUTION OF THIS PROGRAM CONSTITUTES
 * RECIPIENT'S ACCEPTANCE OF THE OSMC PUBLIC LICENSE OR THE GPL VERSION 3,
 * ACCORDING TO RECIPIENTS CHOICE.
 *
 * The OpenModelica software and the Open Source Modelica
 * Consortium (OSMC) Public License (OSMC-PL) are obtained
 * from OSMC, either from the above address,
 * from the URLs: http://www.ida.liu.se/projects/OpenModelica or
 * http://www.openmodelica.org, and in the OpenModelica distribution.
 * GNU version 3 is obtained from: http://www.gnu.org/copyleft/gpl.html.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without
 * even the implied warranty of  MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE, EXCEPT AS EXPRESSLY SET FORTH
 * IN THE BY RECIPIENT SELECTED SUBSIDIARY LICENSE CONDITIONS OF OSMC-PL.
 *
 * See the full OSMC Public License conditions for more details.
 *
 */

#include "FMUWrapper.h"
#include "Variable.h"
#include "DirectedGraph.h"
#include "Logging.h"
#include "Resultfile.h"
#include "Settings.h"
#include "CompositeModel.h"
#include "Util.h"

#include <fmilib.h>
#include <JM/jm_portability.h>

#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

#include <boost/filesystem.hpp>

void fmiLogger(jm_callbacks* c, jm_string module, jm_log_level_enu_t log_level, jm_string message)
{
  switch(log_level)
  {
    case jm_log_level_info:
      logDebug("module " + std::string(module) + ": " + std::string(message));
      break;
    case jm_log_level_warning:
      logWarning("module " + std::string(module) + ": " + std::string(message));
      break;
    case jm_log_level_error:
      logError("module " + std::string(module) + ": " + std::string(message));
      break;
    case jm_log_level_fatal:
      logFatal("module " + std::string(module) + ": " + std::string(message));
      break;
    default:
      logWarning("[log level " + std::string(jm_log_level_to_string(log_level)) + "] module " + std::string(module) + ": " + std::string(message));
  }
}

void fmi2logger(fmi2_component_environment_t env, fmi2_string_t instanceName, fmi2_status_t status, fmi2_string_t category, fmi2_string_t message, ...)
{
  int len;
  char msg[1000];
  va_list argp;
  va_start(argp, message);
  len = vsnprintf(msg, 1000, message, argp);

  switch(status)
  {
    case fmi2_status_ok:
    case fmi2_status_pending:
      logDebug(std::string(instanceName) + " (" + category + "): " + msg);
      break;
    case fmi2_status_warning:
      logWarning(std::string(instanceName) + " (" + category + "): " + msg);
      break;
    case fmi2_status_error:
    case fmi2_status_discard:
      logError(std::string(instanceName) + " (" + category + "): " + msg);
      break;
    case fmi2_status_fatal:
      logFatal(std::string(instanceName) + " (" + category + "): " + msg);
      break;
    default:
      logWarning("fmiStatus = " + std::string(fmi2_status_to_string(status)) + "; " + instanceName + " (" + category + "): " + msg);
  }
}

FMUWrapper::FMUWrapper(CompositeModel& model, std::string fmuPath, std::string instanceName)
  : model(model), fmuPath(fmuPath), instanceName(instanceName)
{
  logTrace();

  if (!boost::filesystem::exists(fmuPath))
    logFatal("Specified file name does not exist: \"" + fmuPath + "\"");

  callbacks.malloc = malloc;
  callbacks.calloc = calloc;
  callbacks.realloc = realloc;
  callbacks.free = free;
  callbacks.logger = fmiLogger;
#ifdef OMS_DEBUG_LOGGING
  callbacks.log_level = jm_log_level_all;
#else
  callbacks.log_level = jm_log_level_warning;
#endif
  callbacks.context = 0;

  //set working directory
  tempDir = fmi_import_mk_temp_dir(&callbacks, model.getSettings().GetTempDirectory(), "temp_");
  logDebug("set working directory to \"" + tempDir + "\"");

  context = fmi_import_allocate_context(&callbacks);

  // check version of FMU
  fmi_version_enu_t version = fmi_import_get_fmi_version(context, fmuPath.c_str(), tempDir.c_str());
  if(fmi_version_2_0_enu != version)
  {
    logError("Unsupported FMI version: " + std::string(fmi_version_to_string(version)));
    return;
  }

  // parse modelDescription.xml
  fmu = fmi2_import_parse_xml(context, tempDir.c_str(), 0);
  if(!fmu)
    logFatal("Error parsing modelDescription.xml");

  // check FMU kind (CS or ME)
  fmuKind = fmi2_import_get_fmu_kind(fmu);
  if(fmi2_fmu_kind_me == fmuKind)
    logDebug("FMU ME");
  else if(fmi2_fmu_kind_cs == fmuKind)
    logDebug("FMU CS");
  else
    logError("Unsupported FMU kind: " + std::string(fmi2_fmu_kind_to_string(fmuKind)));

  callBackFunctions.logger = fmi2logger;
  callBackFunctions.allocateMemory = calloc;
  callBackFunctions.freeMemory = free;
  callBackFunctions.componentEnvironment = fmu;

  if(fmi2_fmu_kind_me == fmuKind)
  {
    jm_status_enu_t jmstatus;

    //Load the FMU shared library
    jmstatus = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_me, &callBackFunctions);
    if (jm_status_error == jmstatus) logFatal("Could not create the DLL loading mechanism (C-API). Error: " + std::string(fmi2_import_get_last_error(fmu)));

    logDebug("Version returned from FMU: " + std::string(fmi2_import_get_version(fmu)));
    logDebug("Platform type returned: " + std::string(fmi2_import_get_types_platform(fmu)));
    logDebug("GUID: " + std::string(fmi2_import_get_GUID(fmu)));

    fmi2_string_t instanceName = "ME-FMU instance";
    jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_model_exchange, NULL, fmi2_false);
    if (jm_status_error == jmstatus) logFatal("fmi2_import_instantiate failed");
  }
  else if(fmi2_fmu_kind_cs == fmuKind)
  {
    jm_status_enu_t jmstatus;

    //Load the FMU shared library
    jmstatus = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_cs, &callBackFunctions);
    if (jm_status_error == jmstatus) logFatal("Could not create the DLL loading mechanism (C-API). Error: " + std::string(fmi2_import_get_last_error(fmu)));

    logDebug("Version returned from FMU: " + std::string(fmi2_import_get_version(fmu)));
    logDebug("Platform type returned: " + std::string(fmi2_import_get_types_platform(fmu)));
    logDebug("GUID: " + std::string(fmi2_import_get_GUID(fmu)));

    fmi2_string_t instanceName = "CS-FMU instance";
    jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_cosimulation, NULL, fmi2_false);
    if (jm_status_error == jmstatus) logFatal("fmi2_import_instantiate failed");
  }

  // create variable list
  fmi2_import_variable_list_t *varList = fmi2_import_get_variable_list(fmu, 0);
  size_t varListSize = fmi2_import_get_variable_list_size(varList);
  logDebug(toString(varListSize) + " variables");
  for(size_t i=0; i<varListSize; ++i)
  {
    fmi2_import_variable_t* var = fmi2_import_get_variable(varList, i);
    Variable v(var, instanceName);
    allVariables.push_back(v);
  }
  fmi2_import_free_variable_list(varList);

  // mark states
  varList = fmi2_import_get_derivatives_list(fmu);
  varListSize = fmi2_import_get_variable_list_size(varList);
  logDebug(toString(varListSize) + " states");
  for(size_t i=0; i<varListSize; ++i)
  {
    fmi2_import_variable_t* var = fmi2_import_get_variable(varList, i);
    fmi2_import_real_variable_t* varReal = fmi2_import_get_variable_as_real(var);
    fmi2_import_variable_t* varState = (fmi2_import_variable_t*)fmi2_import_get_real_variable_derivative_of(varReal);
    if (varState)
    {
      fmi2_value_reference_t state_vr = fmi2_import_get_variable_vr(varState);
      Variable* state_var = getVariable(state_vr);
      if (state_var)
        state_var->markAsState();
      else
        logError("Couldn't find " + toString(fmi2_import_get_variable_name(varState)));
    }
    else
      logError("Couldn't map " + toString(fmi2_import_get_variable_name(var)) + " to the corresponding state variable");
  }
  fmi2_import_free_variable_list(varList);

  // create some special variable maps
  for (int i=0; i<allVariables.size(); i++)
  {
    if (allVariables[i].isInitialUnknown())
      initialUnknowns.push_back(i+1);
    if (allVariables[i].isInput())
      allInputs.push_back(i+1);
    if (allVariables[i].isOutput())
      allOutputs.push_back(i+1);
    if (allVariables[i].isParameter())
      allParameters.push_back(i+1);

    switch(allVariables[i].getBaseType())
    {
      case fmi2_base_type_real:
        realVariables.push_back(i+1);
        break;
      case fmi2_base_type_int:
        intVariables.push_back(i+1);
        break;
      case fmi2_base_type_bool:
        boolVariables.push_back(i+1);
        break;
      case fmi2_base_type_str:
        strVariables.push_back(i+1);
        break;
      case fmi2_base_type_enum:
        enumVariables.push_back(i+1);
        break;
      default:
        logWarning("FMUWrapper: Unsupported base type");
        break;
    }
  }

  // generate internal dependency graphs
  getDependencyGraph_outputs();
  getDependencyGraph_initialUnknowns();
}

FMUWrapper::~FMUWrapper()
{
  logTrace();
  fmi2_import_free_instance(fmu);
  fmi2_import_destroy_dllfmu(fmu);
  fmi2_import_free(fmu);
  fmi_import_free_context(context);
  if (boost::filesystem::is_directory(tempDir))
  {
    fmi_import_rmdir(&callbacks, tempDir.c_str());
    logDebug("removed working directory: \"" + tempDir + "\"");
  }
}

double FMUWrapper::getReal(const std::string& var)
{
  logTrace();
  if(!fmu)
    logFatal("FMUWrapper::getReal failed");

  Variable* v = getVariable(var);

  if(!v)
    logFatal("FMUWrapper::getReal failed");

  double value;
  fmi2_import_get_real(fmu, &v->getValueReference(), 1, &value);

  return value;
}

void FMUWrapper::setReal(const std::string& var, double value)
{
  logTrace();
  if(!fmu)
    logFatal("FMUWrapper::setReal failed");

  Variable* v = getVariable(var);

  if(!v || v->isParameter())
    logFatal("FMUWrapper::setReal failed");

  fmi2_import_set_real(fmu, &v->getValueReference(), 1, &value);
}

void FMUWrapper::setRealParameter(const std::string& var, double value)
{
  logTrace();
  if(!fmu)
    logFatal("FMUWrapper::setRealParameter failed");

  Variable* v = getVariable(var);

  if(!v || !v->isParameter())
    logError("FMUWrapper::setRealParameter: FMU doesn't contain parameter " + var);

  fmi2_import_set_real(fmu, &v->getValueReference(), 1, &value);
}

void FMUWrapper::getDependencyGraph_outputs()
{
  size_t *startIndex, *dependency;
  char* factorKind;

  for (int i=0; i<allOutputs.size(); i++)
    outputsGraph.addVariable(allVariables[allOutputs[i]-1]);

  fmi2_import_get_outputs_dependencies(fmu, &startIndex, &dependency, &factorKind);

  if (!startIndex)
  {
    logDebug("FMUWrapper::getDependencyGraph: [" + instanceName + ": " + fmuPath + "] dependencies are not available");
    /* all outputs depend on all inputs */
    for (int i=0; i<allInputs.size(); i++)
      for (int j=0; j<allOutputs.size(); j++)
        outputsGraph.addEdge(allVariables[allInputs[i]-1], allVariables[allOutputs[j]-1]);
    return;
  }

  for (int i=0; i<allOutputs.size(); i++)
  {
    if (startIndex[i] == startIndex[i + 1])
    {
      logDebug("FMUWrapper::getDependencyGraph: [" + instanceName + ": " + fmuPath + "] output " + allVariables[allOutputs[i]-1].getName() + " has no dependencies");
    }
    else if ((startIndex[i] + 1 == startIndex[i + 1]) && (dependency[startIndex[i]] == 0))
    {
      logDebug("FMUWrapper::getDependencyGraph: [" + instanceName + ": " + fmuPath + "] output " + allVariables[allOutputs[i]-1].getName() + " depends on all");
      for (int j=0; j<allInputs.size(); j++)
        outputsGraph.addEdge(allVariables[allInputs[j]-1], allVariables[allOutputs[i]-1]);
    }
    else
    {
      for (int j=startIndex[i]; j<startIndex[i+1]; j++)
      {
        logDebug("FMUWrapper::getDependencyGraph: [" + instanceName + ": " + fmuPath + "] output " + allVariables[allOutputs[i]-1].getName() + " depends on " + allVariables[dependency[j]-1].getName());
        outputsGraph.addEdge(allVariables[dependency[j]-1], allVariables[allOutputs[i]-1]);
      }
    }
  }
}

void FMUWrapper::getDependencyGraph_initialUnknowns()
{
  size_t *startIndex, *dependency;
  char* factorKind;

  for (int i=0; i<initialUnknowns.size(); i++)
    initialUnknownsGraph.addVariable(allVariables[initialUnknowns[i]-1]);

  fmi2_import_get_initial_unknowns_dependencies(fmu, &startIndex, &dependency, &factorKind);

  if (!startIndex)
  {
    logDebug("FMUWrapper::getDependencyGraph_initialUnknowns: [" + instanceName + ": " + fmuPath + "] dependencies are not available");
    /* all outputs depend on all initial unknowns */
    /* all initial unknowns depend on all inputs */
    for (int i=0; i<initialUnknowns.size(); i++)
    {
      for (int j=0; j<allOutputs.size(); j++)
        initialUnknownsGraph.addEdge(allVariables[initialUnknowns[i]-1], allVariables[allOutputs[j]-1]);
      for (int j=0; j<allInputs.size(); j++)
        initialUnknownsGraph.addEdge(allVariables[allInputs[j]-1], allVariables[initialUnknowns[i]-1]);
    }
    return;
  }

  for (int i=0; i<initialUnknowns.size(); i++)
  {
    if (startIndex[i] == startIndex[i + 1])
    {
      logDebug("FMUWrapper::getDependencyGraph_initialUnknowns: [" + instanceName + ": " + fmuPath + "] initial unknown " + allVariables[initialUnknowns[i]-1].getName() + " has no dependencies");
    }
    else if ((startIndex[i] + 1 == startIndex[i + 1]) && (dependency[startIndex[i]] == 0))
    {
      logDebug("FMUWrapper::getDependencyGraph_initialUnknowns: [" + instanceName + ": " + fmuPath + "] initial unknown " + allVariables[initialUnknowns[i]-1].getName() + " depends on all");
      for (int j=0; j<allOutputs.size(); j++)
        initialUnknownsGraph.addEdge(allVariables[initialUnknowns[i]-1], allVariables[allOutputs[j]-1]);
      for (int j=0; j<allInputs.size(); j++)
        initialUnknownsGraph.addEdge(allVariables[allInputs[j]-1], allVariables[initialUnknowns[i]-1]);
    }
    else
    {
      for (int j=startIndex[i]; j<startIndex[i+1]; j++)
      {
        logDebug("FMUWrapper::getDependencyGraph_initialUnknowns: [" + instanceName + ": " + fmuPath + "] initial unknown " + allVariables[initialUnknowns[i]-1].getName() + " depends on " + allVariables[dependency[j]-1].getName());
        initialUnknownsGraph.addEdge(allVariables[dependency[j]-1], allVariables[initialUnknowns[i]-1]);
      }
    }
  }
}

Variable* FMUWrapper::getVariable(const std::string& name)
{
  for (int i=0; i<allVariables.size(); i++)
    if (name == allVariables[i].getName())
      return &allVariables[i];
  return NULL;
}

Variable* FMUWrapper::getVariable(const fmi2_value_reference_t& state_vr)
{
  for (int i=0; i<allVariables.size(); i++)
    if (state_vr == allVariables[i].getValueReference())
      return &allVariables[i];
  return NULL;
}

void FMUWrapper::describe()
{
  logTrace();

  const char* modelName = fmi2_import_get_model_name(fmu);
  const char* GUID = fmi2_import_get_GUID(fmu);

  std::cout << "Model name: " << modelName << std::endl;
  std::cout << "GUID: " << GUID << std::endl;

  if(fmi2_fmu_kind_me == fmuKind)
  {
    const char* modelIdentifier = fmi2_import_get_model_identifier_ME(fmu);
    std::cout << "Model identifier for ME: " << modelIdentifier << std::endl;
  }
  else if(fmi2_fmu_kind_cs == fmuKind)
  {
    const char* modelIdentifier = fmi2_import_get_model_identifier_CS(fmu);
    std::cout << "Model identifier for CS: " << modelIdentifier << std::endl;
  }
  else
    logError("Unsupported FMU kind: " + std::string(fmi2_fmu_kind_to_string(fmuKind)));
}

std::string FMUWrapper::getFMUKind()
{
  if(fmi2_fmu_kind_me == fmuKind) return "FMI 2.0 ME";
  if(fmi2_fmu_kind_cs == fmuKind) return "FMI 2.0 CS";
  else "Unsupported FMU kind";
}

std::string FMUWrapper::getGUID()
{
  const char* GUID = fmi2_import_get_GUID(fmu);
  return std::string(GUID);
}

std::string FMUWrapper::getGenerationTool()
{
  const char* tool = fmi2_import_get_generation_tool(fmu);
  if (tool)
    return std::string(tool);
  return "<unknown>";
}

void FMUWrapper::do_event_iteration()
{
  eventInfo.newDiscreteStatesNeeded = fmi2_true;
  eventInfo.terminateSimulation     = fmi2_false;
  while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation)
    fmi2_import_new_discrete_states(fmu, &eventInfo);
}

void FMUWrapper::enterInitialization(double startTime)
{
  fmi2_status_t fmistatus;

  double* pTolerance = model.getSettings().GetTolerance();
  relativeTolerance = pTolerance ? *pTolerance : fmi2_import_get_default_experiment_tolerance(fmu);
  tcur = startTime;
  fmi2_boolean_t toleranceControlled = fmi2_true;
  fmi2_boolean_t StopTimeDefined = fmi2_true;

  double* pStopTime = model.getSettings().GetStopTime();
  fmi2_real_t tend = pStopTime ? *pStopTime : fmi2_import_get_default_experiment_stop(fmu);

  logDebug("start time: " + toString(tcur));
  logDebug("relative tolerance: " + toString(relativeTolerance));

  if(fmi2_fmu_kind_me == fmuKind)
  {
    fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled, relativeTolerance, tcur, StopTimeDefined, tend);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_setup_experiment failed");

    fmistatus = fmi2_import_enter_initialization_mode(fmu);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_initialization_mode failed");
  }
  else if(fmi2_fmu_kind_cs == fmuKind)
  {
    fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled, relativeTolerance, tcur, StopTimeDefined, tend);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_setup_experiment failed");

    fmistatus = fmi2_import_enter_initialization_mode(fmu);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_initialization_mode failed");
  }
  else
    logFatal("Unsupported FMU kind");
}

void FMUWrapper::exitInitialization()
{
  fmi2_status_t fmistatus;

  const char* resultFile = model.getSettings().GetResultFile();
  std::string finalResultFile;
  if(resultFile)
    finalResultFile = std::string(resultFile) + "_" + instanceName + "_res.csv";
  else
    finalResultFile = instanceName + "_res.csv";
  logDebug("result file: " + finalResultFile);

  if(fmi2_fmu_kind_me == fmuKind)
  {
    fmistatus = fmi2_import_exit_initialization_mode(fmu);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_exit_initialization_mode failed");

    terminateSimulation = fmi2_false;
    omsResultFile = new Resultfile(finalResultFile, fmu);

    eventInfo.newDiscreteStatesNeeded           = fmi2_false;
    eventInfo.terminateSimulation               = fmi2_false;
    eventInfo.nominalsOfContinuousStatesChanged = fmi2_false;
    eventInfo.valuesOfContinuousStatesChanged   = fmi2_true;
    eventInfo.nextEventTimeDefined              = fmi2_false;
    eventInfo.nextEventTime                     = -0.0;

    // fmi2_import_exit_initialization_mode leaves FMU in event mode
    do_event_iteration();
    fmi2_import_enter_continuous_time_mode(fmu);
    omsResultFile->emit(tcur);

    callEventUpdate = fmi2_false;

    n_states = fmi2_import_get_number_of_continuous_states(fmu);
    n_event_indicators = fmi2_import_get_number_of_event_indicators(fmu);

    logDebug(toString(n_states) + " states");
    logDebug(toString(n_event_indicators) + " event indicators");

    states = (double*)calloc(n_states, sizeof(double));
    states_der = (double*)calloc(n_states, sizeof(double));
    event_indicators = (double*)calloc(n_event_indicators, sizeof(double));
    event_indicators_prev = (double*)calloc(n_event_indicators, sizeof(double));

    // get states and state derivatives
    fmistatus = fmi2_import_get_continuous_states(fmu, states, n_states);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_continuous_states failed");
    fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_derivatives failed");
    fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_event_indicators failed");
  }
  else if(fmi2_fmu_kind_cs == fmuKind)
  {
    fmistatus = fmi2_import_exit_initialization_mode(fmu);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_exit_initialization_mode failed");

    omsResultFile = new Resultfile(finalResultFile, fmu);
    omsResultFile->emit(tcur);
  }
  else
    logFatal("Unsupported FMU kind");
}

void FMUWrapper::terminate()
{
  if(fmi2_fmu_kind_me == fmuKind)
  {
    free(states);
    free(states_der);
  }

  fmi2_status_t fmistatus = fmi2_import_terminate(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_terminate failed");
}

void FMUWrapper::doStep(double stopTime)
{
  fmi2_status_t fmistatus;
  fmi2_real_t hdef = 1e-2;

  if(fmi2_fmu_kind_me == fmuKind)
  {
    // main simulation loop
    fmi2_real_t hcur = hdef;
    fmi2_real_t tlast = tcur;
    while ((tcur < stopTime) && (!(eventInfo.terminateSimulation || terminateSimulation)))
    {
      fmistatus = fmi2_import_set_time(fmu, tcur);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_set_time failed");

      // swap event_indicators and event_indicators_prev
      {
        fmi2_real_t *temp = event_indicators;
        event_indicators = event_indicators_prev;
        event_indicators_prev = temp;

        fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
        if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_event_indicators failed");
      }

      // check if an event indicator has triggered
      int zero_crossing_event = 0;
      for (int k=0; k<n_event_indicators; k++)
      {
        if ((event_indicators[k] > 0) != (event_indicators_prev[k] > 0))
        {
          zero_crossing_event = 1;
          break;
        }
      }

      // handle events
      if (callEventUpdate || zero_crossing_event || (eventInfo.nextEventTimeDefined && tcur == eventInfo.nextEventTime))
      {
        fmistatus = fmi2_import_enter_event_mode(fmu);
        if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_event_mode failed");

        do_event_iteration();

        fmistatus = fmi2_import_enter_continuous_time_mode(fmu);
        if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_continuous_time_mode failed");
        fmistatus = fmi2_import_get_continuous_states(fmu, states, n_states);
        if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_continuous_states failed");
        fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
        if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_derivatives failed");
        fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
        if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_event_indicators failed");
        omsResultFile->emit(tcur);
      }

      // calculate next time step
      tlast = tcur;
      tcur += hdef;
      if (eventInfo.nextEventTimeDefined && (tcur >= eventInfo.nextEventTime))
        tcur = eventInfo.nextEventTime;

      hcur = tcur - tlast;

      if(tcur > stopTime - hcur/1e16)
      {
        // adjust final step size
        tcur = stopTime;
        hcur = tcur - tlast;
      }

      for(int k=0; k<n_states; k++)
        states[k] = states[k] + hcur*states_der[k];

      // set states
      fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_set_continuous_states failed");
      // get state derivatives
      fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_derivatives failed");

      // step is complete
      fmistatus = fmi2_import_completed_integrator_step(fmu, fmi2_true, &callEventUpdate, &terminateSimulation);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_completed_integrator_step failed");

      omsResultFile->emit(tcur);
    }
  }
  else if(fmi2_fmu_kind_cs == fmuKind)
  {
    while (tcur < stopTime)
    {
      fmistatus = fmi2_import_do_step(fmu, tcur, hdef, fmi2_true);
      tcur += hdef;
      omsResultFile->emit(tcur);
    }
  }
}

void FMUWrapper::simulate()
{
  fmi2_status_t fmistatus;

  const char* resultFile = model.getSettings().GetResultFile();
  std::string finalResultFile;
  if(resultFile)
    finalResultFile = resultFile;
  else
    finalResultFile = std::string(fmi2_import_get_model_name(fmu)) + "_res.csv";
  logDebug("result file: " + finalResultFile);

  if(fmi2_fmu_kind_me == fmuKind)
    simulate_me(finalResultFile);
  else if(fmi2_fmu_kind_cs == fmuKind)
    simulate_cs(finalResultFile);

  fmistatus = fmi2_import_terminate(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_terminate failed");
}

void FMUWrapper::simulate_cs(const std::string& resultFileName)
{
  fmi2_status_t fmistatus;

  double* pStartTime = model.getSettings().GetStartTime();
  double* pStopTime = model.getSettings().GetStopTime();
  double* pTolerance = model.getSettings().GetTolerance();
  fmi2_real_t relativeTolerance = pTolerance ? *pTolerance : fmi2_import_get_default_experiment_tolerance(fmu);
  fmi2_real_t tstart = pStartTime ? *pStartTime : fmi2_import_get_default_experiment_start(fmu);
  fmi2_real_t tend = pStopTime ? *pStopTime : fmi2_import_get_default_experiment_stop(fmu);
  fmi2_real_t tcur = tstart;
  fmi2_real_t hdef = 1e-2;
  fmi2_boolean_t toleranceControlled = fmi2_true;
  fmi2_boolean_t StopTimeDefined = fmi2_true;

  logDebug("start time: " + toString(tstart));
  logDebug("stop time: " + toString(tend));
  logDebug("relative tolerance: " + toString(relativeTolerance));

  fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled, relativeTolerance, tstart, StopTimeDefined, tend);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_setup_experiment failed");

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_initialization_mode failed");

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_exit_initialization_mode failed");

  Resultfile resultFile(resultFileName, fmu);
  resultFile.emit(tcur);

  while (tcur < tend)
  {
    fmistatus = fmi2_import_do_step(fmu, tcur, hdef, fmi2_true);
    tcur += hdef;
    resultFile.emit(tcur);
  }
}

void FMUWrapper::simulate_me(const std::string& resultFileName)
{
  fmi2_status_t fmistatus;

  double* pStartTime = model.getSettings().GetStartTime();
  double* pStopTime = model.getSettings().GetStopTime();
  double* pTolerance = model.getSettings().GetTolerance();
  fmi2_real_t tstart = pStartTime ? *pStartTime : fmi2_import_get_default_experiment_start(fmu);
  fmi2_real_t tend = pStopTime ? *pStopTime : fmi2_import_get_default_experiment_stop(fmu);
  fmi2_real_t relativeTolerance = pTolerance ? *pTolerance : fmi2_import_get_default_experiment_tolerance(fmu);
  fmi2_real_t tcur = tstart;
  fmi2_boolean_t toleranceControlled = fmi2_true;
  fmi2_boolean_t StopTimeDefined = fmi2_true;

  logDebug("start time: " + toString(tstart));
  logDebug("stop time: " + toString(tend));
  logDebug("relative tolerance: " + toString(relativeTolerance));

  fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled, relativeTolerance, tstart, StopTimeDefined, tend);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_setup_experiment failed");

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_initialization_mode failed");

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_exit_initialization_mode failed");

  fmi2_boolean_t terminateSimulation = fmi2_false;
  Resultfile resultFile(resultFileName, fmu);

  eventInfo.newDiscreteStatesNeeded           = fmi2_false;
  eventInfo.terminateSimulation               = fmi2_false;
  eventInfo.nominalsOfContinuousStatesChanged = fmi2_false;
  eventInfo.valuesOfContinuousStatesChanged   = fmi2_true;
  eventInfo.nextEventTimeDefined              = fmi2_false;
  eventInfo.nextEventTime                     = -0.0;

  // fmi2_import_exit_initialization_mode leaves FMU in event mode
  do_event_iteration();
  fmi2_import_enter_continuous_time_mode(fmu);
  resultFile.emit(tcur);

  fmi2_boolean_t callEventUpdate = fmi2_false;

  size_t n_states = fmi2_import_get_number_of_continuous_states(fmu);
  size_t n_event_indicators = fmi2_import_get_number_of_event_indicators(fmu);

  logDebug(toString(n_states) + " states");
  logDebug(toString(n_event_indicators) + " event indicators");

  double* states = (double*)calloc(n_states, sizeof(double));
  double* states_der = (double*)calloc(n_states, sizeof(double));
  double* event_indicators = (double*)calloc(n_event_indicators, sizeof(double));
  double* event_indicators_prev = (double*)calloc(n_event_indicators, sizeof(double));

  // get states and state derivatives
  fmistatus = fmi2_import_get_continuous_states(fmu, states, n_states);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_continuous_states failed");
  fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_derivatives failed");
  fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_event_indicators failed");

  // main simulation loop
  fmi2_real_t hdef = 1e-2;
  fmi2_real_t hcur = hdef;
  fmi2_real_t tlast = tstart;
  while ((tcur < tend) && (!(eventInfo.terminateSimulation || terminateSimulation)))
  {
    fmistatus = fmi2_import_set_time(fmu, tcur);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_set_time failed");

    // swap event_indicators and event_indicators_prev
    {
      fmi2_real_t *temp = event_indicators;
      event_indicators = event_indicators_prev;
      event_indicators_prev = temp;

      fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_event_indicators failed");
    }

    // check if an event indicator has triggered
    int zero_crossing_event = 0;
    for (int k=0; k<n_event_indicators; k++)
    {
      if ((event_indicators[k] > 0) != (event_indicators_prev[k] > 0))
      {
        zero_crossing_event = 1;
        break;
      }
    }

    // handle events
    if (callEventUpdate || zero_crossing_event || (eventInfo.nextEventTimeDefined && tcur == eventInfo.nextEventTime))
    {
      fmistatus = fmi2_import_enter_event_mode(fmu);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_event_mode failed");

      do_event_iteration();

      fmistatus = fmi2_import_enter_continuous_time_mode(fmu);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_continuous_time_mode failed");
      fmistatus = fmi2_import_get_continuous_states(fmu, states, n_states);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_continuous_states failed");
      fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_derivatives failed");
      fmistatus = fmi2_import_get_event_indicators(fmu, event_indicators, n_event_indicators);
      if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_event_indicators failed");
      resultFile.emit(tcur);
    }

    // calculate next time step
    tlast = tcur;
    tcur += hdef;
    if (eventInfo.nextEventTimeDefined && (tcur >= eventInfo.nextEventTime))
      tcur = eventInfo.nextEventTime;

    hcur = tcur - tlast;

    if(tcur > tend - hcur/1e16)
    {
      // adjust final step size
      tcur = tend;
      hcur = tcur - tlast;
    }

    for(int k=0; k<n_states; k++)
      states[k] = states[k] + hcur*states_der[k];

    // set states
    fmistatus = fmi2_import_set_continuous_states(fmu, states, n_states);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_set_continuous_states failed");
    // get state derivatives
    fmistatus = fmi2_import_get_derivatives(fmu, states_der, n_states);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_get_derivatives failed");

    // step is complete
    fmistatus = fmi2_import_completed_integrator_step(fmu, fmi2_true, &callEventUpdate, &terminateSimulation);
    if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_completed_integrator_step failed");

    resultFile.emit(tcur);
  }

  free(states);
  free(states_der);
}
