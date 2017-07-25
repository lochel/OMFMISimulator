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

#include "Options.h"

#include <iostream>
#include <stdlib.h>

#include <OMSimulator.h>

using namespace std;

int main(int argc, char *argv[])
{
  ProgramOptions options;
  void* pModel = NULL;

  if (!options.load_flags(argc, argv))
    return 1;

  if (options.help)
    return 0;

  if (options.version)
  {
    cout << oms_getVersion() << endl;
    return 0;
  }

  std::string filename = options.filename;
  std::string type = "";
  if (filename.length() > 4)
    type = filename.substr(filename.length() - 3);
  else
  {
    cout << "Not able to process file '" << filename.c_str() << "'" << endl;
    cout << "Use OMSimulator --help for more information." << endl;
    return 1;
  }

  if (options.tempDir != "")
    oms_setTempDirectory(options.tempDir.c_str());

  if (type == "fmu")
  {
    pModel = oms_newModel();
    oms_instantiateFMU(pModel, filename.c_str(), "fmu");
  }
  else if (type == "xml")
    pModel = oms_loadModel(filename.c_str());
  else
  {
    cout << "Not able to process file '" << filename.c_str() << "'" << endl;
    cout << "Use OMSimulator --help for more information." << endl;
    return 1;
  }

  if (options.resultFile != "")
    oms_setResultFile(pModel, options.resultFile.c_str());
  if (options.useStartTime)
    oms_setStartTime(pModel, options.startTime);
  if (options.useStopTime)
    oms_setStopTime(pModel, options.stopTime);
  if (options.useTolerance)
    oms_setTolerance(pModel, options.tolerance);

  if (options.describe)
  {
    // OMSimulator --describe example.fmu
    oms_describe(pModel);
  }
  else
  {
    // OMSimulator example.fmu
    oms_initialize(pModel);
    oms_simulate(pModel);
    oms_terminate(pModel);
  }

  oms_unload(pModel);
  return 0;
}
