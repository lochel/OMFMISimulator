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

#include "Settings.h"
#include "Logging.h"

#include <cstring>
#include <string>

Settings::Settings()
{
  logDebug("Initializing settings");
  startTime = NULL;
  stopTime = NULL;
  tolerance = NULL;
  resultFile = NULL;
}

Settings::~Settings()
{
  logDebug("Free settings");
  ClearStartTime();
  ClearStopTime();
  ClearTolerance();
  ClearResultFile();
}

void Settings::SetStartTime(double startTime)
{
  if(!this->startTime)
    this->startTime = new double;
  *(this->startTime) = startTime;
}

double* Settings::GetStartTime()
{
  return startTime;
}

void Settings::ClearStartTime()
{
  if(startTime)
    delete startTime;
}

void Settings::SetStopTime(double stopTime)
{
  if(!this->stopTime)
    this->stopTime = new double;
  *(this->stopTime) = stopTime;
}

double* Settings::GetStopTime()
{
  return stopTime;
}

void Settings::ClearStopTime()
{
  if(stopTime)
    delete stopTime;
}

void Settings::SetTolerance(double tolerance)
{
  if(!this->tolerance)
    this->tolerance = new double;
  *(this->tolerance) = tolerance;
}

double* Settings::GetTolerance()
{
  return tolerance;
}

void Settings::ClearTolerance()
{
  if(tolerance)
    delete tolerance;
}

void Settings::SetResultFile(const char* resultFile)
{
  ClearResultFile();
  this->resultFile = new char[std::strlen(resultFile)+1];
  std::strcpy(this->resultFile, resultFile);
}

const char* Settings::GetResultFile()
{
  return resultFile;
}

void Settings::ClearResultFile()
{
  if(resultFile)
    delete[] resultFile;
}
