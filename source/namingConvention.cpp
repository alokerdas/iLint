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

void checkLatchNamePrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  unsigned line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if(ivl_lpm_type(lpm)==IVL_LPM_LATCH)
  {
    ivl_nexus_t outNex  = ivl_lpm_q(lpm);
    ivl_nexus_ptr_t aConn = ivl_nexus_ptr(outNex, 0);
    ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
    if(aConnSig)
    {
      const char *latName = ivl_signal_basename(aConnSig);
      rule = 1189;
      if (table[rule][sAct] == "yes")
      {
        const char *patt = "*_lat"; 
        if(fnmatch(patt, latName, 0))
        {
          printViolation(rule, line, file, latName);
        }
      }
      rule = 1192;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, latName);
      }
    }
  }
}

void checkGatePrefixSuffix(map<int, map<string, string> > & table, ivl_net_logic_t & gate)
{  
  int rule = 1175;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {   
    const char *gateName = ivl_logic_basename(gate);
    const char *patt = "gate_*"; 
    if(fnmatch(patt, gateName, 0))
    {
      int line = ivl_logic_lineno(gate);
      const char *file = ivl_logic_file(gate);
      printViolation(rule, line, file, gateName);
    }
  } 
  rule = 1020;
  if (table[rule][sAct] == "yes")
  {
    ivl_udp_t aUDP = (ivl_logic_type(gate) == IVL_LO_UDP) ? ivl_logic_udp(gate) : NULL;
    const char *udpName = aUDP ? ivl_udp_name(aUDP) : NULL;
    const char *patt = "([a-z])*"; 
    if(udpName && fnmatch(patt, udpName, 0))
    {
      int line = ivl_udp_lineno(aUDP);
      const char *file = ivl_udp_file(aUDP);
      printViolation(rule, line, file, udpName);
    }
  } 
}

void checkActiveSignalName(map<int, map<string, string> > & table, ivl_event_t & evt)
{
  int rule = 1166; // same as 1197, not implemented
  const char *sAct = "active";
  int line = ivl_event_lineno(evt);
  const char *file = ivl_event_file(evt);
  if (table[rule][sAct] == "yes")
  {
    for(unsigned i = 0; i < ivl_event_nneg(evt); i++)
    {
      ivl_nexus_t evtNex  = ivl_event_neg(evt, i);
      for(unsigned j = 0; j < ivl_nexus_ptrs(evtNex); j++)
      {       
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(evtNex, j);
        ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
        if(aConnSig)
        {
          const char *nameNedge = ivl_signal_basename(aConnSig);
          const char *patt = "*_n"; 
          if(fnmatch(patt, nameNedge, 0))
          {
            int line = ivl_signal_lineno(aConnSig);
            const char *file = ivl_signal_file(aConnSig);
            printViolation(rule, line, file, nameNedge);
          }
        }
      }
    }
  }
  rule = 1167; // same as 1197, not implemented
  if (table[rule][sAct] == "yes")
  {
    for(unsigned i = 0; i < ivl_event_npos(evt); i++)
    {
      ivl_nexus_t evtNex  = ivl_event_pos(evt, i);
      for(unsigned j = 0; j < ivl_nexus_ptrs(evtNex); j++)
      {       
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(evtNex, j);
        ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
        if(aConnSig)
        {
          const char *namePedge = ivl_signal_basename(aConnSig);
          const char *patt = "*_p"; 
          if(fnmatch(patt, namePedge, 0))
          {
            int line = ivl_signal_lineno(aConnSig);
            const char *file = ivl_signal_file(aConnSig);
            printViolation(rule, line, file, namePedge);
          }
        }
      }
    }
  }
}

void checkSetPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net)
{
  int rule = 0;
  const char *sAct = "active";
  if (ivl_lpm_type(net) == IVL_LPM_FF)
  {
    const char *stSigName = NULL;
    ivl_nexus_t stNex = ivl_lpm_async_set(net);
    if (!stNex)
      stNex = ivl_lpm_sync_set(net);

    unsigned connect = stNex ? ivl_nexus_ptrs(stNex) : 0;
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t stCon = ivl_nexus_ptr(stNex, i);
      ivl_signal_t stSig = ivl_nexus_ptr_sig(stCon);
      if(stSig)
      {
        stSigName = ivl_signal_basename(stSig);
        int line = ivl_signal_lineno(stSig);
        const char *file = ivl_signal_file(stSig);
        rule = 1153;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "set_*"; 
          if(fnmatch(patt, stSigName, 0))
          {
            printViolation(rule, line, file, stSigName);
          }
        }
        rule = 1195;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "*_a"; 
          if(fnmatch(patt, stSigName, 0))
          {
            printViolation(rule, line, file, stSigName);
          }
        }
      }
    }
  }
}

void checkResetPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net)
{
  int rule = 0;
  const char *sAct = "active";
  if (ivl_lpm_type(net) == IVL_LPM_FF)
  {
    const char *rstSigName = NULL;
    ivl_nexus_t rstNex = ivl_lpm_async_clr(net);
    if (!rstNex)
      rstNex = ivl_lpm_sync_clr(net);

    unsigned connect = rstNex ? ivl_nexus_ptrs(rstNex) : 0;
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t rstCon = ivl_nexus_ptr(rstNex, i);
      ivl_signal_t rstSig = ivl_nexus_ptr_sig(rstCon);
      if(rstSig)
      {
        rstSigName = ivl_signal_basename(rstSig);
        int line = ivl_signal_lineno(rstSig);
        const char *file = ivl_signal_file(rstSig);
        rule = 1152;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "rst_*"; 
          if(fnmatch(patt, rstSigName, 0))
          {
            printViolation(rule, line, file, rstSigName);
          }
        }
        rule = 1195;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "*_a"; 
          if(fnmatch(patt, rstSigName, 0))
          {
            printViolation(rule, line, file, rstSigName);
          }
        }
      }
    }
  }
}

void checkRegPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    ivl_nexus_t qNex = ivl_lpm_q(lpm);
    unsigned connect = ivl_nexus_ptrs(qNex);
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t qCon = ivl_nexus_ptr(qNex, i);
      ivl_signal_t qSig = ivl_nexus_ptr_sig(qCon);
      if(qSig)
      {
        int line = ivl_signal_lineno(qSig);
        const char *file = ivl_signal_file(qSig);
        const char *qSigName = ivl_signal_basename(qSig);
        rule = 1147;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "*_cs"; 
          if(fnmatch(patt, qSigName, 0))
          {
            printViolation(rule, line, file, qSigName);
          }
        }
        rule = 1168;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "*_r"; 
          if(fnmatch(patt, qSigName, 0))
          {
            printViolation(rule, line, file, qSigName);
          }
        }
      }
    }
    ivl_nexus_t dataNex = ivl_lpm_data(lpm, 0);
    connect = ivl_nexus_ptrs(dataNex);
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t dataCon = ivl_nexus_ptr(dataNex, i);
      ivl_signal_t dataSig = ivl_nexus_ptr_sig(dataCon);
      if(dataSig)
      {
        int line = ivl_signal_lineno(dataSig);
        const char *file = ivl_signal_file(dataSig);
        const char *dataSigName = ivl_signal_basename(dataSig);
        rule = 1169;
        if (table[rule][sAct] == "yes")
        {
          const char *patt = "*_nxt"; 
          if(fnmatch(patt, dataSigName, 0))
          {
            printViolation(rule, line, file, dataSigName);
          }
        }
      }
    }
  }
}

void checkWire(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1144; // similar to 1187 but nut same. Not possible to implement
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  if (table[rule][sAct] == "yes")
  {
    if ((ivl_signal_type(mySig) == IVL_SIT_TRI) &&
        (ivl_signal_port(mySig) == IVL_SIP_NONE))
    {
      printViolation(rule, line, file, ivl_signal_basename(mySig));
    } 
  } 
}

void checkRegister(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1143;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  if (table[rule][sAct] == "yes")
  {
    if(ivl_signal_type(mySig) == IVL_SIT_REG)
      printViolation(rule, line, file, ivl_signal_basename(mySig));
  }
}

