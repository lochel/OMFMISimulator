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
#include "oms_logging.h"

#include <stdio.h>
#include <string>

#include <boost/filesystem.hpp>

Settings::Settings()
{
  logInfo("Initializing settings");
  startTime = NULL;
  stopTime = NULL;
  tolerance = NULL;
  tempDir = NULL;
  resultFile = NULL;
}

Settings::~Settings()
{
  logInfo("Free settings");
  ClearStartTime();
  ClearStopTime();
  ClearTolerance();
  ClearTempDirectory();
  if(resultFile) delete[] resultFile;
}

Settings& Settings::getInstance()
{
  // the only instance
  static Settings instance;
  return instance;
}

void Settings::SetStartTime(double startTime)
{
  if(!this->startTime)
    this->startTime = new double;
  *(this->startTime) = startTime;
}

double* Settings::GetStartTime()
{
  return this->startTime;
}

void Settings::ClearStartTime()
{
  if(this->startTime)
    delete this->startTime;
}

void Settings::SetStopTime(double stopTime)
{
  if(!this->stopTime)
    this->stopTime = new double;
  *(this->stopTime) = stopTime;
}

double* Settings::GetStopTime()
{
  return this->stopTime;
}

void Settings::ClearStopTime()
{
  if(this->stopTime)
    delete this->stopTime;
}

void Settings::SetTolerance(double tolerance)
{
  if(!this->tolerance)
    this->tolerance = new double;
  *(this->tolerance) = tolerance;
}

double* Settings::GetTolerance()
{
  return this->tolerance;
}

void Settings::ClearTolerance()
{
  if(this->tolerance)
    delete this->tolerance;
}

void Settings::SetTempDirectory(const char* tempDir)
{
  if (!boost::filesystem::is_directory(tempDir))
  {
    logError("set working directory to \"" + std::string(tempDir) + "\" failed");
    return;
  }

  boost::filesystem::path path(tempDir);
  path = boost::filesystem::canonical(path);

  ClearTempDirectory();
  this->tempDir = new char[path.string().length()+1];
  strcpy(this->tempDir, path.string().c_str());
}

const char* Settings::GetTempDirectory()
{
  return this->tempDir;
}

void Settings::ClearTempDirectory()
{
  if(this->tempDir)
    delete[] this->tempDir;
}

void Settings::SetResultFile(const char* resultFile)
{
  ClearResultFile();
  this->resultFile = new char[strlen(resultFile)+1];
  strcpy(this->resultFile, resultFile);
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
