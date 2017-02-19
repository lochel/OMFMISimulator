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

#include <iostream>
using namespace std;

void* oms_loadModel(const char* filename)
{
  logTrace();
  oms_model* pModel = new oms_model(filename);
  return (void*) pModel;
}

void* oms_loadComposite(const char* filename)
{
  logTrace();
  cout << "Function not implemented yet: loadComposite" << endl;
  return NULL;
}

void oms_simulate(void* model, double* startTime, double* stopTime, double* tolerance, const char* resultFile)
{
  logTrace();
  if(!model)
    return;

  oms_model* pModel = (oms_model*) model;
  pModel->simulate(startTime, stopTime, tolerance, resultFile);
}

void oms_describe(void* model)
{
  logTrace();
  if(!model)
    return;

  oms_model* pModel = (oms_model*) model;
  pModel->describe();
}

void oms_unload(void* model)
{
  logTrace();
  if(!model)
    return;

  oms_model* pModel = (oms_model*) model;
  delete pModel;
}

void oms_setWorkingDirectory(void* model, const char* filename)
{
  logTrace();
  if(!model)
    return;

  oms_model* pModel = (oms_model*) model;
  pModel->setWorkingDirectory(filename);
}