void checkModuleName(map<int, map<string, string> > & table, ivl_scope_t & scope)
{
  int rule = 1136; // same as 1171, not implemented
  const char *sAct = "active";
  int line = ivl_scope_lineno(scope);
  const char *file = ivl_scope_file(scope);
  const char *moduleName = ivl_scope_basename(scope);
  if (table[rule][sAct] == "yes")
  {
    int nml1 = 3;
    const char *sNml1 = "namelengthMin";
    if (table[rule].find(sNml1) != table[rule].end())
    {
      nml1 = stoi(table[rule][sNml1]);
    }
    int nml2 = 16;
    const char *sNml2 = "namelengthMax";
    if (table[rule].find(sNml2) != table[rule].end())
    {
      nml2 = stoi(table[rule][sNml2]);
    }
    if ((strlen(moduleName) < nml1) || (strlen(moduleName) > nml2))    
    {
      printViolation(rule, line, file, moduleName);
    }
  }
  rule = 1137; // same as 1173, not implemented
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "([a-z])*"; 
    if(fnmatch(patt, moduleName, 0))
    {
      printViolation(rule, line, file, moduleName); 
    }
  }
  rule = 1138;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "m_*"; 
    const char *sPat = "pattern";
    if (table[rule].find(sPat) != table[rule].end())
    {
      patt = (char*) table[rule][sPat].c_str();
    }
    if(fnmatch(patt, moduleName, 0))
    {
      printViolation(rule, line, file, moduleName, patt); 
    }
  }
  rule = 1172;
  if (table[rule][sAct] == "yes")
  {
    if (ivl_scope_parent(scope))
    {
      const char *instName = ivl_scope_tname(scope);
      if (moduleName && instName && !strcasecmp(instName, moduleName))
      {
        printViolation(rule, line, file, moduleName); 
      }
    }
  }
  rule = 1196;
  if (table[rule][sAct] == "yes")
  {
    if (ivl_scope_parent(scope))
    {
      const char *instName = ivl_scope_tname(scope);
      const char *patt = "u_*"; 
      if (instName && fnmatch(patt, instName, 0))
      {
        printViolation(rule, line, file, moduleName); 
      }
    }
  }
  rule = 1238;
  if (table[rule][sAct] == "yes")
  {
    if (file && moduleName && strcasecmp(file, moduleName))
    {
      printViolation(rule, line, file, file, moduleName); 
    }
  }
  rule = 1239;
  if (table[rule][sAct] == "yes")
  {
    int nml = 10;
    const char *sNml = "namelength";
    if (table[rule].find(sNml) != table[rule].end())
    {
      nml = stoi(table[rule][sNml]);
    }
    if (strlen(file) > nml)
    {
      printViolation(rule, line, file, strlen(file), nml);
    }
  }
  rule = 1123;
  if (table[rule][sAct] == "yes")
  {
    char *fileExt;
    char *fileName = strdup(file);
    char *tok = strtok(fileName, ".");
    while (tok)
    {
      fileExt = tok;
      tok = strtok(NULL, ".");
    }
    if (strcasecmp(fileExt, "v") &&
        strcasecmp(fileExt, "vlog") &&
        strcasecmp(fileExt, "vhd") &&
        strcasecmp(fileExt, "vhdl"))
    {
      printViolation(rule, 0, file, fileExt);
    }
  }
}

void checkTaskName(map<int, map<string, string> > & table, ivl_scope_t & scope)
{
  int rule = 1133;
  const char *sAct = "active";
  int line = ivl_scope_lineno(scope);
  const char *file = ivl_scope_file(scope);
  const char *taskName = ivl_scope_basename(scope);
  if (table[rule][sAct] == "yes")
  {
    int nml1 = 3;
    const char *sNml1 = "namelengthMin";
    if (table[rule].find(sNml1) != table[rule].end())
    {
      nml1 = stoi(table[rule][sNml1]);
    }
    int nml2 = 16;
    const char *sNml2 = "namelengthMax";
    if (table[rule].find(sNml2) != table[rule].end())
    {
      nml2 = stoi(table[rule][sNml2]);
    }
    if ((strlen(taskName) < nml1) || (strlen(taskName) > nml2))    
    {
      printViolation(rule, line, file, taskName);
    }
  }
  rule = 1134;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "([a-z])*"; 
    const char *sPat = "pattern";
    if (table[rule].find(sPat) != table[rule].end())
    {
      patt = (char*) table[rule][sPat].c_str();
    }
    if(fnmatch(patt, taskName, 0))
    {
      printViolation(rule, line, file, taskName); 
    }
  }
  rule = 1135;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "p_*"; 
    if(fnmatch(patt, taskName, 0))
    {
      printViolation(rule, line, file, taskName); 
    }
  }
  rule = 1181;
  if (table[rule][sAct] == "yes")
  {
    printViolation(rule, line, file, taskName); 
  }
}

