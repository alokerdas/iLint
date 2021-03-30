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

void checkNonConstShiftAmt(map<int, map<string, string> > & table, ivl_expr_t expr, bool firsTime)
{
  int rule = 1162;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  { 
    switch (ivl_expr_type(expr))
    {
      case IVL_EX_SIGNAL:
      {
        ivl_signal_t aSig = ivl_expr_signal(expr);
	const char *aSigName = ivl_signal_basename(aSig);
        int line = ivl_expr_lineno(expr);
        const char *file = ivl_expr_file(expr);
        printViolation(rule, line, file, aSigName);
      }
      break;
      case IVL_EX_BINARY:
      {
	if (firsTime)
        {
          if (ivl_expr_opcode(expr) == 'l')
          {
            checkNonConstShiftAmt(table, ivl_expr_oper2(expr), false);
          }
        }
	else
        {
          checkNonConstShiftAmt(table, ivl_expr_oper1(expr), false);
          checkNonConstShiftAmt(table, ivl_expr_oper2(expr), false);
        }
      }
      break;
      default:
      break;
    }
  }
}

void checkIntegerNegative(map<int, map<string, string> > & table, ivl_statement_t & myStmt)
{
  int rule = 1086;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(myStmt);
  const char *file = ivl_stmt_file(myStmt);
  for (unsigned idx = 0 ;  idx < ivl_stmt_lvals(myStmt) ;  idx++)
  {
    ivl_lval_t lv =  ivl_stmt_lval(myStmt, idx);
    ivl_signal_t lvSig =  ivl_lval_sig(lv);
    if (lvSig)
    {
      const char *lvSigName = ivl_signal_basename(lvSig);
      ivl_expr_t rvExp = ivl_stmt_rval(myStmt);
      if (ivl_expr_type(rvExp) == IVL_EX_NUMBER)
      {
        const char *rvBits = ivl_expr_bits(rvExp);
        if (rvBits[strlen(rvBits) -1] == '1')
        {
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, lvSigName);
          }
        }
      }
    }
  }
}

void checkDeassign(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *asnSigs)
{
  int rule = 1062;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  if (asnSigs)
  {
    for (unsigned idx = 0; idx < ivl_stmt_lvals(net); idx++)
    {
      ivl_signal_t lhSig = ivl_lval_sig(ivl_stmt_lval(net, idx));
      if (lhSig)
      {
        if (asnSigs->find(lhSig) != asnSigs->end())
        {
          asnSigs->erase(lhSig);
        }
	else
        {
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
    if (table[rule][sAct] == "yes")
    { 
      printViolation(rule, line, file);
    }
  }
}

void checkUnsignedVector(map<int, map<string, string> > & table, ivl_expr_t net)
{
  int rule = 1140;
  const char *sAct = "active";
  int line = ivl_expr_lineno(net);
  const char *file = ivl_expr_file(net);
  if (table[rule][sAct] == "yes")
  {
    if((ivl_expr_type(net) == IVL_EX_BINARY) &&
       ((ivl_expr_opcode(net) == 'e') ||
        (ivl_expr_opcode(net) == '>') ||
        (ivl_expr_opcode(net) == '<')))
    {
      ivl_expr_t expSig = ivl_expr_oper1(net);
      if (ivl_expr_type(expSig) == IVL_EX_SIGNAL)
      {
        ivl_signal_t lSig = ivl_expr_signal(expSig);
	const char *lSigName = ivl_signal_basename(lSig);
	if (!ivl_signal_signed(lSig))
	{
          ivl_expr_t rVal = ivl_expr_oper2(net);
          if (ivl_expr_type(rVal) == IVL_EX_NUMBER)
          {
            const char *rBits = ivl_expr_bits(rVal);
            if (rBits[strlen(rBits) -1] == '1')
            {
              printViolation(rule, line, file, lSigName, rBits);
            }
	  }
	}
      }
    }
  }
}

void checkNonConstDivisor(map<int, map<string, string> > & table, ivl_lpm_t & net)
{
  int rule = 1125;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(net);
  const char *file = ivl_lpm_file(net);
  if(ivl_lpm_type(net) == IVL_LPM_DIVIDE)
  {
    for (int i = 0 ;  i < 2 ;  i++)
    {
      ivl_nexus_t divNex = ivl_lpm_data(net, i);
      for(unsigned j = 0; j < ivl_nexus_ptrs(divNex); j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(divNex, j);
        ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
        if(aConnSig)
        {
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, ivl_signal_basename(aConnSig));
          }
	  break;
        }
      }
      rule++;
    }  
  }
}

void checkEdgeNonEdge(map<int, map<string, string> > & table, ivl_event_t & evt)
{
  string nameAny; 
  string namePedge; 
  string nameNedge; 
  int rule = 1120;
  const char *sAct = "active";
  unsigned line = ivl_event_lineno(evt);
  const char *file = ivl_event_file(evt);
  if (table[rule][sAct] == "yes")
  {
    for(unsigned i = 0; i < ivl_event_nany(evt); i++)
    {
      ivl_nexus_t evtNex  = ivl_event_any(evt, i);
      for(unsigned j = 0; j < ivl_nexus_ptrs(evtNex); j++)
      {       
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(evtNex, j);
        ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
        if(aConnSig)
        {
          nameAny += ivl_signal_basename(aConnSig);
	  if (i < ivl_event_nany(evt) - 1)
            nameAny += ",";
        }
      }
    }
    for(unsigned i = 0; i < ivl_event_npos(evt); i++)
    {
      ivl_nexus_t evtNex  = ivl_event_pos(evt, i);
      for(unsigned j = 0; j < ivl_nexus_ptrs(evtNex); j++)
      {       
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(evtNex, j);
        ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
        if(aConnSig)
        {
          namePedge += ivl_signal_basename(aConnSig);
	  if (i < ivl_event_npos(evt) - 1)
            namePedge += ",";
        }
      }
    }
    for(unsigned i = 0; i < ivl_event_nneg(evt); i++)
    {
      ivl_nexus_t evtNex  = ivl_event_neg(evt, i);
      for(unsigned j = 0; j < ivl_nexus_ptrs(evtNex); j++)
      {       
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(evtNex, j);
        ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
        if(aConnSig)
        {
          nameNedge += ivl_signal_basename(aConnSig);
	  if (i < ivl_event_nneg(evt) - 1)
            nameNedge += ",";
        }
      }
    }
    if(!nameAny.empty())
    {
      if (!namePedge.empty())
        printViolation(rule, line, file, namePedge.c_str(), nameAny.c_str()); 
      if (!nameNedge.empty())
        printViolation(rule, line, file, nameNedge.c_str(), nameAny.c_str()); 
    }
  }
}

void checkDirectInputOutput(map<int, map<string, string> > & table, ivl_statement_t & myStmt)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(myStmt);
  const char *file = ivl_stmt_file(myStmt);
  for (unsigned idx = 0 ;  idx < ivl_stmt_lvals(myStmt) ;  idx += 1)
  {
    ivl_lval_t lv =  ivl_stmt_lval(myStmt, idx);
    ivl_signal_t lvSig =  ivl_lval_sig(lv);
    if (lvSig)
    {
      const char *lvSigName = ivl_signal_basename(lvSig);
      ivl_expr_t rvExp = ivl_stmt_rval(myStmt);
      if (ivl_expr_type(rvExp) == IVL_EX_SIGNAL)
      {
        ivl_signal_t rvSig = ivl_expr_signal(rvExp);
        const char *rvSigName = ivl_signal_basename(rvSig);
        if ((ivl_signal_port(rvSig) == IVL_SIP_INPUT) &&
            (ivl_signal_port(lvSig) == IVL_SIP_OUTPUT))
        {
          rule = 1097;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, rvSigName, lvSigName);
          }
        }
      }
    }
  }
}

