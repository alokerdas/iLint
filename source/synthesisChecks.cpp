#include "ivl_target.h"
#include "lint.h"

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
  int rule = 1115;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_data_type(mySig) == IVL_VT_REAL)
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

void WhileLoop(map<int, map<string, string> > & table, ivl_statement_t net)
{

  int rule = 1060;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    int line = ivl_stmt_lineno(net);
    const char *file = ivl_stmt_file(net);
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

void DelayControl(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigLst)
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
	checkProcesStatement(table, dlyStmt, sigLst);
      }
      break;
      default:
      {
      }
      break;
    }
  }
}
