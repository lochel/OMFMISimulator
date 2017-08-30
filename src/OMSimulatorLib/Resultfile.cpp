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

#include <iostream>
#include <fstream>
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
}

Resultfile::~Resultfile()
{
  this->close();
}

bool Resultfile::create(const std::string& filename)
{
  globalClocks.tic(GLOBALCLOCK_RESULTFILE);

  resultFile.open(filename.c_str());
  if (resultFile.is_open())
    logInfo("Result file: " + filename);
  else
  {
    logWarning("Error opening result file \"" + filename + "\"");
    globalClocks.toc(GLOBALCLOCK_RESULTFILE);
    return false;
  }

  resultFile << "time";

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
      resultFile << ", " << varName;
    }
    for (int j=0; j<allOutputs.size(); j++)
    {
      varName = allVariables[allOutputs[j]].getFMUInstanceName() + "." + allVariables[allOutputs[j]].getName();
      replaceAll(varName, ",", "$C");
      resultFile << ", " << varName;
    }
  }

  resultFile << "\n";
  globalClocks.toc(GLOBALCLOCK_RESULTFILE);
  return true;
}

void Resultfile::close()
{
  globalClocks.tic(GLOBALCLOCK_RESULTFILE);
  if (resultFile.is_open())
  {
    resultFile.close();
    logDebug("Result file closed");
  }
  globalClocks.toc(GLOBALCLOCK_RESULTFILE);
}

void Resultfile::addInstance(FMUWrapper* instance)
{
  globalClocks.tic(GLOBALCLOCK_RESULTFILE);
  instances.push_back(instance);
  globalClocks.toc(GLOBALCLOCK_RESULTFILE);
}

void Resultfile::emitVariable(FMUWrapper* instance, const Variable& var)
{
  globalClocks.tic(GLOBALCLOCK_RESULTFILE);

  switch (var.getBaseType())
  {
    case fmi2_base_type_real:
      resultFile << ", " << instance->getReal(var);
      break;
    //case fmi2_base_type_int:
    //case fmi2_base_type_bool:
    //case fmi2_base_type_string:
    default:
      logFatal("Resultfile::emitVariable: unsupported base type");
  }
  globalClocks.toc(GLOBALCLOCK_RESULTFILE);
}

void Resultfile::emit(double time)
{
  globalClocks.tic(GLOBALCLOCK_RESULTFILE);
  resultFile << time;

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

  resultFile << "\n";
  globalClocks.toc(GLOBALCLOCK_RESULTFILE);
}