void traverseExpression(map<int, map<string, string> > & table, ivl_statement_t net, ivl_signal_t lvSig, set<ivl_signal_t> *lhSigs, set<ivl_signal_t> *sigLst)
{
  int rule = 0;
  const char *sAct = "active";
  ivl_expr_t rvExp = ivl_stmt_rval(net);
  int line = ivl_expr_lineno(rvExp);
  const char *file = ivl_expr_file(rvExp);
  ivl_expr_t opr1 = NULL;
  while (rvExp)
  {
    switch (ivl_expr_type(rvExp))
    {
      case IVL_EX_SIGNAL:
      {
        ivl_signal_t rvSig = ivl_expr_signal(rvExp);
        const char *rvSigName = rvSig ? ivl_signal_basename(rvSig) : NULL;
        if (rvSig == lvSig)
        {
          rule = 1075;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, rvSigName);
          }
        }
        rule = 1128;
        if (table[rule][sAct] == "yes")
        {
	  if (lvSig)
	  {
            const char *lvSigName = ivl_signal_basename(lvSig);
	    if (ivl_signal_signed(lvSig))
	    {
	      if (!ivl_signal_signed(rvSig))
	      {
                printViolation(rule, line, file, rvSigName, lvSigName);
	      }
	    }
            else
            {
              if (ivl_signal_signed(rvSig))
              {
                printViolation(rule, line, file, lvSigName, rvSigName);
              }
            }
          }
        }
	if (rvExp == opr1)
	{
          rvExp = NULL;
	}
	else
	{
	  rvExp = opr1;
	  if (opr1 && (ivl_expr_type(opr1) == IVL_EX_UNARY ||
	               ivl_expr_type(opr1) == IVL_EX_SELECT))
	  {
            opr1 = NULL;
	  }
	}
        if (sigLst && (sigLst->find(rvSig) == sigLst->end()))
        {
          rule = 1081; // same as 1159, not implemented
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, rvSigName);
          }
        }
        if (rvSig && (ivl_signal_port(rvSig) == IVL_SIP_NONE)
                  && (ivl_statement_type(net) == IVL_ST_ASSIGN_NB))
        {
          rule = 1204;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, rvSigName);
          }
        }
        if (rvSig && lhSigs && (lhSigs->find(rvSig) != lhSigs->end()))
        {
          rule = 1206;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, rvSigName);
          }
        }
      }
      break;
      case IVL_EX_UNARY:
      {
        rvExp = ivl_expr_oper1(rvExp);
      }
      break;
      case IVL_EX_BINARY:
      {
        rule = 1202;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_expr_opcode(rvExp));
        }
        opr1 = ivl_expr_oper1(rvExp);
        rvExp = ivl_expr_oper2(rvExp);
      }
      break;
      case IVL_EX_SELECT:
      {
        rvExp = ivl_expr_oper1(rvExp);
      }
      break;
      case IVL_EX_NUMBER:
      {
        const char *valBits = ivl_expr_bits(rvExp);
        if (strchr(valBits, 'x') || strchr(valBits, 'X'))
        {
          rule = 1201;
          const char *file = ivl_expr_file(rvExp);
          int line = ivl_expr_lineno(rvExp);
          printViolation(rule, line, file);
        }
        rvExp = NULL;
      }
      break;
      case IVL_EX_STRING:
      {
        rule = 1082;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_expr_string(rvExp));
        }
        rvExp = NULL;
      }
      break;
      case IVL_EX_TERNARY:
      {
        rule = 1202;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_expr_opcode(rvExp));
        }
        rvExp = NULL;
      }
      break;
      default:
      {
        rvExp = NULL;
      }
      break;
    }
  }
}

