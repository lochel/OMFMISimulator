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

#include "oms_resultfile.h"
#include "Logging.h"
#include "Util.h"

#include <fmilib.h>

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>

oms_resultfile::oms_resultfile(std::string filename, fmi2_import_t* fmu)
{
  this->fmu = fmu;
  resultFile.open(filename.c_str());
  logDebug("Result file: " + filename);

  resultFile << "time";

  fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu, 0);
  size_t nVar = fmi2_import_get_variable_list_size(list);
  for(size_t i=0; i<nVar; ++i)
  {
    fmi2_import_variable_t* var = fmi2_import_get_variable(list, i);
    std::string name = fmi2_import_get_variable_name(var);
    resultFile << ", " << name;
  }

  resultFile << std::endl;
}

oms_resultfile::~oms_resultfile()
{
  resultFile.close();
  logDebug("Result file closed");
}

void oms_resultfile::emit(double time)
{
  resultFile << time;

  fmi2_import_variable_list_t *list = fmi2_import_get_variable_list(fmu, 0);
  size_t nVar = fmi2_import_get_variable_list_size(list);
  for(size_t i=0; i<nVar; ++i)
  {
    fmi2_import_variable_t* var = fmi2_import_get_variable(list, i);
    std::string value= "???";

    if(fmi2_base_type_real == fmi2_import_get_variable_base_type(var))
    {
      double real_value;
      fmi2_value_reference_t vr = fmi2_import_get_variable_vr(var);
      fmi2_import_get_real(fmu, &vr, 1, &real_value);
      value = toString(real_value);
    }
    else if(fmi2_base_type_int == fmi2_import_get_variable_base_type(var))
    {
      int int_value;
      fmi2_value_reference_t vr = fmi2_import_get_variable_vr(var);
      fmi2_import_get_integer(fmu, &vr, 1, &int_value);
      value = toString(int_value);
    }
    else if(fmi2_base_type_bool == fmi2_import_get_variable_base_type(var))
    {
      int bool_value;
      fmi2_value_reference_t vr = fmi2_import_get_variable_vr(var);
      fmi2_import_get_boolean(fmu, &vr, 1, &bool_value);
      value = bool_value ? "1" : "0";
    }
    // TODO: string
    else
      logWarning("oms_resultfile::emit: unsupported base type");

    resultFile << ", " << value;
  }

  resultFile << std::endl;
}
