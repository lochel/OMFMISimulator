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

#include "oms_model.h"
#include "oms_logging.h"
#include "oms_resultfile.h"
#include "Settings.h"

#include <fmilib.h>
#include <JM/jm_portability.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdlib.h>

#include <boost/filesystem.hpp>

template<typename T>
std::string toString(const T& value)
{
  std::ostringstream oss;
  oss << value;
  return oss.str();
}

void fmiLogger(jm_callbacks* c, jm_string module, jm_log_level_enu_t log_level, jm_string message)
{
  switch(log_level)
  {
    case jm_log_level_info:
      logInfo("module " + std::string(module) + ": " + std::string(message));
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
      logInfo(std::string(instanceName) + " (" + category + "): " + msg);
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

oms_model::oms_model(std::string fmuPath)
  : fmuPath(fmuPath)
{
  logTrace();

  if (!boost::filesystem::exists(fmuPath))
    logFatal("Specified file name does not exist: \"" + fmuPath + "\"");

  callbacks.malloc = malloc;
  callbacks.calloc = calloc;
  callbacks.realloc = realloc;
  callbacks.free = free;
  callbacks.logger = fmiLogger;
  callbacks.log_level = jm_log_level_warning;
  callbacks.context = 0;

  //set working directory
  tempDir = fmi_import_mk_temp_dir(&callbacks, Settings::getInstance().GetTempDirectory(), "temp_");
  logInfo("set working directory to \"" + tempDir + "\"");

  context = fmi_import_allocate_context(&callbacks);
  fmu = NULL;
}

oms_model::~oms_model()
{
  logTrace();
  fmi_import_free_context(context);
  if (boost::filesystem::is_directory(tempDir))
  {
    fmi_import_rmdir(&callbacks, tempDir.c_str());
    logInfo("removed working directory: \"" + tempDir + "\"");
  }
}

void oms_model::describe()
{
  logTrace();

  // check version of FMU
  fmi_version_enu_t version = fmi_import_get_fmi_version(context, fmuPath.c_str(), tempDir.c_str());
  if(fmi_version_2_0_enu == version)
    std::cout << "FMI version: 2.0" << std::endl;
  else
  {
    logError("Unsupported FMI version: " + std::string(fmi_version_to_string(version)));
    return;
  }

  // parse modelDescription.xml
  fmu = fmi2_import_parse_xml(context, tempDir.c_str(), 0);
  if(!fmu)
  {
    logFatal("Error parsing modelDescription.xml");
    return;
  }

  const char* modelName = fmi2_import_get_model_name(fmu);
  const char* GUID = fmi2_import_get_GUID(fmu);

  std::cout << "Model name: " << modelName << std::endl;
  std::cout << "GUID: " << GUID << std::endl;

  fmi2_fmu_kind_enu_t fmuKind = fmi2_import_get_fmu_kind(fmu);
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

  fmi2_import_free(fmu);
}

void oms_model::do_event_iteration()
{
  eventInfo.newDiscreteStatesNeeded = fmi2_true;
  eventInfo.terminateSimulation     = fmi2_false;
  while (eventInfo.newDiscreteStatesNeeded && !eventInfo.terminateSimulation)
    fmi2_import_new_discrete_states(fmu, &eventInfo);
}

void oms_model::simulate(const char* resultFile)
{
  fmi2_status_t fmistatus;

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
  fmi2_fmu_kind_enu_t fmuKind = fmi2_import_get_fmu_kind(fmu);
  if(fmi2_fmu_kind_me == fmuKind)
    logInfo("FMU ME");
  else if(fmi2_fmu_kind_cs == fmuKind)
    logInfo("FMU CS");
  else
    logError("Unsupported FMU kind: " + std::string(fmi2_fmu_kind_to_string(fmuKind)));

  fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu, 0);
  size_t nVar = fmi2_import_get_variable_list_size(list);
  logInfo(toString(nVar) + " variables");
  for(size_t i=0; i<nVar; ++i)
  {
    fmi2_import_variable_t* var = fmi2_import_get_variable(list, i);
    fmi2_causality_enu_t varCausality = fmi2_import_get_causality(var);
    if(fmi2_causality_enu_parameter == varCausality ||
       fmi2_causality_enu_input == varCausality ||
       fmi2_causality_enu_output == varCausality)
    {
      std::string name = fmi2_import_get_variable_name(var);
      std::string value;
      std::string causality = fmi2_causality_to_string(varCausality);

      if(fmi2_base_type_real == fmi2_import_get_variable_base_type(var))
      {
        fmi2_import_real_variable_t *var_real = fmi2_import_get_variable_as_real(var);
        double value_real = fmi2_import_get_real_variable_start(var_real);
        value = toString(value_real);
      }
      else if(fmi2_base_type_int == fmi2_import_get_variable_base_type(var))
      {
        fmi2_import_integer_variable_t *var_int = fmi2_import_get_variable_as_integer(var);
        int value_int = fmi2_import_get_integer_variable_start(var_int);
        value = toString(value_int);
      }
      else if(fmi2_base_type_bool == fmi2_import_get_variable_base_type(var))
      {
        fmi2_import_bool_variable_t *var_bool = fmi2_import_get_variable_as_boolean(var);
        int value_bool = fmi2_import_get_boolean_variable_start(var_bool);
        value = value_bool ? "true" : "false";
      }
      else if(fmi2_base_type_str == fmi2_import_get_variable_base_type(var))
      {
        fmi2_import_string_variable_t *var_str = fmi2_import_get_variable_as_string(var);
        value = fmi2_import_get_string_variable_start(var_str);
      }

      logInfo(causality + " " + name + " = " + value);
    }
  }

  callBackFunctions.logger = fmi2logger;
  callBackFunctions.allocateMemory = calloc;
  callBackFunctions.freeMemory = free;
  callBackFunctions.componentEnvironment = fmu;

  std::string finalResultFile;
  if(resultFile)
    finalResultFile = resultFile;
  else
    finalResultFile = std::string(fmi2_import_get_model_name(fmu)) + "_res.csv";
  logInfo("result file: " + finalResultFile);

  if(fmi2_fmu_kind_me == fmuKind)
    simulate_me(finalResultFile);
  else if(fmi2_fmu_kind_cs == fmuKind)
    simulate_cs(finalResultFile);

  fmistatus = fmi2_import_terminate(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_terminate failed");
  fmi2_import_free_instance(fmu);
  fmi2_import_destroy_dllfmu(fmu);
  fmi2_import_free(fmu);
}

void oms_model::simulate_cs(std::string resultFileName)
{
  fmi2_status_t fmistatus;
  jm_status_enu_t jmstatus;

  //Load the FMU shared library
  jmstatus = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_cs, &callBackFunctions);
  if (jm_status_error == jmstatus) logFatal("Could not create the DLL loading mechanism (C-API). Error: " + std::string(fmi2_import_get_last_error(fmu)));

  logInfo("Version returned from FMU: " + std::string(fmi2_import_get_version(fmu)));
  logInfo("Platform type returned: " + std::string(fmi2_import_get_types_platform(fmu)));
  logInfo("GUID: " + std::string(fmi2_import_get_GUID(fmu)));

  fmi2_string_t instanceName = "CS-FMU instance";
  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_cosimulation, NULL, fmi2_false);
  if (jm_status_error == jmstatus) logFatal("fmi2_import_instantiate failed");

  double* pStartTime = Settings::getInstance().GetStartTime();
  double* pStopTime = Settings::getInstance().GetStopTime();
  double* pTolerance = Settings::getInstance().GetTolerance();
  fmi2_real_t tstart = pStartTime ? *pStartTime : fmi2_import_get_default_experiment_start(fmu);
  fmi2_real_t tend = pStopTime ? *pStopTime : fmi2_import_get_default_experiment_stop(fmu);
  fmi2_real_t relativeTolerance = pTolerance ? *pTolerance : fmi2_import_get_default_experiment_tolerance(fmu);
  fmi2_real_t tcur = tstart;
  fmi2_boolean_t toleranceControlled = fmi2_true;
  fmi2_boolean_t StopTimeDefined = fmi2_true;
  fmi2_real_t hdef = 1e-2;

  logInfo("start time: " + toString(tstart));
  logInfo("stop time: " + toString(tend));
  logInfo("relative tolerance: " + toString(relativeTolerance));

  fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled, relativeTolerance, tstart, StopTimeDefined, tend);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_setup_experiment failed");

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_initialization_mode failed");

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_exit_initialization_mode failed");

  oms_resultfile resultFile(resultFileName, fmu);
  resultFile.emit(tcur);

  while (tcur < tend)
  {
    fmistatus = fmi2_import_do_step(fmu, tcur, hdef, fmi2_true);
    tcur += hdef;
    resultFile.emit(tcur);
  }
}

