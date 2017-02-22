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

#include "oms_logging.h"
#include "oms_version.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <ctime>

using namespace std;

std::string TimeStr()
{
  time_t rawtime;
  struct tm* timeinfo;
  char buffer[64];

  time (&rawtime);
  timeinfo = localtime(&rawtime);
  strftime(buffer, 64, "%a %b %d %T %Y", timeinfo);
  return std::string(buffer);
}

Log::Log()
{
  numWarnings = 0;
  numErrors = 0;
  logFile.open("omsllog.txt");
  Info("Initializing logging (" + std::string(oms_git_version) + ")");
}

Log::~Log()
{
  Info("Logging completed properly");
  logFile.close();
  cout << "Logging information has been saved to \"omsllog.txt\"" << endl;
  if(numErrors > 0 || numWarnings > 0)
  {
    cout << "  " << numWarnings << " warnings" << endl;
    cout << "  " << numErrors << " errors" << endl;
  }
}

Log& Log::getInstance()
{
  // The only instance
  static Log instance;
  return instance;
}

void Log::Info(const std::string msg)
{
  logFile << TimeStr() << " | info:    " << msg << endl;
}

void Log::Warning(const std::string msg)
{
  numWarnings++;
  logFile << TimeStr() << " | warning: " << msg << endl;
  cerr << "warning: " << msg << endl;
}

void Log::Error(const std::string msg)
{
  numErrors++;
  logFile << TimeStr() << " | error:   " << msg << endl;
  cerr << "error:   " << msg << endl;
}

void Log::Fatal(const std::string msg)
{
  numErrors++;
  logFile << TimeStr() << " | fatal:   " << msg << endl;
  cerr << "fatal:   " << msg << endl;
  exit(1);
}

void Log::Trace(const std::string function, const std::string file, const long line)
{
  logFile << TimeStr() << " | trace:   " << function << " (" << file << ":" << line << ")" << endl;
}
