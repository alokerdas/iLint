/*
 * Copyright (c) 2020-2025 Aloke Kumar Das (possibilities@labandlectures.tech)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "ivl_target.h"
#include "lint.h"

void checkPMETER(map<int, map<string, string> > & table , ivl_parameter_t & param)
{  
  const char *sAct = "active";
  const char *sPat = "pattern";
  int line = ivl_parameter_lineno(param); 
  const char *file = ivl_parameter_file(param); 
  const char* paraname = ivl_parameter_basename(param);
  
  int rule = 1009;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "[_A-Z0-9]*"; 
    if (table[rule].find(sPat) != table[rule].end())
      patt = (char*) table[rule][sPat].c_str();
      
    if (fnmatch(patt, paraname, 0))
      printViolation(rule, line, file, paraname, patt);
  } 
}

void checkPmeterWidth(map<int, map<string, string> > & table, ivl_parameter_t & param)
{
  const char *sAct = "active";
  const char *sNml = "namelength";
  int line = ivl_parameter_lineno(param); 
  const char *file = ivl_parameter_file(param); 
  const char* paraname = ivl_parameter_basename(param);

  int rule = 1010;
  if (table[rule][sAct] == "yes")
  {
    int paramWidth = 22;
    if (table[rule].find(sNml) != table[rule].end())
    {
      paramWidth = atoi(table[rule][sNml].c_str());
    }
    ivl_expr_t expr =  ivl_parameter_expr(param);
    if (ivl_expr_type(expr)==IVL_EX_NUMBER)
    {
      unsigned bitLength = ivl_expr_width(expr); 
      if (bitLength > paramWidth)
      {
        printViolation(rule, line, file, bitLength, paraname);
      }
    }
  }
}

void checkPmeterNameLength(map<int, map<string, string> > & table, ivl_parameter_t & param)
{
  const char *sAct = "active";
  int line = ivl_parameter_lineno(param); 
  const char *file = ivl_parameter_file(param); 
  const char* paraname = ivl_parameter_basename(param);

  int rule = 1043;
  if (table[rule][sAct] == "yes")
  {
    if((strlen(paraname) < 3) || (strlen(paraname) >16))
      printViolation(rule, line, file,paraname);
  }
}

void checkPmeterPrefixSuffix(map<int, map<string, string> > & table, ivl_parameter_t & param)
{
  const char *sAct = "active";
  const char *sPat = "pattern";
  int line = ivl_parameter_lineno(param); 
  const char *file = ivl_parameter_file(param); 
  const char* paraname = ivl_parameter_basename(param);

  int rule = 1042;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "p_"; 
    if (table[rule].find(sPat) != table[rule].end())
      patt = table[rule][sPat].c_str();

    if(strncasecmp(paraname, patt, strlen(patt)))
      printViolation(rule, line, file, paraname, patt);
  }
}

