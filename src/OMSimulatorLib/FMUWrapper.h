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

#ifndef _OMS_FMU_H_
#define _OMS_FMU_H_

#include "Variable.h"
#include "DirectedGraph.h"
#include "Resultfile.h"

#include <fmilib.h>
#include <string>
#include <vector>
#include <map>

class CompositeModel;

class FMUWrapper
{
public:
  FMUWrapper(CompositeModel& model, std::string fmuPath, std::string instanceName);
  ~FMUWrapper();

  double getReal(const std::string& var);
  void setReal(const std::string& var, double value);
  void setRealParameter(const std::string& var, double value);
  void describe();
  void simulate();

  void enterInitialization(double startTime);
  void exitInitialization();
  void terminate();
  void doStep(double stopTime);

  const DirectedGraph& getOutputsGraph() {return outputsGraph;}
  const DirectedGraph& getInitialUnknownsGraph() {return initialUnknownsGraph;}
  Variable* getVariable(const std::string& varName);

  std::string& getFMUPath() {return fmuPath;}
  std::string getFMUInstanceName() {return instanceName;}
  std::string getFMUKind();
  std::string getGUID();
  std::string getGenerationTool();

private:
  void do_event_iteration();
  void simulate_cs(const std::string& resultFileName);
  void simulate_me(const std::string& resultFileName);
  void getDependencyGraph();
  void getDependencyGraph_Initialization();

private:
  CompositeModel& model;

  std::string fmuPath;
  std::string tempDir;
  std::string instanceName;
  jm_callbacks callbacks;
  fmi2_fmu_kind_enu_t fmuKind;
  fmi2_callback_functions_t callBackFunctions;
  fmi_import_context_t* context;
  fmi2_import_t* fmu;
  fmi2_event_info_t eventInfo;

  std::map<std::string, fmi2_value_reference_t> parameterLookUp;
  std::vector<Variable> outputs;
  std::vector<Variable> allVariables;
  std::vector<Variable> initialUnknowns;

  DirectedGraph outputsGraph;
  DirectedGraph initialUnknownsGraph;

  // ME & CS
  fmi2_real_t tcur;
  fmi2_real_t relativeTolerance;
  Resultfile *omsResultFile;

  // ME
  fmi2_boolean_t callEventUpdate;
  fmi2_boolean_t terminateSimulation;
  size_t n_states;
  size_t n_event_indicators;
  double* states;
  double* states_der;
  double* event_indicators;
  double* event_indicators_prev;
};

#endif