void SignalAssignedToSelf(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigLst, set<ivl_signal_t> *lhSigs)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);
  for (unsigned idx = 0; idx < ivl_stmt_lvals(net);  idx++)
  {
    ivl_lval_t lv =  ivl_stmt_lval(net, idx);
    ivl_signal_t lvSig =  ivl_lval_sig(lv);
    if (lvSig)
    {
      const char *lvSigName = ivl_signal_basename(lvSig);
      if (lhSigs)
      {
        if (lhSigs->find(lvSig) == lhSigs->end())
        {
          lhSigs->insert(lvSig);
        }
        else
        {
          rule = 1077; // same as 1203, not implemented
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, lvSigName);
          }
	  //delete lhSigs; can't delete, its complecated.
        }
      }
      else
      {
	lhSigs = new set<ivl_signal_t>;
        lhSigs->insert(lvSig);
      }
      traverseExpression(table, net, lvSig, lhSigs, sigLst);
    }
  }
}

void checkCaseXZ(map<int, map<string, string> > & table, ivl_statement_t net)
{
  int rule = 1076; // same as 1207, not implemented
  const char *sAct = "active";
  const char *casexz = NULL;
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);

  ivl_expr_t casCondExpr = ivl_stmt_cond_expr(net);
  if (table[rule][sAct] == "yes")
  {
    if (ivl_expr_type(casCondExpr) == IVL_EX_NUMBER)
    {
      printViolation(rule, line, file);
    }
  }

  rule = 1064; // 1061, 1062, 1063 are not implemented
  if (table[rule][sAct] == "yes")
  {
    if (ivl_expr_type(casCondExpr) == IVL_EX_UNARY ||
        ivl_expr_type(casCondExpr) == IVL_EX_BINARY)
    {
      printViolation(rule, line, file, ivl_expr_opcode(casCondExpr));
    }
    if (ivl_expr_type(casCondExpr) == IVL_EX_TERNARY)
    {
      printViolation(rule, line, file, '?');
    }
  }

  switch (ivl_statement_type(net))
  {
    case IVL_ST_CASEZ:
    {
      rule = 1027;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file);
      }

      rule = 1047;
      if (table[rule][sAct] == "yes")
      {
        unsigned casCnt = ivl_stmt_case_count(net);
        for (unsigned idx = 0; idx < casCnt; idx ++) 
        {
          ivl_expr_t lblExp = ivl_stmt_case_expr(net, idx);
          if (ivl_expr_type(lblExp) == IVL_EX_NUMBER)
          {
            const char *lblBits = ivl_expr_bits(lblExp);
            if (strchr(lblBits, 'x') || strchr(lblBits, 'X'))
            {
              int line = ivl_expr_lineno(lblExp);
              const char *file = ivl_expr_file(lblExp);
              printViolation(rule, line, file);
            }
          }
        }
      }
    }
    break;
    case IVL_ST_CASEX:
    {
      rule = 1028;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file);
      }
    }
    break;
    default:
    break;
  }
}

