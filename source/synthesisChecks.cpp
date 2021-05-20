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

void SignalConnectedBothInputOutputPorts(map<int, map<string, string> > & table, ivl_signal_t aSig)
{
  int rule = 1285;
  const char *sAct = "active";
  int line = ivl_signal_lineno(aSig);
  const char *file = ivl_signal_file(aSig);
  const char *sigName = ivl_signal_basename(aSig);
  if (table[rule][sAct] == "yes")
  {
    unsigned elements = ivl_signal_array_count(aSig);
    for (int i = 0; i < elements; i++)
    {
      bool inputConnected = false;
      bool outputConnected = false;
      ivl_nexus_t aJoint = ivl_signal_nex(aSig, i);
      unsigned connections = ivl_nexus_ptrs(aJoint);
      for (int j = 0; j < connections; j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
        ivl_signal_t connSig = ivl_nexus_ptr_sig(aConn);
	if (connSig)
	{
          if (ivl_signal_port(connSig) == IVL_SIP_INPUT)
            inputConnected = true;
          if (ivl_signal_port(connSig) == IVL_SIP_OUTPUT)
            outputConnected = true;
	}
      }
      if (inputConnected && outputConnected)
      {
        printViolation(rule, line, file, sigName);
      }
    }
  }
}

void checkPossibleLossofCarryorBorrow(map<int, map<string, string>> &table, ivl_lpm_t &net)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(net);
  const char *file = ivl_lpm_file(net);
  const char *outSigName = NULL; 
  unsigned widthA, widthB, widthQ;
  ivl_signal_t sigA, sigB, sigQ;
  widthA = widthB = widthQ = 0;
  sigA = sigB = sigQ = NULL;

  if ((ivl_lpm_type(net) == IVL_LPM_ADD) ||
      (ivl_lpm_type(net) == IVL_LPM_MULT) ||
      (ivl_lpm_type(net) == IVL_LPM_SHIFTL) ||
      (ivl_lpm_type(net) == IVL_LPM_SHIFTR))
  {
    ivl_nexus_t nexA = ivl_lpm_data(net, 0);
    if(nexA)
    {
      for(unsigned i = 0; i < ivl_nexus_ptrs(nexA); i++)
      {
        ivl_nexus_ptr_t conA = ivl_nexus_ptr(nexA, i);
        sigA = ivl_nexus_ptr_sig(conA);
        if(sigA)
        {
          widthA = ivl_signal_width(sigA);
	  break;
        }
      }
    }
    ivl_nexus_t nexB = ivl_lpm_data(net, 1);
    if(nexB)
    {
      for(unsigned i = 0; i < ivl_nexus_ptrs(nexB); i++)
      {
        ivl_nexus_ptr_t conB = ivl_nexus_ptr(nexB, i);
        sigB = ivl_nexus_ptr_sig(conB);
        if(sigB)
        {
          widthB = ivl_signal_width(sigB);
	  break;
        }
      }
    }
    ivl_nexus_t nexQ = ivl_lpm_q(net);
    if(nexQ)
    {
      for(unsigned i = 0; i < ivl_nexus_ptrs(nexQ); i++)
      {
        ivl_nexus_ptr_t conQ = ivl_nexus_ptr(nexQ, i);
        sigQ = ivl_nexus_ptr_sig(conQ);
        if(sigQ)
        {
          widthQ = ivl_signal_width(sigQ);
	  break;
        }
      }
    }
  }
  if (ivl_lpm_type(net) == IVL_LPM_MULT)
  {
    rule = 1225;
    if (table[rule][sAct] == "yes")
    {
      if(widthA + widthB > widthQ)
      {
        printViolation(rule, line, file, ivl_signal_basename(sigQ));
      }
    }
  }
  if (ivl_lpm_type(net) == IVL_LPM_ADD)
  {
    rule = 1226;
    if (table[rule][sAct] == "yes")
    {
      unsigned maxWidth;
      if(widthA > widthB)
        maxWidth = widthA;
      else
        maxWidth = widthB;

      if(maxWidth + 1 > widthQ)
      {
	// for adder, the compiler changes the width of inputs = outputs
	// so, 1226 will always be shown. can't do anything
        printViolation(rule, line, file);
      }
    }
  }
  if ((ivl_lpm_type(net) == IVL_LPM_SHIFTL) ||
      (ivl_lpm_type(net) == IVL_LPM_SHIFTR))
  {
    rule = 1229;
    if (table[rule][sAct] == "yes")
    {
      if (widthB >= widthA)
      {
        printViolation(rule, line, file, ivl_signal_basename(sigA));
      }
    }
  }
	    /*



                for (idx = 0 ;  idx < width ;  idx += 1)
                {     
                      i = idx; //printf("%s\n",ivl_nexus_name(ivl_lpm_q(net, idx)));
                      //printf("Q : %s\n",ivl_nexus_name(ivl_lpm_q(net, idx)));
                }
                for (idx = 0 ;  idx < width ;  idx += 1)
                {
                      const char* lname = ivl_nexus_name(ivl_lpm_data(net, idx));
                      char buf[20];
                      strcpy(buf,lname);

                      t1 = strtok(buf,"<");

                      j = idx; //printf("%s\n",ivl_nexus_name(ivl_lpm_data(net, idx)));
                     // printf("A : %s\n",ivl_nexus_name(ivl_lpm_data(net, idx)));
                }
                for (idx = 0 ;  idx < width ;  idx += 1)
                {
                      const char* rname = ivl_nexus_name(ivl_lpm_datab(net, idx));
                      char buf[20];
                      strcpy(buf,rname);

                      t2 = strtok(buf,"<");

                      k =idx;  //printf("%s\n",ivl_nexus_name(ivl_lpm_datab(net, idx))); 
                      //printf("B : %s\n",ivl_nexus_name(ivl_lpm_datab(net, idx)));
                }


       if( i && j && k)
           if((j+k)>=i)
             printViolation(1225,line,file,t1,t2);
              //printf("possible loss value in Multiplication\n");  
    if (table[1229][sAct] == "yes")
    {
           unsigned idx;
           unsigned width = ivl_lpm_width(net);
           static unsigned Ac = 0;
           static unsigned Sc = 0; 

           static const char*Signame ;

           switch(ivl_lpm_type(net))
           {
               case IVL_LPM_SHIFTL:
               // printf("  LPM_SHIFTL %s: <width=%u, selects=%u %s>\n",
                    //    ivl_lpm_basename(net), width, ivl_lpm_selects(net),
                  //      ivl_lpm_signed(net)? "signed" : "unsigned");
                //for (idx = 0 ;  idx < width ;  idx += 1)
                     // printf("    Q %u: %s\n", idx,
                       //       ivl_nexus_name(ivl_lpm_q(net, idx)));
                for (idx = 0 ;  idx < width ;  idx += 1)
                  if(!strstr(ivl_nexus_name(ivl_lpm_data(net, idx)),"_s")){
                    Ac++;
                     Signame = ivl_nexus_name(ivl_lpm_data(net, idx));}
                for (idx = 0 ;  idx < ivl_lpm_selects(net) ;  idx += 1)
                    Sc++;


                 if(Ac && Sc)
                   if(Ac==Sc)
                    {
                       char buf[20];
                       strcpy(buf,Signame);   
                       char* t = strtok(buf,"<");
                       printViolation(1229,line,file,t);
                    }
                    // printf("All Bits shifted Out\n");   
 
                      //printf("    Shift %u: %s\n", idx,ivl_nexus_name(ivl_lpm_select(net, idx)));
               break;  
           } 





    }




    */
}

