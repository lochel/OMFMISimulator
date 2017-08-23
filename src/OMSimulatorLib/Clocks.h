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

enum Index_t
{
  INITIALIZATION_CLOCK = 0,
  SIMULATION_CLOCK,
  RESULTFILE_CLOCK,
  MAX_CLOCK_INDEX
};

class Clocks
{
public:
  static void tic(Index_t clock);
  static void toc(Index_t clock);
  static void resetAndTic(Index_t clock) {reset(clock); tic(clock);}
  static void reset(Index_t clock);
  static void resetAll();
  static std::string getStats();

private:
  Clock clocks[MAX_CLOCK_INDEX];

private:
  Clocks();
  ~Clocks();

  static Clocks& getInstance();

  // Stop the compiler generating methods of copy the object
  Clocks(Clocks const& copy);            // Not Implemented
  Clocks& operator=(Clocks const& copy); // Not Implemented
};

#endif