void checkCaseLabels(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigLst, set<ivl_signal_t> *lhSigs)
{
  const char *sAct = "active";

  ivl_expr_t casCondExpr = ivl_stmt_cond_expr(net);
  int line = ivl_expr_lineno(casCondExpr);
  const char *file = ivl_expr_file(casCondExpr);

  int rule = 1064; // 1061, 1062, 1063 are not implemented
  if (table[rule][sAct] == "yes")
  {
    if (ivl_expr_type(casCondExpr) == IVL_EX_UNARY ||
        ivl_expr_type(casCondExpr) == IVL_EX_BINARY)
    {
      printViolation(rule, line, file, ivl_expr_opcode(casCondExpr));
    }
    if (ivl_expr_type(casCondExpr) == IVL_EX_TERNARY)
    {
      printViolation(rule, line, file, '?');
    }
  }

  rule = 1076;
  if (table[rule][sAct] == "yes")
  {
    if (ivl_expr_type(casCondExpr) == IVL_EX_NUMBER)
    {
      printViolation(rule, line, file);
    }
  }

  unsigned casExpWidth = 0;
  unsigned operWidth = 0;
  ivl_expr_t opr1 = NULL;
  if (ivl_expr_type(casCondExpr) != IVL_EX_NUMBER)
  {
    opr1 = ivl_expr_oper1(casCondExpr);
  }
  while (opr1)
  {
    if (ivl_expr_type(opr1) == IVL_EX_SIGNAL)
    {
      operWidth = ivl_signal_width(ivl_expr_signal(opr1));
      if (operWidth > casExpWidth)
      {
	casExpWidth = operWidth;
      }
      if (ivl_expr_oper2(casCondExpr) == opr1)
      {
	break;
      }
      else
      {
        opr1 = ivl_expr_oper2(casCondExpr); //may need change
      }
    }
    else
    {
      casCondExpr = opr1;
      opr1 = (ivl_expr_type(casCondExpr) != IVL_EX_NUMBER) ? ivl_expr_oper1(opr1) : NULL;
    }
  }

  unsigned casCnt = ivl_stmt_case_count(net);
  for (unsigned idx = 0; idx < casCnt; idx ++) 
  {
    ivl_expr_t lblExp = ivl_stmt_case_expr(net, idx);
    switch (ivl_expr_type(lblExp))
    {
      case IVL_EX_NUMBER:
      {
        rule = 1026;
        line = ivl_expr_lineno(lblExp);
        file = ivl_expr_file(lblExp);
        if (table[rule][sAct] == "yes")
        {
          const char *lblBits = ivl_expr_bits(lblExp);
          if (strchr(lblBits, 'z') || strchr(lblBits, 'Z'))
          {
            printViolation(rule, line, file, "z or Z");
          }
          if (strchr(lblBits, 'x') || strchr(lblBits, 'X'))
          {
            printViolation(rule, line, file, "x or X");
          }
        }

        rule = 1044;
        unsigned lblExpWidth = ivl_expr_uvalue(lblExp);
        if (lblExpWidth >= pow(2, casExpWidth))
        {
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, casExpWidth);
          }
        }
      }
      break;
      case IVL_EX_SELECT:
      {
        rule = 1218;
        line = ivl_expr_lineno(lblExp);
        file = ivl_expr_file(lblExp);
        if (table[rule][sAct] == "yes")
        {
          string somStr;
          switch (ivl_expr_value(lblExp))
	  {
            case IVL_VT_LOGIC:
            {
              somStr = "LOGIC SIGNAL";
	    }
	    break;
	    default:
	    break;
	  }
          printViolation(rule, line, file, somStr.c_str());
        }
      }
      break;
      default:
      {
        if (idx >= pow(2, ivl_expr_width(casCondExpr)))
        {
          rule = 1219;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file);
          }
        }
        // Here default case, using rule as a flag
        rule = 0;
      }
      break;
    }
    checkProcesStatement(table, ivl_stmt_case_stmt(net, idx), sigLst, lhSigs);
  }
  if (casCnt && rule)
  {
    rule = 1046;
    if (table[rule][sAct] == "yes")
    {
      line = ivl_stmt_lineno(net);
      file = ivl_stmt_file(net);
      printViolation(rule, line, file);
    }
  }
}

