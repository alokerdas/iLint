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

#include "general.h"
#include <fstream>

void readConfiguration(map<int, map<string, string> > & confTable)
{
  const char *fileName = "lint.res";
  const int buffLength = 256;
  const int maxRules = 1400;
  const char *sAct = "active";
  char *oneLine = (char*) malloc(buffLength * sizeof (char));

  ifstream confile(fileName);
  if (confile.is_open())
  {
    int ruleNo;
    while (!confile.eof())
    {
      const char *sRul = "rule";
      confile.getline(oneLine, buffLength);

      if (!strcasecmp(oneLine, "all"))
      {
        for (int i = 1001; i < maxRules ; i++)
        {
          confTable[i][sAct] = "yes";
        }
        continue;
      }

      char *tok = strtok(oneLine, "=");
      if (tok)
      {
        if (strcasecmp(tok, sRul)) // ulta logic
          confTable[ruleNo][tok]=strtok(NULL, "=");  
        else
          ruleNo = atoi(strtok(NULL, "="));
      }
    }
  }
  else
  {
    for (int i = 1001; i < maxRules ; i++)
    {
      confTable[i][sAct] = "yes";
    }
  }
  if (oneLine)
    free(oneLine);
}