void ProceduralContinuousAssignmentNotSynthesizable(map<int, map<string, string>> &table, ivl_statement_t net)
{
  int rule = 1205;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  if (table[rule][sAct] == "yes")
  {
    printViolation(rule, line, file);
  }
}

void checkForkStatementNotSynthesizable(map<int, map<string, string> > & table, ivl_scope_t net)
{    
  int rule = 1186;
  const char *sAct = "active";
  int line = ivl_scope_lineno(net);
  const char *file = ivl_scope_file(net);
  if (table[rule][sAct] == "yes")
  {
    const char *FName = ivl_scope_basename(net);
    printViolation(rule, line, file, FName);
  }
}

void checkEmptyModule(map<int, map<string, string> > & table, ivl_scope_t & myMod)
{   
  int rule = 1174;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    if (!(ivl_scope_logs(myMod) ||
          ivl_scope_events(myMod) ||
          ivl_scope_lpms(myMod)))
    {
      int line = ivl_scope_lineno(myMod);
      const char *file = ivl_scope_file(myMod);
      const char *modName = ivl_scope_basename(myMod);
      printViolation(rule, line, file, modName);
    }
  }
}

void checkNetType(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{   
  int rule = 1119;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  if (table[rule][sAct] == "yes")
  {
    string type;
    const char *name = ivl_signal_basename(mySig);
    switch (ivl_signal_type(mySig))
    {
      case IVL_SIT_TRI0:
        type = "tri0";
      break;
      case IVL_SIT_TRI1:
        type = "tri1";
      break;
      case IVL_SIT_TRIAND:
        type = "triand";
      break;
      case IVL_SIT_TRIOR:
        type = "trior";
      break;
      default:
        type = "";
      break;
    }
    if (type != "")
      printViolation(rule, line, file, name, type.c_str());
  }
}

void checkMemory(map<int, map<string, string> > & table, ivl_signal_t & mem)
{
  int rule = 0; 
  const char *sAct = "active";
  int line = ivl_signal_lineno(mem);
  const char *file = ivl_signal_file(mem);
  const char *memName = ivl_signal_basename(mem);
  if (ivl_signal_type(mem) == IVL_SIT_REG)
  {
    rule = 1116; 
    if ((table[rule][sAct] == "yes") && (ivl_signal_dimensions(mem) > 0))
    {
      printViolation(rule, line, file, memName);
    }
    rule = 1117; 
    if (table[rule][sAct] == "yes")
    {
      int memLen = 2048;
      const char *smLen = "memLength";
      if (table[rule].find(smLen) != table[rule].end())
      {
        memLen = stoi(table[rule][smLen]);
      }
      if (ivl_signal_array_count(mem) > memLen)
      {
        printViolation(rule, line, file, ivl_signal_array_count(mem), memName);
      }
    }
    rule = 1118; 
    if (table[rule][sAct] == "yes")
    {
      int wordLen = 8;
      const char *swLen = "wordLength";
      if (table[rule].find(swLen) != table[rule].end())
      {
        wordLen = stoi(table[rule][swLen]);
      }
      if (ivl_signal_width(mem) > wordLen)
      {
        printViolation(rule, line, file, ivl_signal_width(mem), memName);
      }
    }
  }
}

void variable_synthesizeble(map<int, map<string, string> > & table, ivl_signal_t &mySig)
{
  int rule = 1115; // also 1200 and 1329
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_data_type(mySig) == IVL_VT_REAL) // also realtime
    {
      unsigned line = ivl_signal_lineno(mySig);
      const char *file = ivl_signal_file(mySig);
      const char *sigName = ivl_signal_basename(mySig);
      printViolation(rule, line, file, sigName, "real");
    }
  }
}