bool checkEvent(map<int, map<string, string> > & table, ivl_event_t & evt, set<ivl_signal_t> *sigLst)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_event_lineno(evt);
  const char *file = ivl_event_file(evt);
  const char *senSigName = NULL;

  for(unsigned i = 0; i < ivl_event_nany(evt); i++)
  {
    ivl_nexus_t aLevel  = ivl_event_any(evt, i);   
    for(unsigned j = 0 ; j < ivl_nexus_ptrs(aLevel); j++)
    {
      ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aLevel, j);
      ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
      if(aConnSig)
      {
        senSigName = ivl_signal_basename(aConnSig);
        if (sigLst)
        {
          if (sigLst->find(aConnSig) == sigLst->end())
          {
            sigLst->insert(aConnSig);
          }
          else
          {
            rule = 1091;
            if (table[rule][sAct] == "yes")
            { 
              printViolation(rule, line, file, senSigName);
            }
          }
        }
      }

      ivl_net_logic_t aConnGate = ivl_nexus_ptr_log(aConn);
      if(aConnGate)
      {
        rule = 1096; // same as 1020
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }

      ivl_lpm_t aConnLpm = ivl_nexus_ptr_lpm(aConn);
      if(aConnLpm && (ivl_lpm_type(aConnLpm) == IVL_LPM_PART_VP))
      {
        rule = 1160;
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }

      ivl_net_const_t aConnConst = ivl_nexus_ptr_con(aConn);
      if(aConnConst)
      {
        rule = 1093;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
    }
  }
  for(unsigned i = 0; i < ivl_event_nneg(evt); i++)
  {
    ivl_nexus_t aNegEdge  = ivl_event_neg(evt, i);
    unsigned connections = ivl_nexus_ptrs(aNegEdge);
    for(unsigned j = 0 ; j < connections; j++)
    {
      ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aNegEdge, j);
      ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
      if(aConnSig)
      {
        senSigName = ivl_signal_basename(aConnSig);
        if (sigLst)
        {
          if (sigLst->find(aConnSig) == sigLst->end())
          {
            sigLst->insert(aConnSig);
          }
          else
          {
            rule = 1091;
            if (table[rule][sAct] == "yes")
            { 
              printViolation(rule, line, file);
            }
          }
        }
      }

      ivl_net_logic_t aConnGate = ivl_nexus_ptr_log(aConn);
      if(aConnGate)
      {
        rule = 1096; // same as 1020
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }

      ivl_lpm_t aConnLpm = ivl_nexus_ptr_lpm(aConn);
      if(aConnLpm && (ivl_lpm_type(aConnLpm) == IVL_LPM_PART_VP))
      {
        rule = 1160;
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }

      ivl_net_const_t aConnConst = ivl_nexus_ptr_con(aConn);
      if(aConnConst)
      {
        rule = 1093;
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }
    }
  }
  for(unsigned i = 0; i < ivl_event_npos(evt); i++)
  {
    ivl_nexus_t aPosEdge  = ivl_event_pos(evt, i);
    unsigned connections = ivl_nexus_ptrs(aPosEdge);
    for(unsigned j = 0 ; j < connections; j++)
    {
      ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aPosEdge, j);
      ivl_signal_t aConnSig = ivl_nexus_ptr_sig(aConn);
      if(aConnSig)
      {
        senSigName = ivl_signal_basename(aConnSig);
        if (sigLst)
        {
          if (sigLst->find(aConnSig) == sigLst->end())
          {
            sigLst->insert(aConnSig);
          }
          else
          {
            rule = 1091;
            if (table[rule][sAct] == "yes")
            { 
              printViolation(rule, line, file, senSigName);
            }
          }
        }
      }

      ivl_net_logic_t aConnGate = ivl_nexus_ptr_log(aConn);
      if(aConnGate)
      {
        rule = 1096; // same as 1020
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file);
        }
      }

      ivl_lpm_t aConnLpm = ivl_nexus_ptr_lpm(aConn);
      if(aConnLpm && (ivl_lpm_type(aConnLpm) == IVL_LPM_PART_VP))
      {
        rule = 1160;
        if (table[rule][sAct] == "yes")
        { 
          printViolation(rule, line, file, senSigName);
        }
      }

      ivl_net_const_t aConnConst = ivl_nexus_ptr_con(aConn);
      if(aConnConst)
      {
        rule = 1093;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
    }
  }
  bool edgeEv = false;
  if (ivl_event_nneg(evt) || ivl_event_npos(evt))
  {
    edgeEv = true;
  }
  return edgeEv;
}

void checkVectorIndexSufficient(map<int, map<string, string> > & table, ivl_expr_t rExp)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_expr_lineno(rExp);
  const char *file = ivl_expr_file(rExp);

  ivl_expr_t opr1 = ivl_expr_oper1(rExp);
  if (ivl_expr_type(opr1) == IVL_EX_SIGNAL)
  {
    ivl_signal_t opr1Sig = ivl_expr_signal(opr1);
    ivl_expr_t opr2 = ivl_expr_oper2(rExp);
    if (ivl_expr_type(opr2) == IVL_EX_SIGNAL)
    {
      ivl_signal_t opr2Sig = ivl_expr_signal(opr2);
      unsigned oper1Width = ivl_signal_width(opr1Sig);
      unsigned oper2Width = ivl_signal_width(opr2Sig);
      oper2Width = pow(2, oper2Width);
      if (oper1Width > oper2Width)
      {
        rule = 1035;
      }
      if (oper1Width < oper2Width)
      {
        rule = 1036;
      }
      if (rule && table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, oper2Width);
      }
      if (!ivl_signal_dimensions(opr1Sig))
      {
        rule = 1121;
        printViolation(rule, line, file, ivl_signal_basename(opr2Sig));
      }
    }
  }
}