void checkFunctionName(map<int, map<string, string> > & table, ivl_scope_t & scope)
{
  int rule = 1130;
  const char *sAct = "active";
  int line = ivl_scope_lineno(scope);
  const char *file = ivl_scope_file(scope);
  const char *funcName = ivl_scope_basename(scope);
  if (table[rule][sAct] == "yes")
  {
    int nml1 = 3;
    const char *sNml1 = "namelengthMin";
    if (table[rule].find(sNml1) != table[rule].end())
    {
      nml1 = stoi(table[rule][sNml1]);
    }
    int nml2 = 16;
    const char *sNml2 = "namelengthMax";
    if (table[rule].find(sNml2) != table[rule].end())
    {
      nml2 = stoi(table[rule][sNml2]);
    }
    if ((strlen(funcName) < nml1) || (strlen(funcName) > nml2))    
    {
      printViolation(rule, line, file, funcName);
    }
  }
  rule = 1131;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "([a-z])*"; 
    const char *sPat = "pattern";
    if (table[rule].find(sPat) != table[rule].end())
    {
      patt = (char*) table[rule][sPat].c_str();
    }
    if(fnmatch(patt, funcName, 0))
    {
      printViolation(rule, line, file, funcName); 
    }
  }
  rule = 1132;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "f_*"; 
    if(fnmatch(patt, funcName, 0))
    {
      printViolation(rule, line, file, funcName); 
    }
  }
}

void checkIntegerFunction(map<int, map<string, string> > & table, ivl_signal_t & port)
{
  int rule = 0;
  const char *sAct = "active";
  ivl_scope_t portScope = ivl_signal_scope(port);
  if (ivl_signal_integer(port))
  {
    rule = 1129;
    if (table[rule][sAct] == "yes")
    {
      int line = ivl_scope_lineno(portScope);
      const char *file = ivl_scope_file(portScope);
      const char *funcName = ivl_scope_basename(portScope);
      printViolation(rule, line, file, funcName);   
    }
  }
}

void checkVariableName(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  const char *sigName = ivl_signal_basename(mySig);
  if(ivl_signal_integer(mySig))
  {
    rule = 1106;
    if (table[rule][sAct] == "yes")
    {
      const char *patt = "([a-z])*"; 
      const char *sPat = "pattern";
      if (table[rule].find(sPat) != table[rule].end())
      {
        patt = (char*) table[rule][sPat].c_str();
      }
      if(fnmatch(patt, sigName, 0))
      {
        printViolation(rule, line, file, sigName); 
      }
    }
    rule = 1107;
    if (table[rule][sAct] == "yes")
    {
      int nml = 10;
      const char *sNml = "namelength";
      if (table[rule].find(sNml) != table[rule].end())
      {
        nml = stoi(table[rule][sNml]);
      }
      if(strlen(ivl_signal_basename(mySig)) > nml)
      {
        printViolation(rule, line, file, sigName);
      }
    }
    rule = 1108;
    if (table[rule][sAct] == "yes")
    {
      const char *patt = "v_*";
      if(fnmatch(patt, sigName, 0))
      {
        printViolation(rule, line, file, sigName); 
      }
    }
    rule = 1198;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file, sigName); 
    }
  }
}

void MissingProcessLabelName(map<int, map<string, string> > & table,ivl_statement_t net)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  ivl_scope_t sscope = ivl_stmt_block_scope(net);
  if(!sscope)
  {
    rule = 1087;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }
  else
  {
    const char *blockName = ivl_scope_basename(sscope);
    rule = 1182;
    if (table[rule][sAct] == "yes")
    {
      const char *patt = "*_PROC";
      if(fnmatch(patt, blockName, 0))
      {
        printViolation(rule, line, file, blockName); 
      }
    }
    rule = 1183;
    if (table[rule][sAct] == "yes")
    {
      int nml1 = 3;
      const char *sNml1 = "namelengthMin";
      if (table[rule].find(sNml1) != table[rule].end())
      {
        nml1 = stoi(table[rule][sNml1]);
      }
      int nml2 = 16;
      const char *sNml2 = "namelengthMax";
      if (table[rule].find(sNml2) != table[rule].end())
      {
        nml2 = stoi(table[rule][sNml2]);
      }
      if ((strlen(blockName) < nml1) || (strlen(blockName) > nml2))    
      {
        printViolation(rule, line, file, blockName);
      }
    }
    rule = 1184;
    if (table[rule][sAct] == "yes")
    {
      const char *patt = "([a-z])*"; 
      const char *sPat = "pattern";
      if (table[rule].find(sPat) != table[rule].end())
      {
        patt = (char*) table[rule][sPat].c_str();
      }
      if(fnmatch(patt, blockName, 0))
      {
        printViolation(rule, line, file, blockName); 
      }
    }
  }
}

void checkVhdlReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1034;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);

  if (table[rule][sAct] == "yes")
  { 
    const char *sigName = ivl_signal_basename(mySig);
    if(!strcasecmp(sigName,"abs") || !strcasecmp(sigName,"access") ||
       !strcasecmp(sigName,"alias") || !strcasecmp(sigName,"after") ||
       !strcasecmp(sigName,"all") || !strcasecmp(sigName,"and") ||
       !strcasecmp(sigName,"architecture") || !strcasecmp(sigName,"array") ||
       !strcasecmp(sigName,"assert") || !strcasecmp(sigName,"attribute") ||
       !strcasecmp(sigName,"begin") || !strcasecmp(sigName,"block") ||
       !strcasecmp(sigName,"body") || !strcasecmp(sigName,"buffer") ||
       !strcasecmp(sigName,"configuration") || !strcasecmp(sigName,"case") ||
       !strcasecmp(sigName,"bus") || !strcasecmp(sigName,"configuration") ||
       !strcasecmp(sigName,"constant") || !strcasecmp(sigName,"disconnect") ||
       !strcasecmp(sigName,"downto") || !strcasecmp(sigName,"else") ||
       !strcasecmp(sigName,"elseif") || !strcasecmp(sigName,"end") ||
       !strcasecmp(sigName,"entity") || !strcasecmp(sigName,"exit") ||
       !strcasecmp(sigName,"file") || !strcasecmp(sigName,"for") ||
       !strcasecmp(sigName,"function") || !strcasecmp(sigName,"generate") ||
       !strcasecmp(sigName,"generic") || !strcasecmp(sigName,"group") ||
       !strcasecmp(sigName,"guarded") || !strcasecmp(sigName,"if") ||
       !strcasecmp(sigName,"impure") || !strcasecmp(sigName,"in") ||
       !strcasecmp(sigName,"inertial") || !strcasecmp(sigName,"inout") ||
       !strcasecmp(sigName,"is") || !strcasecmp(sigName,"label") ||
       !strcasecmp(sigName,"library") || !strcasecmp(sigName,"linkage") ||
       !strcasecmp(sigName,"literal") || !strcasecmp(sigName,"loop") ||
       !strcasecmp(sigName,"map") || !strcasecmp(sigName,"mode") ||
       !strcasecmp(sigName,"nand") || !strcasecmp(sigName,"new") ||
       !strcasecmp(sigName,"next") || !strcasecmp(sigName,"nor") ||
       !strcasecmp(sigName,"not") || !strcasecmp(sigName,"off") ||
       !strcasecmp(sigName,"on") || !strcasecmp(sigName,"open") ||
       !strcasecmp(sigName,"or") || !strcasecmp(sigName,"others") ||
       !strcasecmp(sigName,"out") || !strcasecmp(sigName,"package") ||
       !strcasecmp(sigName,"port") || !strcasecmp(sigName,"postpond") ||
       !strcasecmp(sigName,"procedure") || !strcasecmp(sigName,"process") ||
       !strcasecmp(sigName,"pure") || !strcasecmp(sigName,"range") ||
       !strcasecmp(sigName,"register") || !strcasecmp(sigName,"record") ||
       !strcasecmp(sigName,"reject") || !strcasecmp(sigName,"rem") ||
       !strcasecmp(sigName,"report") || !strcasecmp(sigName,"return") ||
       !strcasecmp(sigName,"roll") || !strcasecmp(sigName,"ror") ||
       !strcasecmp(sigName,"selec") || !strcasecmp(sigName,"severity") ||
       !strcasecmp(sigName,"shared") || !strcasecmp(sigName,"signal") ||
       !strcasecmp(sigName,"sla") || !strcasecmp(sigName,"sll") ||
       !strcasecmp(sigName,"sra") || !strcasecmp(sigName,"subtype") ||
       !strcasecmp(sigName,"then") || !strcasecmp(sigName,"to") ||
       !strcasecmp(sigName,"transport") || !strcasecmp(sigName,"type") ||
       !strcasecmp(sigName,"unaffected") || !strcasecmp(sigName,"units") ||
       !strcasecmp(sigName,"until") || !strcasecmp(sigName,"use") ||
       !strcasecmp(sigName,"variable") || !strcasecmp(sigName,"wait") ||
       !strcasecmp(sigName,"when") || !strcasecmp(sigName,"while") ||
       !strcasecmp(sigName,"with") || !strcasecmp(sigName,"Xnor") ||
       !strcasecmp(sigName,"Xor"))
      printViolation(rule, line, file, sigName); 
  }
}

void checkSystemVerilogReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  const char*SigName = ivl_signal_basename(mySig);
  if (!strcasecmp(SigName,"alias") || !strcasecmp(SigName,"always_comb") ||
      !strcasecmp(SigName,"always_latch") || !strcasecmp(SigName,"always_ff") ||
      !strcasecmp(SigName,"always") || !strcasecmp(SigName,"and") ||
      !strcasecmp(SigName,"assert") || !strcasecmp(SigName,"assume") ||
      !strcasecmp(SigName,"assign") || !strcasecmp(SigName,"automatic") ||
      !strcasecmp(SigName,"before") || !strcasecmp(SigName,"bind") ||
      !strcasecmp(SigName,"begin") || !strcasecmp(SigName,"bins") ||
      !strcasecmp(SigName,"binsof") || !strcasecmp(SigName,"bit") ||
      !strcasecmp(SigName,"break") || !strcasecmp(SigName,"buf") ||
      !strcasecmp(SigName,"bufif0") || !strcasecmp(SigName,"bufif1") ||
      !strcasecmp(SigName,"case") || !strcasecmp(SigName,"casex") ||
      !strcasecmp(SigName,"casez") || !strcasecmp(SigName,"cmos") ||
      !strcasecmp(SigName,"cell") || !strcasecmp(SigName,"chandle") ||
      !strcasecmp(SigName,"class") || !strcasecmp(SigName,"clocking") ||
      !strcasecmp(SigName,"const") || !strcasecmp(SigName,"config") ||
      !strcasecmp(SigName,"constraint") || !strcasecmp(SigName,"context") ||
      !strcasecmp(SigName,"cover") || !strcasecmp(SigName,"continue") ||
      !strcasecmp(SigName,"covergroup") || !strcasecmp(SigName,"coverpoint") ||
      !strcasecmp(SigName,"cross") || !strcasecmp(SigName,"deassign") ||
      !strcasecmp(SigName,"default") || !strcasecmp(SigName,"defparam") ||
      !strcasecmp(SigName,"disable") || !strcasecmp(SigName,"design") ||
      !strcasecmp(SigName,"do") || !strcasecmp(SigName,"dist") ||
      !strcasecmp(SigName,"edge") || !strcasecmp(SigName,"else") ||
      !strcasecmp(SigName,"byte") || !strcasecmp(SigName,"endspecify") ||
      !strcasecmp(SigName,"end") || !strcasecmp(SigName,"endcase") ||
      !strcasecmp(SigName,"endmodule") || !strcasecmp(SigName,"endclass") ||
      !strcasecmp(SigName,"endclocking") || !strcasecmp(SigName,"endconfig") ||
      !strcasecmp(SigName,"endfunction") || !strcasecmp(SigName,"endgenerate") ||
      !strcasecmp(SigName,"endgroup") || !strcasecmp(SigName,"endinterface") ||
      !strcasecmp(SigName,"endpackage") || !strcasecmp(SigName,"endprogram") ||
      !strcasecmp(SigName,"endproperty") || !strcasecmp(SigName,"endsequence") ||
      !strcasecmp(SigName,"enum") || !strcasecmp(SigName,"endfunction") ||
      !strcasecmp(SigName,"endprimitive") || !strcasecmp(SigName,"endspec   ") ||
      !strcasecmp(SigName,"endtable") || !strcasecmp(SigName,"endtask") ||
      !strcasecmp(SigName,"event") || !strcasecmp(SigName,"expect") ||
      !strcasecmp(SigName,"export") || !strcasecmp(SigName,"extern") ||
      !strcasecmp(SigName,"extends") || !strcasecmp(SigName,"final") ||
      !strcasecmp(SigName,"first_match") || !strcasecmp(SigName,"fork") ||
      !strcasecmp(SigName,"forkjoin") || !strcasecmp(SigName,"for") ||
      !strcasecmp(SigName,"force") || !strcasecmp(SigName,"forever") ||
      !strcasecmp(SigName,"foreach") || !strcasecmp(SigName,"function") ||
      !strcasecmp(SigName,"genver") || !strcasecmp(SigName,"generate") ||
      !strcasecmp(SigName,"highz0") || !strcasecmp(SigName,"highz1") ||
      !strcasecmp(SigName,"if") || !strcasecmp(SigName,"iff") ||
      !strcasecmp(SigName,"ifnone") || !strcasecmp(SigName,"ignore_bins") ||
      !strcasecmp(SigName,"illegal_bins") || !strcasecmp(SigName,"include") ||
      !strcasecmp(SigName,"import") || !strcasecmp(SigName,"incdir") ||
      !strcasecmp(SigName,"initial") || !strcasecmp(SigName,"inout") ||
      !strcasecmp(SigName,"input") || !strcasecmp(SigName,"int") ||
      !strcasecmp(SigName,"instance") || !strcasecmp(SigName,"inside") ||
      !strcasecmp(SigName,"integer") || !strcasecmp(SigName,"intersect") ||
      !strcasecmp(SigName,"interface") || !strcasecmp(SigName,"join") ||
      !strcasecmp(SigName,"join_any") || !strcasecmp(SigName,"join_none") ||
      !strcasecmp(SigName,"large") || !strcasecmp(SigName,"local") ||
      !strcasecmp(SigName,"logic") || !strcasecmp(SigName,"localparam") ||
      !strcasecmp(SigName,"library") || !strcasecmp(SigName,"liblist") ||
      !strcasecmp(SigName,"longint") || !strcasecmp(SigName,"macromodule") ||
      !strcasecmp(SigName,"matches") || !strcasecmp(SigName,"modport") ||
      !strcasecmp(SigName,"medium") || !strcasecmp(SigName,"module") ||
      !strcasecmp(SigName,"nand") || !strcasecmp(SigName,"negedge") ||
      !strcasecmp(SigName,"not") || !strcasecmp(SigName,"new") ||
      !strcasecmp(SigName,"null") || !strcasecmp(SigName,"noshowcancelled") ||
      !strcasecmp(SigName,"nmos") || !strcasecmp(SigName,"nor") ||
      !strcasecmp(SigName,"notif0") || !strcasecmp(SigName,"notif1") ||
      !strcasecmp(SigName,"or") || !strcasecmp(SigName,"output") ||
      !strcasecmp(SigName,"parameter") || !strcasecmp(SigName,"process") ||
      !strcasecmp(SigName,"pmos") || !strcasecmp(SigName,"posedge") ||
      !strcasecmp(SigName,"primitive") || !strcasecmp(SigName,"pull0") ||
      !strcasecmp(SigName,"pull1") || !strcasecmp(SigName,"pullup") ||
      !strcasecmp(SigName,"pulldown") || !strcasecmp(SigName,"packed") ||
      !strcasecmp(SigName,"priority") || !strcasecmp(SigName,"program") ||
      !strcasecmp(SigName,"property") || !strcasecmp(SigName,"protected") ||
      !strcasecmp(SigName,"pulsestyle_onevent") || !strcasecmp(SigName,"rcmos") ||
      !strcasecmp(SigName,"pure") || !strcasecmp(SigName,"pulsestyle_ondetect") ||
      !strcasecmp(SigName,"real") || !strcasecmp(SigName,"realtime") ||
      !strcasecmp(SigName,"reg") || !strcasecmp(SigName,"ref") ||
      !strcasecmp(SigName,"return") || !strcasecmp(SigName,"rand") ||
      !strcasecmp(SigName,"randc") || !strcasecmp(SigName,"randcase") ||
      !strcasecmp(SigName,"randsequence") || !strcasecmp(SigName,"repeat") ||
      !strcasecmp(SigName,"release") || !strcasecmp(SigName,"rnmos") ||
      !strcasecmp(SigName,"rpmos") || !strcasecmp(SigName,"rtran") ||
      !strcasecmp(SigName,"rtranif0") || !strcasecmp(SigName,"rtranif1") ||
      !strcasecmp(SigName,"scalared") || !strcasecmp(SigName,"sequence") ||
      !strcasecmp(SigName,"shortint") || !strcasecmp(SigName,"shortreal") ||
      !strcasecmp(SigName,"showcancelled") || !strcasecmp(SigName,"signed") ||
      !strcasecmp(SigName,"solve") || !strcasecmp(SigName,"static") ||
      !strcasecmp(SigName,"string") || !strcasecmp(SigName,"struct") ||
      !strcasecmp(SigName,"super") || !strcasecmp(SigName,"specify") ||
      !strcasecmp(SigName,"small") || !strcasecmp(SigName,"strong0") ||
      !strcasecmp(SigName,"strong1") || !strcasecmp(SigName,"specparam") ||
      !strcasecmp(SigName,"supply0") || !strcasecmp(SigName,"supply1") ||
      !strcasecmp(SigName,"tagged") || !strcasecmp(SigName,"this") ||
      !strcasecmp(SigName,"throughout") || !strcasecmp(SigName,"time") ||
      !strcasecmp(SigName,"timeprecession") || !strcasecmp(SigName,"type") ||
      !strcasecmp(SigName,"timeunit") || !strcasecmp(SigName,"typedef") ||
      !strcasecmp(SigName,"table") || !strcasecmp(SigName,"task") ||
      !strcasecmp(SigName,"tran") || !strcasecmp(SigName,"tranif0") ||
      !strcasecmp(SigName,"tranif1") || !strcasecmp(SigName,"tri") ||
      !strcasecmp(SigName,"tri0") || !strcasecmp(SigName,"tri1") ||
      !strcasecmp(SigName,"triand") || !strcasecmp(SigName,"trior") ||
      !strcasecmp(SigName,"trireg") || !strcasecmp(SigName,"union") ||
      !strcasecmp(SigName,"unique") || !strcasecmp(SigName,"unsigned") ||
      !strcasecmp(SigName,"use") || !strcasecmp(SigName,"vectored") ||
      !strcasecmp(SigName,"void") || !strcasecmp(SigName,"var") ||
      !strcasecmp(SigName,"virtual") || !strcasecmp(SigName,"wait") ||
      !strcasecmp(SigName,"wait_order") || !strcasecmp(SigName,"while") ||
      !strcasecmp(SigName,"weak0") || !strcasecmp(SigName,"weak1") ||
      !strcasecmp(SigName,"wire") || !strcasecmp(SigName,"wor") ||
      !strcasecmp(SigName,"wand") || !strcasecmp(SigName,"within") ||
      !strcasecmp(SigName,"with") || !strcasecmp(SigName,"wildcard") ||
      !strcasecmp(SigName,"xor") || !strcasecmp(SigName,"xnor"))
  {
    rule = 1032;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file, SigName); 
    }
    rule = 1033;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file, SigName); 
    }
  }
}

