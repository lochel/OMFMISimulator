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

#ifndef _OMS_CLOCKS_H_
#define _OMS_CLOCKS_H_

#include "Clock.h"

#include <string>
#include <stack>

enum ClockIndex_t
{
  CLOCK_IDLE = 0,
  CLOCK_INITIALIZATION,
  CLOCK_SIMULATION,
  CLOCK_RESULTFILE,
  CLOCK_MAX_INDEX
};

class Clocks
{
public:
  static void tic(ClockIndex_t clock);
  static void toc(ClockIndex_t clock);
  static std::string getStats();

private:
  Clock clocks[CLOCK_MAX_INDEX];
  std::stack<ClockIndex_t> activeClocks;

private:
  Clocks();
  ~Clocks();

  static Clocks& getInstance();

  // Stop the compiler generating methods of copy the object
  Clocks(Clocks const& copy);            // Not Implemented
  Clocks& operator=(Clocks const& copy); // Not Implemented
};

#endif