void oms_model::simulate_me(std::string resultFileName)
{
  fmi2_status_t fmistatus;
  jm_status_enu_t jmstatus;

  //Load the FMU shared library
  jmstatus = fmi2_import_create_dllfmu(fmu, fmi2_fmu_kind_me, &callBackFunctions);
  if (jm_status_error == jmstatus) logFatal("Could not create the DLL loading mechanism (C-API). Error: " + std::string(fmi2_import_get_last_error(fmu)));

  logInfo("Version returned from FMU: " + std::string(fmi2_import_get_version(fmu)));
  logInfo("Platform type returned: " + std::string(fmi2_import_get_types_platform(fmu)));
  logInfo("GUID: " + std::string(fmi2_import_get_GUID(fmu)));

  size_t n_states = fmi2_import_get_number_of_continuous_states(fmu);
  size_t n_event_indicators = fmi2_import_get_number_of_event_indicators(fmu);

  logInfo(toString(n_states) + " states");
  logInfo(toString(n_event_indicators) + " event indicators");

  fmi2_string_t instanceName = "ME-FMU instance";
  jmstatus = fmi2_import_instantiate(fmu, instanceName, fmi2_model_exchange, NULL, fmi2_false);
  if (jm_status_error == jmstatus) logFatal("fmi2_import_instantiate failed");

  double* pStartTime = Settings::getInstance().GetStartTime();
  double* pStopTime = Settings::getInstance().GetStopTime();
  double* pTolerance = Settings::getInstance().GetTolerance();
  fmi2_real_t tstart = pStartTime ? *pStartTime : fmi2_import_get_default_experiment_start(fmu);
  fmi2_real_t tend = pStopTime ? *pStopTime : fmi2_import_get_default_experiment_stop(fmu);
  fmi2_real_t relativeTolerance = pTolerance ? *pTolerance : fmi2_import_get_default_experiment_tolerance(fmu);
  fmi2_real_t tcur = tstart;
  fmi2_boolean_t toleranceControlled = fmi2_true;
  fmi2_boolean_t StopTimeDefined = fmi2_true;

  logInfo("start time: " + toString(tstart));
  logInfo("stop time: " + toString(tend));
  logInfo("relative tolerance: " + toString(relativeTolerance));

  fmistatus = fmi2_import_setup_experiment(fmu, toleranceControlled, relativeTolerance, tstart, StopTimeDefined, tend);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_setup_experiment failed");

  fmistatus = fmi2_import_enter_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_enter_initialization_mode failed");

  fmistatus = fmi2_import_exit_initialization_mode(fmu);
  if (fmi2_status_ok != fmistatus) logFatal("fmi2_import_exit_initialization_mode failed");

  fmi2_boolean_t terminateSimulation = fmi2_false;
  oms_resultfile resultFile(resultFileName, fmu);

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
