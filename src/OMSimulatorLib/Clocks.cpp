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

#include "Clocks.h"
#include "Clock.h"
#include "Util.h"

#include <string>

Clocks::Clocks()
{
  for (int i = 0; i<MAX_CLOCK_INDEX; ++i)
    clocks[i].reset();
}

Clocks::~Clocks()
{
}

Clocks& Clocks::getInstance()
{
  // The only instance
  static Clocks instance;
  return instance;
}

void Clocks::tic(Index_t index)
{
  Clocks& instance = getInstance();
  instance.clocks[index].tic();
}

void Clocks::toc(Index_t index)
{
  Clocks& instance = getInstance();
  instance.clocks[index].toc();
}

void Clocks::reset(Index_t index)
{
  Clocks& instance = getInstance();
  instance.clocks[index].reset();
}

void Clocks::resetAll()
{
  Clocks& instance = getInstance();
  for (int i=0; i<MAX_CLOCK_INDEX; ++i)
    instance.clocks[i].reset();
}

std::string Clocks::getStats()
{
  Clocks& instance = getInstance();
  std::string stats = "";

  stats += "Initialization: " + toString(instance.clocks[INITIALIZATION_CLOCK].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[INITIALIZATION_CLOCK].getElapsedWallTime()) + "s [wall clock])";
  stats += "\nSimulation: " + toString(instance.clocks[SIMULATION_CLOCK].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[SIMULATION_CLOCK].getElapsedWallTime()) + "s [wall clock])";
  stats += "\nResult file: " + toString(instance.clocks[RESULTFILE_CLOCK].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[RESULTFILE_CLOCK].getElapsedWallTime()) + "s [wall clock])";

  return stats;
}