void checkIntegerConcatenation(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1105;
  const char *sAct = "active";
  if ((table[rule][sAct] == "yes") && ivl_signal_integer(mySig))
  {
    const char *sigName = ivl_signal_basename(mySig);
    unsigned elements = ivl_signal_array_count(mySig);
    for (int i = 0; i < elements; i++)
    {
      ivl_nexus_t aJoint = ivl_signal_nex(mySig, i);
      unsigned connections = ivl_nexus_ptrs(aJoint);
      for (int j = 0; j < connections; j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
        if (ivl_nexus_ptr_lpm(aConn))
        {
          ivl_lpm_t anLPM = ivl_nexus_ptr_lpm(aConn);
          if (ivl_lpm_type(anLPM) == IVL_LPM_CONCAT)
          {
            int line = ivl_lpm_lineno(anLPM);
            const char *file = ivl_lpm_file(anLPM);
            printViolation(rule, line, file, sigName);
            break;
          }
        }
      }
    }
  }
}

void checkMultipleClock(map<int, map<string, string> > & table, ivl_event_t & evt)
{
  // this implementation of 1102 doesn't work
  int rule = 1102;
  const char *sAct = "active";
  int line = ivl_event_lineno(evt);
  const char *file = ivl_event_file(evt);
  if (table[rule][sAct] == "yes")
  {
    int edge = ivl_event_nneg(evt);   
    edge += ivl_event_npos(evt);   
    if (edge > 1)
    {
       printViolation(rule, line, file);
    }
  } 
}

void checkBiDirecTionalPort(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1098;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_port(mySig)==IVL_SIP_INOUT)
    {
        printViolation(rule, line, file, ivl_signal_basename(mySig)); 
    }
  }
}

