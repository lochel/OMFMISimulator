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

#ifndef _OMS_MODEL_H_
#define _OMS_MODEL_H_

#include <fmilib.h>
#include <string>

class oms_model
{
public:
  oms_model(std::string fmuPath);
  ~oms_model();

  void describe();
  void simulate(double* startTime, double* stopTime, double* tolerance, const char* resultFile);
  void setWorkingDirectory(std::string tempDir);

private:
  void do_event_iteration();
  void simulate_cs(double* startTime, double* stopTime, double* tolerance, std::string resultFileName);
  void simulate_me(double* startTime, double* stopTime, double* tolerance, std::string resultFileName);

private:
  std::string fmuPath;
  std::string tmpPath;
  jm_callbacks callbacks;
  fmi2_callback_functions_t callBackFunctions;
  fmi_import_context_t* context;
  fmi2_import_t* fmu;
  fmi2_event_info_t eventInfo;
};

#endif