void checkConditExpr(map<int, map<string, string> > & table, ivl_expr_t expr)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_expr_lineno(expr);
  const char *file = ivl_expr_file(expr);
  switch (ivl_expr_type(expr))
  {
    case IVL_EX_SIGNAL:
    {
      rule = 1019;
      if (table[rule][sAct] == "yes")
      {
        unsigned nBit = ivl_expr_width(expr);
        if (nBit > 1)
        {
          printViolation(rule, line, file, nBit);
        }
      }
    }
    break;
    case IVL_EX_NUMBER:
    {
      const char *exprBits = ivl_expr_bits(expr);
      rule = 1050;
      if (table[rule][sAct] == "yes")
      {
        if (strchr(exprBits, 'z') || strchr(exprBits, 'Z'))
        {
          printViolation(rule, line, file, "z or Z");
        }
        if (strchr(exprBits, 'x') || strchr(exprBits, 'X'))
        {
          printViolation(rule, line, file, "x or X");
        }
      }
      rule = 1074;
      if (table[rule][sAct] == "yes")
      {
	// Can not implement rule 1074. No trace of -ve
        //printViolation(rule, line, file, exprBits);
      }
    }
    break;
    case IVL_EX_UNARY:
    {
      ivl_expr_t unExpr = ivl_expr_oper1(expr);
      if (ivl_expr_opcode(expr) == '&')
      {
        if (ivl_expr_width(unExpr) == 1)
        {
          const char *aSigName = NULL;
          if (ivl_expr_type(unExpr) == IVL_EX_SIGNAL)
          {
            ivl_signal_t aSig = ivl_expr_signal(unExpr);
            aSigName = ivl_signal_basename(aSig);
          }
	  else
          {
            aSigName = ivl_expr_name(unExpr);
          }
          rule = 1177;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, aSigName);
          }
        }
      }
      checkConditExpr(table, unExpr);
    }
    break;
    case IVL_EX_BINARY:
    {
      if (ivl_expr_opcode(expr) == 'n')
      {
        rule = 1030; // same as 1180, not implemented
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
      else
      {
        checkConditExpr(table, ivl_expr_oper1(expr));
        checkConditExpr(table, ivl_expr_oper2(expr));
      }
    }
    break;
    case IVL_EX_TERNARY:
    {
      rule = 1142;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file);
      }
      ivl_expr_t opr3rd = ivl_expr_oper3(expr);
      if (ivl_expr_type(opr3rd) == IVL_EX_NUMBER)
      {
        const char *someBits = ivl_expr_bits(opr3rd);
      }
    }
    break;
    case IVL_EX_SELECT:
    {
      checkVectorIndexSufficient(table, expr);
    }
    break;
    default:
    {
    }
    break;
  }
}

void checkConditClauses(map<int, map<string, string> > & table, ivl_statement_t cls, set<ivl_signal_t> *sigLst, set<ivl_signal_t> *lhSigs)
{
  int rule = 1210; // also 1214
  const char *sAct = "active";
  int line = ivl_stmt_lineno(cls);
  const char *file = ivl_stmt_file(cls);

  ivl_statement_t tCls = ivl_stmt_cond_true(cls);
  ivl_statement_t fCls = ivl_stmt_cond_false(cls);
  if (!tCls)
  {
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file, "IF");
    }
  }
  if (!fCls)
  {
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file, "ELSE");
    }
  }

  if (tCls && (ivl_statement_type(tCls) == IVL_ST_FORCE ||
               ivl_statement_type(tCls) == IVL_ST_ASSIGN ||
               ivl_statement_type(tCls) == IVL_ST_RELEASE ||
               ivl_statement_type(tCls) == IVL_ST_CASSIGN ||
               ivl_statement_type(tCls) == IVL_ST_DEASSIGN ||
               ivl_statement_type(tCls) == IVL_ST_ASSIGN_NB))
  {
    line = ivl_stmt_lineno(tCls);
    file = ivl_stmt_file(tCls);
    for (unsigned idx = 0 ;  idx < ivl_stmt_lvals(tCls) ;  idx++)
    {
      ivl_lval_t tLvl = ivl_stmt_lval(tCls, idx);
      if (tLvl)
      {
        ivl_signal_t tLvlSig = ivl_lval_sig(tLvl);
        if (tLvlSig)
        {
          const char *tLvlSigName = ivl_signal_basename(tLvlSig);
          if (fCls && (ivl_statement_type(fCls) == IVL_ST_FORCE ||
                       ivl_statement_type(fCls) == IVL_ST_ASSIGN ||
                       ivl_statement_type(fCls) == IVL_ST_RELEASE ||
                       ivl_statement_type(fCls) == IVL_ST_CASSIGN ||
                       ivl_statement_type(fCls) == IVL_ST_DEASSIGN ||
                       ivl_statement_type(fCls) == IVL_ST_ASSIGN_NB))
          {
            int foundSigInFalseCls = 0;
            for (unsigned jdx = 0 ;  jdx < ivl_stmt_lvals(fCls) ;  jdx++)
            {
              ivl_lval_t fLvl = ivl_stmt_lval(fCls, jdx);
              if (fLvl)
              {
                ivl_signal_t fLvlSig = ivl_lval_sig(fLvl);
                if (fLvlSig == tLvlSig)
                {
                  foundSigInFalseCls = 1;
                  if (ivl_statement_type(tCls) != ivl_statement_type(fCls))
                  {
		    // this is same as 1051. So 1051 not implemented
                    rule = 1025;
                    if (table[rule][sAct] == "yes")
                    {
                      printViolation(rule, line, file, tLvlSigName);
                    }
                  }
                }
              }
            }
            if (!foundSigInFalseCls)
            {
              rule = 1022;
              if (table[rule][sAct] == "yes")
              {
                printViolation(rule, line, file, tLvlSigName);
              }
            }
          }
          else
          {
            rule = 1022;
            if (table[rule][sAct] == "yes")
            {
              printViolation(rule, line, file, tLvlSigName);
            }
          }
        }
      }
    }
  }
  if (tCls && fCls && ivl_statement_type(fCls) == IVL_ST_CONDIT)
  {
    if (ivl_stmt_cond_true(fCls))
    {
      if (ivl_stmt_cond_false(fCls))
      {
        // nested if-else-if
        rule = 1049;
        printViolation(rule, line, file);
      }
    }
  }

  ivl_statement_t condStmt = cls;
  while (condStmt && ivl_statement_type(condStmt) == IVL_ST_CONDIT)
  {
    ivl_expr_t fExpr = ivl_stmt_cond_expr(condStmt);
    checkConditExpr(table, fExpr); // not a full proof way for 1050 and 1019
    checkUnsignedVector(table, fExpr); // not a full proof way for 1050 and 1019
    condStmt = ivl_stmt_cond_false(condStmt);
  }

  if (tCls)
  {
    checkProcesStatement(table, tCls, sigLst, lhSigs);
  }
  if (fCls)
  {
    checkProcesStatement(table, fCls, sigLst, lhSigs);
  }
}

