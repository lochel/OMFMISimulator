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

#ifndef _OMS_MATVER4_H_
#define _OMS_MATVER4_H_

#include <stdio.h>

enum MatVer4Type_t
{
  MatVer4Type_DOUBLE = 0,
  MatVer4Type_INT32 = 20,
  MatVer4Type_CHAR = 51
};

struct MatVer4Header
{
  unsigned int type;
  unsigned int mrows;
  unsigned int ncols;
  unsigned int imagf;
  unsigned int namelen;
};

struct MatVer4Matrix
{
  MatVer4Header header;
  void *data;
};

int writeMatVer4Matrix(FILE* file, const char* name, size_t rows, size_t cols, const void* matrixData, MatVer4Type_t type);
int appendMatVer4Matrix(FILE* file, long position, const char* name, size_t rows, size_t cols, const void* matrixData, MatVer4Type_t type);

MatVer4Matrix* readMatVer4Matrix(FILE* file);
int skipMatVer4Matrix(FILE* file);

void deleteMatVer4Matrix(MatVer4Matrix** matrix);

#endif
