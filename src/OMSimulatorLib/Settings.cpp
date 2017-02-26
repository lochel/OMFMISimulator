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
  logDebug("Initializing settings");
  startTime = NULL;
  stopTime = NULL;
  tolerance = NULL;
  tempDir = NULL;
  resultFile = NULL;
}

Settings::~Settings()
{
  logDebug("Free settings");
  ClearStartTime();
  ClearStopTime();
  ClearTolerance();
  ClearTempDirectory();
  ClearResultFile();
}

Settings& Settings::getInstance()
{
  // the only instance
  static Settings instance;
  return instance;
}

void Settings::SetStartTime(double startTime)
{
  Settings& settings = getInstance();
  if(!settings.startTime)
    settings.startTime = new double;
  *(settings.startTime) = startTime;
}

double* Settings::GetStartTime()
{
  Settings& settings = getInstance();
  return settings.startTime;
}

void Settings::ClearStartTime()
{
  Settings& settings = getInstance();
  if(settings.startTime)
    delete settings.startTime;
}

void Settings::SetStopTime(double stopTime)
{
  Settings& settings = getInstance();
  if(!settings.stopTime)
    settings.stopTime = new double;
  *(settings.stopTime) = stopTime;
}

double* Settings::GetStopTime()
{
  Settings& settings = getInstance();
  return settings.stopTime;
}

void Settings::ClearStopTime()
{
  Settings& settings = getInstance();
  if(settings.stopTime)
    delete settings.stopTime;
}

void Settings::SetTolerance(double tolerance)
{
  Settings& settings = getInstance();
  if(!settings.tolerance)
    settings.tolerance = new double;
  *(settings.tolerance) = tolerance;
}

double* Settings::GetTolerance()
{
  Settings& settings = getInstance();
  return settings.tolerance;
}

void Settings::ClearTolerance()
{
  Settings& settings = getInstance();
  if(settings.tolerance)
    delete settings.tolerance;
}

void Settings::SetTempDirectory(const char* tempDir)
{
  if (!boost::filesystem::is_directory(tempDir))
  {
    logError("set working directory to \"" + std::string(tempDir) + "\" failed");
    return;
  }

  Settings& settings = getInstance();
  boost::filesystem::path path(tempDir);
  path = boost::filesystem::canonical(path);

  ClearTempDirectory();
  settings.tempDir = new char[path.string().length()+1];
  strcpy(settings.tempDir, path.string().c_str());
}

const char* Settings::GetTempDirectory()
{
  Settings& settings = getInstance();
  return settings.tempDir;
}

void Settings::ClearTempDirectory()
{
  Settings& settings = getInstance();
  if(settings.tempDir)
    delete[] settings.tempDir;
}

void Settings::SetResultFile(const char* resultFile)
{
  Settings& settings = getInstance();
  ClearResultFile();
  settings.resultFile = new char[strlen(resultFile)+1];
  strcpy(settings.resultFile, resultFile);
}

const char* Settings::GetResultFile()
{
  Settings& settings = getInstance();
  return settings.resultFile;
}

void Settings::ClearResultFile()
{
  Settings& settings = getInstance();
  if(settings.resultFile)
    delete[] settings.resultFile;
}