void checkDriveSynth(map<int, map<string, string> > & table, ivl_signal_t &mySig)
{
  int rule = 1021;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);

  if (table[rule][sAct] == "yes")
  {
    const char *type = NULL;
    unsigned elements = ivl_signal_array_count(mySig);
    for (int idx = 0; idx < elements; idx++)
    {
      ivl_nexus_t nex = ivl_signal_nex(mySig, idx);
      for(unsigned i = 0; nex && i < ivl_nexus_ptrs(nex); i++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(nex, i);
        for (unsigned j = 0; j < 2; j++)
        {
          ivl_drive_t dr =  ivl_nexus_ptr_drive0(aConn);
          if (j == 1)
            dr =  ivl_nexus_ptr_drive1(aConn);

          switch(dr)
          { 
            case IVL_DR_SMALL:
              type = "SMALL"; 
            break;
            case IVL_DR_MEDIUM:
              type = "MEDIUM"; 
            break;
            case IVL_DR_WEAK:
              type = "WEAK"; 
            break;
            case IVL_DR_LARGE:
              type = "LARGE";
            case IVL_DR_PULL:
              type = "PULL"; 
            break;
            case IVL_DR_STRONG:
              type = "STRONG";
            break;
            default:
              type = NULL;
            break;
          }
          if (type)
            printViolation(rule, line, file, type);
        }
      }
    }
  }
  if (table[1048][sAct] == "yes")
  {
    if(!strcasecmp(ivl_signal_basename(mySig), "WE_mem"))
      printViolation(1048, line, file, ivl_signal_name(mySig));
  }
}

void checkUDPInstanceNotSynthesizable(map<int, map<string, string> > & table, ivl_net_logic_t &myLog)
{
  int rule = 1024; // same as 1123
  const char *sAct = "active";
  int line = ivl_logic_lineno(myLog);
  const char *file = ivl_logic_file(myLog);

  if (table[rule][sAct] == "yes")
  {
    if (ivl_logic_type(myLog) == IVL_LO_UDP)
    {
      const char *udpName = ivl_logic_basename(myLog);
      printViolation(rule, line, file, udpName);
    }
  }
}

void IBlock(map<int, map<string, string> > & table, ivl_process_t proc)
{
  int rule = 1089;
  const char *sAct = "active";
  int line = ivl_process_lineno(proc);
  const char *file = ivl_process_file(proc);
  if (table[rule][sAct] == "yes")
  {
    if (ivl_process_type(proc) == IVL_PR_INITIAL)
    {
      printViolation(rule, line, file);
    }
  }
}

void checkUserTask(map<int, map<string, string> > & table, ivl_statement_t net)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  const char *scpName = ivl_scope_basename(ivl_stmt_call(net));
  switch (ivl_statement_type(net))
  {
    case IVL_ST_UTASK:
    {
      rule = 1069;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, scpName);
      }
    }
    break;
    case IVL_ST_DISABLE:
    {
      rule = 1070;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, scpName);
      }
    }
    break;
    default:
    {
    }
    break;
  }
}

void checkNestedEvents(map<int, map<string, string> > & table, ivl_statement_t net, bool firsTime)
{
  int rule = 0;
  const char *sAct = "active";
  if (!firsTime)
  {
    int line = ivl_stmt_lineno(net);
    const char *file = ivl_stmt_file(net);
    rule = 1065;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
    rule = 1068;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }
}

void checkRepeatExpression(map<int, map<string, string> > & table, ivl_statement_t net)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  if (ivl_statement_type(net) == IVL_ST_REPEAT)
  {
    rule = 1067;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }
  else
  {
    rule = 1066;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
    rule = 1078;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }
//      ivl_expr_t repExpr = ivl_stmt_cond_expr(net);
//      ivl_expr_type_t expTyp = ivl_expr_type(repExpr);
}

