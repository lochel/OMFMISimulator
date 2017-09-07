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

#include "MatVer4Writer.h"

#include <climits>
#include <stdexcept>
#include <stdint.h>
#include <string.h>

const bool isBigEndian()
{
  union
  {
    uint32_t i32;
    uint8_t i8[4];
  } test = { 0x01020304 };
  return (1 == test.i8[0]);
}

unsigned int size2uint(size_t value)
{
  if (value > UINT_MAX)
    throw std::out_of_range("Converting 'size_t' to 'unsigned int' failed.");
  return static_cast<unsigned int>(value);
}

int writeMatVer4Matrix(FILE* file, const char* name, size_t rows, size_t cols, const void* matrixData, MatVer4Type_t type)
{
  struct MatVer4Header
  {
    unsigned int type;
    unsigned int mrows;
    unsigned int ncols;
    unsigned int imagf;
    unsigned int namelen;
  } header;

  size_t size;
  switch (type)
  {
  case MatVer4Type_DOUBLE:
    size = sizeof(double);
    break;
  case MatVer4Type_INT32:
    size = sizeof(int32_t);
    break;
  case MatVer4Type_CHAR:
    size = sizeof(uint8_t);
    break;
  default:
    return -1;
  }

  header.type = (isBigEndian() ? 1000 : 0) + type;
  header.mrows = size2uint(rows);
  header.ncols = size2uint(cols);
  header.imagf = 0;
  header.namelen = (unsigned int) strlen(name) + 1;

  fwrite(&header, sizeof(MatVer4Header), 1, file);
  fwrite(name, sizeof(char), header.namelen, file);
  fwrite(matrixData, size, rows * cols, file);

  return 0;
}

int appendMatVer4Matrix(FILE* file, long position, const char* name, size_t rows, size_t cols, const void* matrixData, MatVer4Type_t type)
{
  struct MatVer4Header
  {
    unsigned int type;
    unsigned int mrows;
    unsigned int ncols;
    unsigned int imagf;
    unsigned int namelen;
  } header;

  size_t size;
  switch (type)
  {
  case MatVer4Type_DOUBLE:
    size = sizeof(double);
    break;
  case MatVer4Type_INT32:
    size = sizeof(int32_t);
    break;
  case MatVer4Type_CHAR:
    size = sizeof(uint8_t);
    break;
  default:
    return -1;
  }

  long eof = ftell(file);
  fseek(file, position, SEEK_SET);
  fread(&header, sizeof(MatVer4Header), 1, file);
  if (header.type != (isBigEndian() ? 1000 : 0) + type)
    return -1;
  if (header.mrows != rows)
    return -1;
  header.ncols += size2uint(cols);
  if (header.imagf != 0)
    return -1;
  if (header.namelen != strlen(name) + 1)
    return -1;

  fseek(file, position, SEEK_SET);
  fwrite(&header, sizeof(MatVer4Header), 1, file);
  fseek(file, eof, SEEK_SET);
  fwrite(matrixData, size, rows * cols, file);
  return 0;
}
