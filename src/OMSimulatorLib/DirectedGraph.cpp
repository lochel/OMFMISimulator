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

#include "DirectedGraph.h"
#include "Variable.h"
#include "Logging.h"
#include "oms_resultfile.h"

#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <sstream>
#include <stdlib.h>

DirectedGraph::DirectedGraph()
{

}

DirectedGraph::~DirectedGraph()
{

}

int DirectedGraph::addVariable(const Variable& var)
{
  nodes.push_back(var);
  return nodes.size()-1;
}

void DirectedGraph::addEdge(const Variable& var1, const Variable& var2)
{
  int index1 = -1;
  int index2 = -1;

  for(int i=0; i<nodes.size(); ++i)
  {
    if(var1 == nodes[i])
      index1 = i;

    if(var2 == nodes[i])
      index2 = i;

    if(index1 != -1 && index2 != -1)
      break;
  }

  if(-1 == index1)
    index1 = addVariable(var1);
  if(-1 == index2)
    index2 = addVariable(var2);

  edges.push_back(std::pair<int, int>(index1, index2));
}

void DirectedGraph::dotExport(const std::string& filename)
{
  /*
   * digraph graphname
   * {
   *   0[label="instance.name"];
   *   1[label="instance.name"];
   *   2[label="instance.name"];
   *
   *   0 -> 1;
   *   0 -> 2;
   * }
   */
  std::ofstream dotFile(filename.c_str());
  dotFile << "digraph G" << std::endl;
  dotFile << "{" << std::endl;
  for(int i=0; i<nodes.size(); i++)
    dotFile << "  " << i << "[label=\"" << nodes[i].fmuInstance << "." << nodes[i].name << "\", shape=box];" << std::endl;
  dotFile << std::endl;

  for(int i=0; i<edges.size(); i++)
    dotFile << "  " << edges[i].first << " -> " << edges[i].second << ";" << std::endl;
  dotFile << "}" << std::endl;
  dotFile.close();
}

void DirectedGraph::includeGraph(const DirectedGraph& graph)
{
  for(int i=0; i<graph.nodes.size(); i++)
    addVariable(graph.nodes[i]);

  for(int i=0; i<graph.edges.size(); i++)
    addEdge(graph.nodes[graph.edges[i].first], graph.nodes[graph.edges[i].second]);
}