void traverseLoopExpression(map<int, map<string, string>> &table, ivl_expr_t loopExp, ivl_signal_t &loopVar)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_expr_lineno(loopExp);
  const char *file = ivl_expr_file(loopExp);
  ivl_expr_t opr1 = NULL;
  ivl_expr_t opr2 = NULL;
  ivl_expr_t opr3 = NULL;
  switch (ivl_expr_type(loopExp))
  {
    case IVL_EX_SIGNAL:
    {
      ivl_signal_t loopSig = ivl_expr_signal(loopExp);
      if (loopSig)
      {
        const char *loopSigName = ivl_signal_basename(loopSig);
        if (loopVar)
	{
          rule = 1242;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, loopSigName);
          }
	}
	else
	{
          loopVar = loopSig;
	}
      }
    }
    break;
    case IVL_EX_NUMBER:
    {
      rule = 1201;
      if (table[rule][sAct] == "yes")
      {
        const char *valBits = ivl_expr_bits(loopExp);
        if (strchr(valBits, 'x') || strchr(valBits, 'X'))
        {
          printViolation(rule, line, file);
        }
      }
    }
    break;
    case IVL_EX_STRING:
    {
      rule = 1082;
      if (table[rule][sAct] == "yes")
      {
          printViolation(rule, line, file, ivl_expr_string(loopExp));
      }
    }
    break;
    case IVL_EX_UNARY:
    case IVL_EX_SELECT:
    {
      opr1 = ivl_expr_oper1(loopExp);
      traverseLoopExpression(table, opr1, loopVar);
    }
    break;
    case IVL_EX_BINARY:
    {
      opr1 = ivl_expr_oper1(loopExp);
      traverseLoopExpression(table, opr1, loopVar);
      opr2 = ivl_expr_oper2(loopExp);
      traverseLoopExpression(table, opr2, loopVar);
    }
    break;
    case IVL_EX_TERNARY:
    {
      opr1 = ivl_expr_oper1(loopExp);
      traverseLoopExpression(table, opr1, loopVar);
      opr2 = ivl_expr_oper2(loopExp);
      traverseLoopExpression(table, opr2, loopVar);
      opr3 = ivl_expr_oper3(loopExp);
      traverseLoopExpression(table, opr3, loopVar);
    }
    break;
    default:
    {
    }
    break;
  }
}

void WhileLoop(map<int, map<string, string>> &table, ivl_statement_t net)
{

  int rule = 1060; // same as 1254. FOR/WHILE lloop
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  if (table[rule][sAct] == "yes")
  {
    printViolation(rule, line, file);
  }
}

void ReleaseStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *forceSigs)
{
  int rule = 1056;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  if (table[rule][sAct] == "yes")
  { 
    printViolation(rule, line, file);
  }
  if (forceSigs)
  {
    for (unsigned idx = 0; idx < ivl_stmt_lvals(net); idx++)
    {
      ivl_signal_t lhSig = ivl_lval_sig(ivl_stmt_lval(net, idx));
      if (lhSig)
      {
        if (forceSigs->find(lhSig) != forceSigs->end())
        {
          forceSigs->erase(lhSig);
        }
	else
        {
          rule = 1057;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file);
          }
        }
      }
    }
  }
  else
  {
    rule = 1057;
    if (table[rule][sAct] == "yes")
    { 
      printViolation(rule, line, file);
    }
  }
}

void ForceStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *forceSigs)
{
  int rule = 1054;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  if (table[rule][sAct] == "yes")
  { 
    printViolation(rule, line, file);
  }
  if (forceSigs)
  {
    for (unsigned idx = 0; idx < ivl_stmt_lvals(net); idx++)
    {
      ivl_signal_t lhSig = ivl_lval_sig(ivl_stmt_lval(net, idx));
      if (lhSig && forceSigs->find(lhSig) == forceSigs->end())
      {
        forceSigs->insert(lhSig);
      }
    }
  }
  else
  {
    rule = 1055;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }
}

void SystemTaskCall(map<int, map<string, string> > & table, ivl_statement_t net)
{
  int rule = 1053;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  { 
    int line = ivl_stmt_lineno(net);
    const char *file = ivl_stmt_file(net);
    printViolation(rule, line, file, ivl_stmt_name(net));
  }
}

void DelayControl(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> &sigLst, set<ivl_signal_t> &sigSet)
{
  int rule = 0;
  const char *sAct = "active";
  ivl_statement_type_t code = ivl_statement_type(net);
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  switch (code)
  {
    case IVL_ST_ASSIGN:
    case IVL_ST_ASSIGN_NB:
    {
      if (ivl_stmt_delay_expr(net))
      {
        int rule = 1052;
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }
      break;
      case IVL_ST_DELAYX:
      {
        int rule = 1079;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, "#");
        }
      }
      case IVL_ST_DELAY:
      {
        int rule = 1052;
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
	ivl_statement_t dlyStmt = ivl_stmt_sub_stmt(net);
	if (dlyStmt && ivl_statement_type(dlyStmt) == IVL_ST_ASSIGN_NB)
	{
          int rule = 1072;
          if (table[rule][sAct] == "yes")
          { 
            printViolation(rule, line, file);
          }
	}
	ivl_signal_t dummy = NULL;
	checkProcesStatement(table, dlyStmt, dummy, sigLst, sigSet);
      }
      break;
      default:
      {
      }
      break;
    }
  }
}
