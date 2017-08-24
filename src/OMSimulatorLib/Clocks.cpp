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
#include "Logging.h"

#include <string>

Clocks::Clocks()
{
  for (int i = 0; i<CLOCK_MAX_INDEX; ++i)
   clocks[i].reset();

  activeClocks.push(CLOCK_IDLE);
  clocks[CLOCK_IDLE].tic();
}

Clocks::~Clocks()
{
  if (!(activeClocks.size() == 1 && activeClocks.top() == CLOCK_IDLE))
    logWarning("Time measurement is corrupted.");
}

Clocks& Clocks::getInstance()
{
  // The only instance
  static Clocks instance;
  return instance;
}

void Clocks::tic(ClockIndex_t index)
{
  Clocks& instance = getInstance();
  ClockIndex_t activeClock = instance.activeClocks.top();

  if (activeClock == index)
    return;

  instance.clocks[activeClock].toc();
  instance.clocks[index].tic();
  instance.activeClocks.push(index);
}

void Clocks::toc(ClockIndex_t index)
{
  Clocks& instance = getInstance();
  ClockIndex_t activeClock = instance.activeClocks.top();

  if (activeClock != index)
    logWarning("Time measurement is corrupted.");

  instance.activeClocks.pop();
  activeClock = instance.activeClocks.top();

  instance.clocks[index].toc();
  instance.clocks[activeClock].tic();
}

std::string Clocks::getStats()
{
  Clocks& instance = getInstance();
  std::string stats = "";

  double cpuTime = 0.0;
  double wallTime = 0.0;
  for (int i = 0; i<CLOCK_MAX_INDEX; ++i)
  {
    cpuTime += instance.clocks[i].getElapsedCPUTime();
    wallTime += instance.clocks[i].getElapsedWallTime();
  }

  stats += "Total: " + toString(cpuTime) + "s [cpu clock] (" + toString(wallTime) + "s [wall clock])";
  stats += "\nInitialization: " + toString(instance.clocks[CLOCK_INITIALIZATION].getElapsedCPUTime()/cpuTime*100.0) + "% - " + toString(instance.clocks[CLOCK_INITIALIZATION].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[CLOCK_INITIALIZATION].getElapsedWallTime()) + "s [wall clock])";
  stats += "\nSimulation: " + toString(instance.clocks[CLOCK_SIMULATION].getElapsedCPUTime()/cpuTime*100.0) + "% - " + toString(instance.clocks[CLOCK_SIMULATION].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[CLOCK_SIMULATION].getElapsedWallTime()) + "s [wall clock])";
  stats += "\nResult file: " + toString(instance.clocks[CLOCK_RESULTFILE].getElapsedCPUTime()/cpuTime*100.0) + "% - " + toString(instance.clocks[CLOCK_RESULTFILE].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[CLOCK_RESULTFILE].getElapsedWallTime()) + "s [wall clock])";
  stats += "\nIdle: " + toString(instance.clocks[CLOCK_IDLE].getElapsedCPUTime()/cpuTime*100.0) + "% - " + toString(instance.clocks[CLOCK_IDLE].getElapsedCPUTime()) + "s [cpu clock] (" + toString(instance.clocks[CLOCK_IDLE].getElapsedWallTime()) + "s [wall clock])";

  return stats;
}