void checkLPM(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  const char *sAct = "active";
  const char *sPat = "pattern";
  int rule = 1003;

  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    if (table[rule][sAct] == "yes")
    {
       const char *patt = "clk_[_a-zA-Z0-9]*";
       if (table[rule].find(sPat) != table[rule].end())
           patt = (char*) table[rule][sPat].c_str();
       ivl_nexus_t nex = ivl_lpm_clk(lpm);
       ivl_signal_t sig = ivl_nexus_ptr_sig(ivl_nexus_ptr(nex, 0));
       const char* nexName = ivl_signal_name(sig);
       int line = 0; //ivl_signal_lineno(sig);
       const char *file = NULL; //ivl_signal_file(sig);
        if (nexName && fnmatch(patt, nexName, 0))
       printViolation(rule, line, file, nexName, patt);
    }
  }
}

void checkSignalName(map<int, map<string, string> > & table, ivl_signal_t & sig)
{
  int rule;
  const char *sAct = "active";
  const char *sPat = "pattern";
  const char *sNml = "namelength";
  int line = ivl_signal_lineno(sig);
  const char *file = ivl_signal_file(sig);
  const char* sigName = ivl_signal_basename(sig);
  rule = 1001;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "[_a-z0-9]*";
    if (table[rule].find(sPat) != table[rule].end())
      patt = (char*) table[rule][sPat].c_str();

    if (fnmatch(patt, sigName, 0))
      printViolation(rule, line, file, sigName);
  }
  rule = 1002;
  if (table[rule][sAct] == "yes")
  {
    int nameLen = 10;
    if (table[rule].find(sNml) != table[rule].end())
      nameLen = atoi(table[rule][sNml].c_str());

    if (strlen(sigName) > nameLen)
      printViolation(rule, line, file, sigName, nameLen);
  }
  rule = 1127;
  if (table[rule][sAct] == "yes")
  {
    const char *patt = "s_*";
    if(fnmatch(patt, sigName, 0))
    {
      printViolation(rule, line, file, sigName, patt); 
    }
  }
  rule = 1185;
  if (table[rule][sAct] == "yes")
  {
    unsigned sigDim = ivl_signal_packed_dimensions(sig);
    for (int i = 0; i < sigDim; i++)
    {
      int sigLSB = ivl_signal_packed_lsb(sig, i);
      int sigMSB = ivl_signal_packed_msb(sig, i);
      if (sigLSB > sigMSB)
        printViolation(rule, line, file, sigName);
      if (sigLSB && sigMSB)
        printViolation(rule, line, file, sigName);
    }
  }
}
