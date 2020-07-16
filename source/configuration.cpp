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
