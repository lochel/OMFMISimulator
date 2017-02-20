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

#include <OMSimulator.h>

#include <iostream>
#include <stdlib.h>
using namespace std;

void printUsage()
{
  cout << "Usage: OMSimulator [OPTIONS] filename" << endl;
  cout << "OPTIONS" << endl;
  cout << "  --describe             Displays brief summary of given FMU" << endl;
  cout << "  --help                 Displays the help text" << endl;
  cout << "  --resultFile=<string>  Specifies the name of the output result file" << endl;
  cout << "  --startTime=<double>   Specifies the start time" << endl;
  cout << "  --stopTime=<double>    Specifies the stop time" << endl;
  cout << "  --tempDir=<string>     Specifies the working directory" << endl;
  cout << "  --tolerance=<double>   Specifies the relative tolerance" << endl;
}

int main(int argc, char *argv[])
{
  bool describe = false;
  string filename("");
  string resultFile("");

  for(int i=1; i<argc; i++)
  {
    string arg(argv[i]);
    if(0 == arg.compare("--describe"))
      describe = true;
    else if(0 == arg.compare("--help"))
    {
      printUsage();
      return 0;
    }
    else if(0 == arg.compare(0, 13, "--resultFile="))
    {
      resultFile = arg.substr(13);
    }
    else if(0 == arg.compare(0, 12, "--startTime="))
    {
      double startTime = atof(arg.substr(12).c_str());
      oms_setStartTime(startTime);
    }
    else if(0 == arg.compare(0, 11, "--stopTime="))
    {
      double stopTime = atof(arg.substr(11).c_str());
      oms_setStopTime(stopTime);
    }
    else if(0 == arg.compare(0, 10, "--tempDir="))
    {
      oms_setWorkingDirectory(arg.substr(10).c_str());
    }
    else if(0 == arg.compare(0, 12, "--tolerance="))
    {
      double tolerance = atof(arg.substr(12).c_str());
      oms_setTolerance(tolerance);
    }
    else if(filename.empty())
      filename = arg;
    else
    {
      cout << "Not able to process argument: " << arg << endl;
      cout << "Use OMSimulator --help for more information." << endl;
      return 0;
    }
  }

  if(filename.empty())
  {
    cout << "Use OMSimulator --help for more information." << endl;
    return 0;
  }

  if(describe)
  {
    // OMSimulator --describe example.fmu
    void* pModel = oms_loadModel(filename.c_str());
    oms_describe(pModel);
    oms_unload(pModel);
  }
  else
  {
    // OMSimulator example.fmu
    void* pModel = oms_loadModel(filename.c_str());
    oms_simulate(pModel, resultFile.empty() ? NULL : resultFile.c_str());
    oms_unload(pModel);
  }

  return 0;
}
