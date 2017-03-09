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
#include "DirectedGraph.h"
#include "Settings.h"

#include <fmilib.h>
#include <JM/jm_portability.h>

#include <iostream>
#include <string>
#include <map>
#include <sstream>
#include <stdlib.h>

#include <boost/filesystem.hpp>

oms_model::oms_model()
  : fmuInstances()
{
  logTrace();
}

oms_model::oms_model(const std::string& descriptionPath)
{
  logTrace();
  logError("Function not implemented yet: oms_model::oms_model");
}

oms_model::~oms_model()
{
  logTrace();

  std::map<std::string, oms_fmu*>::iterator it;
  for (it=fmuInstances.begin(); it != fmuInstances.end(); it++)
    delete it->second;
}

void oms_model::instantiateFMU(const std::string& filename, const std::string& instanceName)
{
  fmuInstances[instanceName] = new oms_fmu(filename, instanceName);
  outputsGraph.includeGraph(fmuInstances[instanceName]->getOutputsGraph());
}

void oms_model::setReal(const std::string& var, double value)
{
  std::stringstream var_(var);
  std::string fmuInstance;
  std::string fmuVar;

  // TODO: Improve this
  std::getline(var_, fmuInstance, '.');
  std::getline(var_, fmuVar, '.');

  if (fmuInstances.find(fmuInstance) == fmuInstances.end())
  {
    logError("oms_model::setReal: FMU instance \"" + fmuInstance + "\" doesn't exist in model");
    return;
  }

  fmuInstances[fmuInstance]->setReal(fmuVar, value);
}

void oms_model::addConnection(const std::string& from, const std::string& to)
{
  //logError("Function not implemented yet: oms_model::addConnection");
  std::stringstream var1_(from);
  std::stringstream var2_(to);
  std::string fmuInstance1, fmuInstance2;
  std::string fmuVar1, fmuVar2;

  // TODO: Improve this
  std::getline(var1_, fmuInstance1, '.');
  std::getline(var1_, fmuVar1, '.');

  // TODO: Improve this
  std::getline(var2_, fmuInstance2, '.');
  std::getline(var2_, fmuVar2, '.');

  if (fmuInstances.find(fmuInstance1) == fmuInstances.end())
  {
    logError("oms_model::addConnection: FMU instance \"" + fmuInstance1 + "\" doesn't exist in model");
    return;
  }

  if (fmuInstances.find(fmuInstance2) == fmuInstances.end())
  {
    logError("oms_model::addConnection: FMU instance \"" + fmuInstance2 + "\" doesn't exist in model");
    return;
  }

  Variable *var1 = fmuInstances[fmuInstance1]->getVariable(fmuVar1);
  Variable *var2 = fmuInstances[fmuInstance2]->getVariable(fmuVar2);

  outputsGraph.addEdge(*var1, *var2);
}

void oms_model::exportDependencyGraph(const std::string& prefix)
{
  logTrace();
  outputsGraph.dotExport(prefix + ".dot");
}

void oms_model::describe()
{
  logTrace();

  std::map<std::string, oms_fmu*>::iterator it;
  for (it=fmuInstances.begin(); it != fmuInstances.end(); it++)
  {
    std::cout << it->first << std::endl;
    it->second->describe();
  }
}

void oms_model::simulate()
{
  logTrace();

  std::map<std::string, oms_fmu*>::iterator it;
  for (it=fmuInstances.begin(); it != fmuInstances.end(); it++)
    it->second->simulate();
}

