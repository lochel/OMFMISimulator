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

#include "Resultfile.h"
#include "Logging.h"
#include "Util.h"
#include "Clocks.h"
#include "Variable.h"
#include "FMUWrapper.h"

#include <fmilib.h>

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

// https://stackoverflow.com/a/3418285
void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
  if (from.empty())
    return;

  size_t start_pos = 0;
  while ((start_pos = str.find(from, start_pos)) != std::string::npos)
  {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
  }
}

Resultfile::Resultfile()
{
#ifdef USE_C_FILE
  resultFile = NULL;
  logInfo("USING C FILE");
#else
  logInfo("USING C++ FILE");
#endif
}

Resultfile::~Resultfile()
{
  this->close();
}

bool Resultfile::create(const std::string& filename)
{
  OMS_TIC(globalClocks, GLOBALCLOCK_RESULTFILE);

#ifdef USE_C_FILE
  resultFile = fopen(filename.c_str(), "w");
  if (resultFile)
    logInfo("Result file: " + filename);
  else
  {
    logWarning("Error opening result file \"" + filename + "\"");
    OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
    return false;
  }
#else
  resultFile.open(filename.c_str());
  if (resultFile.is_open())
    logInfo("Result file: " + filename);
  else
  {
    logWarning("Error opening result file \"" + filename + "\"");
    OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
    return false;
  }
#endif

#ifdef USE_C_FILE
  fprintf(resultFile, "time");
#else
  resultFile << "time";
#endif

  for (int i=0; i<instances.size(); i++)
  {
    std::vector<Variable>& allVariables = instances[i]->getAllVariables();
    std::vector<unsigned int>& allInputs = instances[i]->getAllInputs();
    std::vector<unsigned int>& allOutputs = instances[i]->getAllOutputs();

    std::string varName;
    for (int j=0; j<allInputs.size(); j++)
    {
      varName = allVariables[allInputs[j]].getFMUInstanceName() + "." + allVariables[allInputs[j]].getName();
      replaceAll(varName, ",", "$C");
#ifdef USE_C_FILE
      fprintf(resultFile, ", %s", varName.c_str());
#else
      resultFile << ", " << varName;
#endif
    }
    for (int j=0; j<allOutputs.size(); j++)
    {
      varName = allVariables[allOutputs[j]].getFMUInstanceName() + "." + allVariables[allOutputs[j]].getName();
      replaceAll(varName, ",", "$C");

#ifdef USE_C_FILE
      fprintf(resultFile, ", %s", varName.c_str());
#else
      resultFile << ", " << varName;
#endif
    }
  }

#ifdef USE_C_FILE
  fprintf(resultFile, "\n");
#else
  resultFile << "\n";
#endif
  OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
  return true;
}

void Resultfile::close()
{
  OMS_TIC(globalClocks, GLOBALCLOCK_RESULTFILE);

#ifdef USE_C_FILE
  if (resultFile)
  {
    fclose(resultFile);
    resultFile = NULL;
    logDebug("Result file closed");
  }
#else
  if (resultFile.is_open())
  {
    resultFile.close();
    logDebug("Result file closed");
  }
#endif
  OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
}

void Resultfile::addInstance(FMUWrapper* instance)
{
  OMS_TIC(globalClocks, GLOBALCLOCK_RESULTFILE);
  instances.push_back(instance);
  OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
}

void Resultfile::emitVariable(FMUWrapper* instance, const Variable& var)
{
  OMS_TIC(globalClocks, GLOBALCLOCK_RESULTFILE);

  switch (var.getBaseType())
  {
    case fmi2_base_type_real:

#ifdef USE_C_FILE
      fprintf(resultFile, ", %g", instance->getReal(var));
#else
      resultFile << ", " << instance->getReal(var);
#endif
      break;
    //case fmi2_base_type_int:
    //case fmi2_base_type_bool:
    //case fmi2_base_type_string:
    default:
      logFatal("Resultfile::emitVariable: unsupported base type");
  }
  OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
}

void Resultfile::emit(double time)
{
#ifdef USE_C_FILE
  if (!resultFile)
    return;
#else
  if (!resultFile.is_open())
    return;
#endif

  OMS_TIC(globalClocks, GLOBALCLOCK_RESULTFILE);
#ifdef USE_C_FILE
  fprintf(resultFile, "%g", time);
#else
  resultFile << time;
#endif

  for (int i=0; i<instances.size(); i++)
  {
    const std::vector<Variable>& allVariables = instances[i]->getAllVariables();
    const std::vector<unsigned int>& allInputs = instances[i]->getAllInputs();
    const std::vector<unsigned int>& allOutputs = instances[i]->getAllOutputs();

    for (int j=0; j<allInputs.size(); j++)
      emitVariable(instances[i], allVariables[allInputs[j]]);
    for (int j=0; j<allOutputs.size(); j++)
      emitVariable(instances[i], allVariables[allOutputs[j]]);
  }

#ifdef USE_C_FILE
  fprintf(resultFile, "\n");
#else
  resultFile << "\n";
#endif
  OMS_TOC(globalClocks, GLOBALCLOCK_RESULTFILE);
}
