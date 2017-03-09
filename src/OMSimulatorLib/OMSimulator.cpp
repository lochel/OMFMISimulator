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

#include "OMSimulator.h"
#include "oms_model.h"
#include "oms_logging.h"
#include "Settings.h"
#include "oms_version.h"

void* oms_newModel()
{
  logTrace();
  oms_model* pModel = new oms_model();
  return (void*) pModel;
}

void* oms_loadModel(const char* filename)
{
  logTrace();
  oms_model* pModel = new oms_model(filename);
  return (void*) pModel;
}

void oms_unload(void* model)
{
  logTrace();
  if(!model)
  {
    logError("oms_unload: invalid pointer");
    return;
  }

  oms_model* pModel = (oms_model*) model;
  delete pModel;
}

void oms_instantiateFMU(void* model, const char* filename, const char* instanceName)
{
  logTrace();
  if(!model)
  {
    logError("oms_simulate: invalid pointer");
    return;
  }

  oms_model* pModel = (oms_model*) model;
  pModel->instantiateFMU(filename, instanceName);
}

void oms_setReal(void *model, const char *var, double value)
{
  logTrace();
  if (!model)
  {
    logError("oms_simulate: invalid pointer");
    return;
  }

  oms_model *pModel = (oms_model *)model;
  pModel->setReal(var, value);
}

void oms_addConnection(void* model, const char* from, const char* to)
{
  logTrace();
  if(!model)
  {
    logError("oms_addConnection: invalid pointer");
    return;
  }

  oms_model* pModel = (oms_model*) model;
  pModel->addConnection(from, to);
}

void oms_simulate(void* model)
{
  logTrace();
  if(!model)
  {
    logError("oms_simulate: invalid pointer");
    return;
  }

  oms_model* pModel = (oms_model*) model;
  pModel->simulate();
}

void oms_describe(void* model)
{
  logTrace();
  if(!model)
  {
    logError("oms_describe: invalid pointer");
    return;
  }

  oms_model* pModel = (oms_model*) model;
  pModel->describe();
}

void oms_exportDependencyGraph(void* model, const char* filename)
{
  logTrace();
  if(!model)
  {
    logError("oms_describe: invalid pointer");
    return;
  }

  oms_model* pModel = (oms_model*) model;
  pModel->exportDependencyGraph(filename);
}

void oms_setStartTime(double startTime)
{
  logTrace();
  Settings::SetStartTime(startTime);
}

void oms_setStopTime(double stopTime)
{
  logTrace();
  Settings::SetStopTime(stopTime);
}

void oms_setTolerance(double tolerance)
{
  logTrace();
  Settings::SetTolerance(tolerance);
}

void oms_setWorkingDirectory(const char* filename)
{
  logTrace();
  Settings::SetTempDirectory(filename);
}

void oms_setResultFile(const char* filename)
{
  logTrace();
  Settings::SetResultFile(filename);
}

const char* oms_getVersion()
{
  return oms_git_version;
}