void checkMemoryisReadandWrittenatSameTime(map<int, map<string, string>> &table, ivl_statement_t net, set<ivl_signal_t> *lhSigs)
{
  traverseExpression(table, net, NULL, lhSigs, NULL);
}

void checkBlockStatements(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigLst, set<ivl_signal_t> *sigSet, bool edge)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);

  set<ivl_signal_t> *forceSigs = NULL;
  set<ivl_signal_t> *asgnSigs = NULL;
  bool blkStmtFound = false;
  bool nblkStmtFound = false;
  int moreIfCaseForRepWhileStmt = 0;
  unsigned noStmt = ivl_stmt_block_count(net);
  for (unsigned idx = 0; idx < noStmt; idx++)
  {
    ivl_statement_t aStmt = ivl_stmt_block_stmt(net, idx);
    switch (ivl_statement_type(aStmt))
    {
      case IVL_ST_CASSIGN:
      {
        ProceduralContinuousAssignmentNotSynthesizable(table, aStmt); 
        SignalAssignedToSelf(table, aStmt, sigLst, sigSet);
        if (!asgnSigs)
        {
          asgnSigs = new set<ivl_signal_t>;
	}
        for (unsigned idx = 0; idx < ivl_stmt_lvals(aStmt); idx++)
        {
          ivl_signal_t lhSig = ivl_lval_sig(ivl_stmt_lval(aStmt, idx));
          if (lhSig && asgnSigs->find(lhSig) == asgnSigs->end())
          {
            asgnSigs->insert(lhSig);
          }
        }
      }
      break;
      case IVL_ST_DEASSIGN:
      {
        checkDeassign(table, aStmt, asgnSigs);
      }
      break;
      case IVL_ST_ASSIGN:
      case IVL_ST_ASSIGN_NB:
      {
        checkDirectInputOutput(table, aStmt);
        SignalAssignedToSelf(table, aStmt, sigLst, sigSet);
        DelayControl(table, aStmt, sigLst, sigSet);
        checkNetStuck(table, aStmt);
        checkIntegerNegative(table, aStmt);
        checkNonConstShiftAmt(table, ivl_stmt_rval(aStmt), true);
        if (ivl_statement_type(aStmt) == IVL_ST_ASSIGN)
          blkStmtFound = true;
	else
          nblkStmtFound = true;
      }
      break;
      case IVL_ST_CONDIT:
      {
        moreIfCaseForRepWhileStmt++;
        checkConditExpr(table, ivl_stmt_cond_expr(aStmt));
        checkUnsignedVector(table, ivl_stmt_cond_expr(aStmt));
        checkConditClauses(table, aStmt, sigLst, sigSet);
      }
      break;
      case IVL_ST_RELEASE:
      {
        ReleaseStatement(table, aStmt, forceSigs);
      }
      break;
      case IVL_ST_FORCE:
      {
	if (!forceSigs)
        {
	  forceSigs = new set<ivl_signal_t>;
        }
        ForceStatement(table, aStmt, forceSigs);
      }
      break;
      case IVL_ST_UTASK:
      case IVL_ST_DISABLE:
      {
        checkUserTask(table, aStmt);
      }
      break;
      case IVL_ST_STASK:
      {
        SystemTaskCall(table, aStmt);
      }
      break;
      case IVL_ST_DELAY:
      case IVL_ST_DELAYX:
      {
        DelayControl(table, aStmt, sigLst, sigSet);
      }
      break;
      case IVL_ST_WHILE:
      case IVL_ST_DO_WHILE:
      {
        moreIfCaseForRepWhileStmt++;
        WhileLoop(table, aStmt);
      }
      break;
      case IVL_ST_WAIT:
      {
        // @ (something) is not allowed inside a block
	checkNestedEvents(table, aStmt); // false
      }
      break;
      case IVL_ST_NOOP:
      case IVL_ST_REPEAT:
      {
        moreIfCaseForRepWhileStmt++;
        // if repeat statement has z or x in the value then it becomes NOOP
        checkRepeatExpression(table, aStmt);
      }
      break;
      case IVL_ST_TRIGGER:
      {
        rule = 1083;
        if (table[rule][sAct] == "yes")
        {
          int aline = ivl_stmt_lineno(aStmt);
          const char *afile = ivl_stmt_file(aStmt);
          printViolation(rule, aline, afile, ivl_event_basename(ivl_stmt_events(aStmt, 0)));
        }
      }
      break;
      default:
      {
        moreIfCaseForRepWhileStmt++;
      }
      break;
    }
  }
  for (unsigned idx = 0; idx < noStmt; idx++)
  {
    ivl_statement_t aStmt = ivl_stmt_block_stmt(net, idx);
    switch (ivl_statement_type(aStmt))
    {
      case IVL_ST_ASSIGN:
      case IVL_ST_CASSIGN:
      case IVL_ST_ASSIGN_NB:
      {
        checkMemoryisReadandWrittenatSameTime(table, aStmt, sigSet);
      }
      break;
      default:
      {
      }
      break;
    }
  }

  if (nblkStmtFound)
  {
    if (blkStmtFound)
    {
      rule = 1029;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file);
      }
    }
    else
    {
      rule = 1073;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file);
      }
    }
  }

  if (forceSigs && !forceSigs->empty())
  {
    rule = 1055;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
    delete forceSigs;
  }

  if (asgnSigs && !asgnSigs->empty())
  {
    rule = 1061;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
    delete asgnSigs;
  }

  if (noStmt > 10)
  {
    rule = 1071;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file, noStmt);
    }
  }

  if (moreIfCaseForRepWhileStmt > 1)
  {
    rule = 1085;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }
}

void checkProcesStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sensitivityList, set<ivl_signal_t> *lhSigs, bool edge, bool firsTime)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);

  ivl_statement_type_t code = ivl_statement_type(net);
  switch (code)
  { 
    case IVL_ST_WAIT:
    {
      for (unsigned idx = 0; idx < ivl_stmt_nevent(net); idx++)
      {
        ivl_event_t anEv = ivl_stmt_events(net, idx);
        edge |= checkEvent(table, anEv, sensitivityList);
      }
      if (edge)
        checkComboInSequential(table, ivl_stmt_sub_stmt(net));
      checkNestedEvents(table, net, firsTime);
      checkProcesStatement(table, ivl_stmt_sub_stmt(net), sensitivityList, lhSigs, edge, false);
    }
    break; 
    case IVL_ST_CONDIT:
    {
      checkConditClauses(table, net, sensitivityList, lhSigs);
    }
    break;
    case IVL_ST_BLOCK:
    {
      MissingProcessLabelName(table, net);
      checkBlockStatements(table, net, sensitivityList, lhSigs, edge);
    }
    break; 
    case IVL_ST_CASE:
    {
      checkCaseLabels(table, net, sensitivityList, lhSigs);
    }
    break; 
    case IVL_ST_CASEX:
    case IVL_ST_CASEZ:
    {
      checkCaseXZ(table, net);  
      checkCaseLabels(table, net, sensitivityList, lhSigs);
    }
    break; 
    case IVL_ST_ASSIGN:
    case IVL_ST_ASSIGN_NB:
    {
      DelayControl(table, net, sensitivityList, lhSigs);
      checkConditExpr(table, ivl_stmt_rval(net));
      checkDirectInputOutput(table, net);
      SignalAssignedToSelf(table, net, sensitivityList, lhSigs);
      checkNetStuck(table, net);
      checkUnsignedVector(table, ivl_stmt_rval(net));
      checkNonConstShiftAmt(table, ivl_stmt_rval(net), true);
      checkIntegerNegative(table, net);
      for (unsigned idx = 0; idx < ivl_stmt_lvals(net); idx++)
      {
        rule = 1045;
        ivl_signal_t lhSig = ivl_lval_sig(ivl_stmt_lval(net, idx));
        if (lhSig && (ivl_signal_width(lhSig) != ivl_lval_width(ivl_stmt_lval(net, idx))))
        {
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, ivl_signal_basename(lhSig));
          }
        }
      }
    }
    break; 
    case IVL_ST_CASSIGN:
    {
      ProceduralContinuousAssignmentNotSynthesizable(table, net); 
      SignalAssignedToSelf(table, net, sensitivityList, lhSigs);
    }
    break;
    case IVL_ST_DELAY:
    case IVL_ST_DELAYX:
    {
      DelayControl(table, net, sensitivityList, lhSigs);
    }
    break;
    case IVL_ST_UTASK:
    case IVL_ST_DISABLE:
    {
      checkUserTask(table, net);
    }
    break;
    case IVL_ST_STASK:
    {
      SystemTaskCall(table, net);
    }
    break;
    case IVL_ST_RELEASE:
    {
      ReleaseStatement(table, net);
    }
    break;
    case IVL_ST_FORCE:
    {
      ForceStatement(table, net);
    }
    break;
    case IVL_ST_WHILE:
    case IVL_ST_DO_WHILE:
    {
      WhileLoop(table, net);
    }
    break;
    case IVL_ST_NOOP:
    case IVL_ST_REPEAT:
    {
      // if repeat statement has z or x in the value then it becomes NOOP
      checkRepeatExpression(table, net);
    }
    break;
    case IVL_ST_FORK:
    {
      ivl_scope_t forkScp = ivl_stmt_block_scope(net);
      if (!forkScp)
      {
        int rule = 1186;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, NULL);
        }
      }
    }
    break;
    case IVL_ST_TRIGGER:
    {
      rule = 1083;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, ivl_event_basename(ivl_stmt_events(net, 0)));
      }
    }
    break;
    default:
    {
    }
    break;
  } 
}
