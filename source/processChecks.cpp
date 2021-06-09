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

void functionCalledInAnAlwaysBlock(map<int, map<string, string>> &table, ivl_expr_t fExp)
{
  int rule = 1230;
  const char *sAct = "active";
  int line = ivl_expr_lineno(fExp);
  const char *file = ivl_expr_file(fExp); 

  if (table[rule][sAct] == "yes")
  {
    if (ivl_expr_type(fExp) == IVL_EX_UFUNC)
    {
      ivl_scope_t func = ivl_expr_def(fExp);
      printViolation(rule, line, file, ivl_scope_basename(func));
    }
  }
}

void checkNonConstShiftAmt(map<int, map<string, string>> &table, ivl_expr_t expr, bool firsTime)
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
      rule = 1121;
      if (rule && table[rule][sAct] == "yes")
      {
        if (!ivl_signal_dimensions(opr1Sig))
        {
          printViolation(rule, line, file, ivl_signal_basename(opr2Sig));
        }
      }
    }
  }
}

void traverseExpression(map<int, map<string, string> > & table, ivl_expr_t anExpr, ivl_signal_t lvSig, ivl_signal_t &loopVar, set<ivl_signal_t> &lhSigs, set<ivl_signal_t> &sigLst, bool first = true)
{
  int rule = 0;
  const char *sAct = "active";
  int line = anExpr ? ivl_expr_lineno(anExpr) : 0;
  const char *file = anExpr ? ivl_expr_file(anExpr) : NULL;
  ivl_expr_t opr1 = NULL;
  ivl_expr_t opr2 = NULL;
  ivl_expr_t opr3 = NULL;
  switch (ivl_expr_type(anExpr))
  {
    case IVL_EX_SIGNAL:
    {
      ivl_signal_t expSig = ivl_expr_signal(anExpr);
      if (expSig)
      {
        const char *expSigName = ivl_signal_basename(expSig);
	if (first)
	{
          if (loopVar)
	  {
            rule = 1242;
            if (table[rule][sAct] == "yes")
            {
              printViolation(rule, line, file, expSigName);
            }
	  }
          else
	  {
            loopVar = expSig;
	  }
          rule = 1075;
          if (table[rule][sAct] == "yes")
          {
            if (expSig == lvSig)
            {
              printViolation(rule, line, file, expSigName);
            }
          }
          rule = 1081; // same as 1159, not implemented
          if (table[rule][sAct] == "yes")
          {
            if (!sigLst.empty() && (sigLst.find(expSig) == sigLst.end()))
            {
              printViolation(rule, line, file, expSigName);
            }
          }
          rule = 1204;
          if (table[rule][sAct] == "yes")
          {
            if (ivl_signal_port(expSig) == IVL_SIP_NONE)// && (ivl_statement_type(net) == IVL_ST_ASSIGN_NB))
            {
              printViolation(rule, line, file, expSigName);
            }
          }
          rule = 1206;
          if (table[rule][sAct] == "yes")
          {
            if (lhSigs.find(expSig) != lhSigs.end())
            {
              printViolation(rule, line, file, expSigName);
            }
          }
	  if (lvSig)
	  {
            const char *lvSigName = ivl_signal_basename(lvSig);
            rule = 1128;
            if (table[rule][sAct] == "yes")
            {
              if (ivl_signal_signed(lvSig))
              {
                if (!ivl_signal_signed(expSig))
                {
                  printViolation(rule, line, file, expSigName, lvSigName);
                }
              }
              else
              {
                if (ivl_signal_signed(expSig))
                {
                  printViolation(rule, line, file, lvSigName, expSigName);
                }
              }
            }
            rule = 1240;
            if (table[rule][sAct] == "yes")
            {
              unsigned lvSigWidth = ivl_signal_width(lvSig);
              unsigned expSigWidth = ivl_signal_width(expSig);
    	    if (lvSigWidth != expSigWidth)
  	    {
                printViolation(rule, line, file, lvSigName, lvSigWidth, expSigName, expSigWidth);
              }
            }
          }
          rule = 1019; // same as 1224, not implemnted
          if (table[rule][sAct] == "yes")
          {
            unsigned nBit = ivl_expr_width(anExpr);
            if (nBit > 1)
            {
              printViolation(rule, line, file, nBit);
            }
          }
	}
	else
	{
          rule = 1231;
          if (table[rule][sAct] == "yes")
          {
            if (lhSigs.find(expSig) == lhSigs.end())
            {
              if (ivl_signal_port(expSig) == IVL_SIP_NONE)
              {
                printViolation(rule, line, file, expSigName);
              }
            }
          }
          rule = 1063;
          if (table[rule][sAct] == "yes")
          {
	    if (lvSig)
	    {
              const char *lvSigName = ivl_signal_basename(lvSig);
              if (lhSigs.find(lvSig) != lhSigs.end())
              {
                printViolation(rule, line, file, lvSigName);
              }
            }
          }
	}
      }
    }
    break;
    case IVL_EX_NUMBER:
    {
      if (first)
      {
        rule = 1201;
        if (table[rule][sAct] == "yes")
        {
          const char *valBits = ivl_expr_bits(anExpr);
          if (strchr(valBits, 'x') || strchr(valBits, 'X'))
          {
            printViolation(rule, line, file);
          }
        }
        rule = 1050;
        if (table[rule][sAct] == "yes")
        {
          const char *exprBits = ivl_expr_bits(anExpr);
          if (strchr(exprBits, 'z') || strchr(exprBits, 'Z'))
          {
            printViolation(rule, line, file, "z or Z");
          }
          if (strchr(exprBits, 'x') || strchr(exprBits, 'X'))
          {
            printViolation(rule, line, file, "x or X");
          }
        }
      }
    }
    break;
    case IVL_EX_STRING:
    {
      if (first)
      {
        rule = 1082;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_expr_string(anExpr));
        }
      }
    }
    break;
    case IVL_EX_UNARY:
    {
      opr1 = ivl_expr_oper1(anExpr);
      if (first)
      {
        if (ivl_expr_opcode(anExpr) == '&')
        {
          if (ivl_expr_width(opr1) == 1)
          {
            const char *aSigName = NULL;
            if (ivl_expr_type(opr1) == IVL_EX_SIGNAL)
            {
              ivl_signal_t aSig = ivl_expr_signal(opr1);
              aSigName = ivl_signal_basename(aSig);
            }
            else
            {
              aSigName = ivl_expr_name(opr1);
            }
            rule = 1177;
            if (table[rule][sAct] == "yes")
            {
              printViolation(rule, line, file, aSigName);
            }
          }
        }
      }
      traverseExpression(table, opr1, lvSig, loopVar, lhSigs, sigLst, first);
    }
    break;
    case IVL_EX_SELECT:
    {
      if (first)
      {
        checkVectorIndexSufficient(table, anExpr);
      }
      opr1 = ivl_expr_oper1(anExpr);
      traverseExpression(table, opr1, lvSig, loopVar, lhSigs, sigLst, first);
    }
    break;
    case IVL_EX_BINARY:
    {
      if (first)
      {
        rule = 1202;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_expr_opcode(anExpr));
        }
        if (ivl_expr_opcode(anExpr) == 'E')
        {
          rule = 1251;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file);
          }
        }
        if (ivl_expr_opcode(anExpr) == 'n')
        {
          rule = 1030; // same as 1180, not implemented
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file);
          }
        }
      }
      opr1 = ivl_expr_oper1(anExpr);
      traverseExpression(table, opr1, lvSig, loopVar, lhSigs, sigLst, first);
      opr2 = ivl_expr_oper2(anExpr);
      traverseExpression(table, opr2, lvSig, loopVar, lhSigs, sigLst, first);
    }
    break;
    case IVL_EX_TERNARY:
    {
      if (first)
      {
        rule = 1202;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, '?');
        }
        rule = 1142;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
      opr1 = ivl_expr_oper1(anExpr);
      traverseExpression(table, opr1, lvSig, loopVar, lhSigs, sigLst, first);
      opr2 = ivl_expr_oper2(anExpr);
      traverseExpression(table, opr2, lvSig, loopVar, lhSigs, sigLst, first);
      opr3 = ivl_expr_oper3(anExpr);
      traverseExpression(table, opr3, lvSig, loopVar, lhSigs, sigLst, first);
    }
    break;
    default:
    {
    }
    break;
  }
}

void SignalAssignedToSelf(map<int, map<string, string> > & table, ivl_statement_t net, ivl_signal_t loopVar, set<ivl_signal_t> &sigLst, set<ivl_signal_t> &lhSigs)
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
      if (lhSigs.find(lvSig) == lhSigs.end())
      {
        lhSigs.insert(lvSig);
      }
      else
      {
        rule = 1077; // same as 1203, not implemented
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, lvSigName);
        }
      }
      if (lvSig == loopVar)
      {
        rule = 1243;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, lvSigName);
        }
      }
      loopVar = NULL;
      ivl_expr_t rvExpr = ivl_stmt_rval(net);
      traverseExpression(table, rvExpr, lvSig, loopVar, lhSigs, sigLst);
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

  rule = 1064;
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

void checkCaseLabels(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> &sigLst, set<ivl_signal_t> &lhSigs)
{
  const char *sAct = "active";

  ivl_expr_t casCondExpr = ivl_stmt_cond_expr(net);
  int line = ivl_expr_lineno(casCondExpr);
  const char *file = ivl_expr_file(casCondExpr);

  int rule = 1064;
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

  unsigned casCnt = ivl_stmt_case_count(net);
  unsigned casExpWidth = ivl_expr_width(casCondExpr);
  if (casCnt < pow(2, casExpWidth))
  {
    rule = 1221;
    if (table[rule][sAct] == "yes")
    {
      printViolation(rule, line, file);
    }
  }

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
      opr1 = NULL;
    }
    else
    {
      casCondExpr = opr1;
      opr1 = (ivl_expr_type(casCondExpr) != IVL_EX_NUMBER) ? ivl_expr_oper1(opr1) : NULL;
    }
  }

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
        if (lblExpWidth >= pow(2, operWidth))
        {
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, operWidth);
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
      case IVL_EX_REALNUM:
      {
        rule = 1018;
        line = ivl_expr_lineno(lblExp);
        file = ivl_expr_file(lblExp);
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
      break;
      default:
      {
        if (casCnt > pow(2, ivl_expr_width(casCondExpr)))
        {
          rule = 1219;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file);
          }
        }
	else
        {
          rule = 1222;
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
    ivl_signal_t dummy = NULL;
    checkProcesStatement(table, ivl_stmt_case_stmt(net, idx), dummy, sigLst, lhSigs);
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

bool checkEvent(map<int, map<string, string> > & table, ivl_event_t & evt, set<ivl_signal_t> &sigLst)
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
        if (sigLst.find(aConnSig) == sigLst.end())
        {
          sigLst.insert(aConnSig);
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
        if (sigLst.find(aConnSig) == sigLst.end())
        {
          sigLst.insert(aConnSig);
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
        if (sigLst.find(aConnSig) == sigLst.end())
        {
          sigLst.insert(aConnSig);
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
      rule = 1019; // same as 1224, not implemnted
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
      if (ivl_expr_opcode(expr) == 'E')
      {
        rule = 1251;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
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

void checkIfElse(ivl_statement_t cls, map<ivl_signal_t, ivl_statement_t> &lhSigs)
{
  ivl_statement_type_t stmtTyp = ivl_statement_type(cls);
  if ((stmtTyp == IVL_ST_FORCE) ||
      (stmtTyp == IVL_ST_ASSIGN) ||
      (stmtTyp == IVL_ST_RELEASE) ||
      (stmtTyp == IVL_ST_CASSIGN) ||
      (stmtTyp == IVL_ST_DEASSIGN) ||
      (stmtTyp == IVL_ST_ASSIGN_NB))
  {
    for (unsigned idx = 0 ;  idx < ivl_stmt_lvals(cls) ;  idx++)
    {
      ivl_lval_t tLvl = ivl_stmt_lval(cls, idx);
      if (tLvl)
      {
        ivl_signal_t tLvlSig = ivl_lval_sig(tLvl);
        if (tLvlSig)
        {
          lhSigs[tLvlSig] = cls;
        }
      }
    }
  }
}

void checkConditClauses(map<int, map<string, string> > & table, ivl_statement_t cls, set<ivl_signal_t> &sigLst, set<ivl_signal_t> &lhSigs)
{
  int rule = 1210; // also 1214
  const char *sAct = "active";
  int line = ivl_stmt_lineno(cls);
  const char *file = ivl_stmt_file(cls);

  ivl_statement_t tCls = ivl_stmt_cond_true(cls);
  ivl_statement_t fCls = ivl_stmt_cond_false(cls);
  if (table[rule][sAct] == "yes")
  {
    if (!tCls)
    {
      printViolation(rule, line, file, "IF");
    }
    if (!fCls)
    {
      printViolation(rule, line, file, "ELSE");
    }
  }

  rule = 1049;
  if (table[rule][sAct] == "yes")
  {
    if (fCls)
    {
      if (ivl_statement_type(fCls) == IVL_ST_CONDIT)
      {
        if (ivl_stmt_cond_true(fCls))
        {
          if (ivl_stmt_cond_false(fCls))
          {
            // nested if-else-if
            printViolation(rule, line, file);
          }
        }
      }
    }
  }

  bool eventAtIf = false;
  bool eventAtElse = false;
  map<ivl_signal_t, ivl_statement_t> ifLhSigs;
  if (tCls)
  {
    if (ivl_statement_type(tCls) == IVL_ST_BLOCK)
    {
      unsigned noStmt = ivl_stmt_block_count(tCls);
      for (unsigned idx = 0; idx < noStmt; idx++)
      {
        ivl_statement_t aStmt = ivl_stmt_block_stmt(tCls, idx);
        if (ivl_statement_type(aStmt) == IVL_ST_WAIT)
        {
          line = ivl_stmt_lineno(aStmt);
          file = ivl_stmt_file(aStmt);
          eventAtIf = true;
        }
	else
	{
          checkIfElse(aStmt, ifLhSigs);
        }
      }
    }
    else if (ivl_statement_type(tCls) == IVL_ST_WAIT)
    {
      line = ivl_stmt_lineno(tCls);
      file = ivl_stmt_file(tCls);
      eventAtIf = true;
    }
    else
    {
      checkIfElse(tCls, ifLhSigs);
    }
  }
  map<ivl_signal_t, ivl_statement_t> elseLhSigs;
  if (fCls)
  {
    if (ivl_statement_type(fCls) == IVL_ST_BLOCK)
    {
      unsigned noStmt = ivl_stmt_block_count(fCls);
      for (unsigned idx = 0; idx < noStmt; idx++)
      {
        ivl_statement_t aStmt = ivl_stmt_block_stmt(fCls, idx);
        if (ivl_statement_type(aStmt) == IVL_ST_WAIT)
        {
          line = ivl_stmt_lineno(aStmt);
          file = ivl_stmt_file(aStmt);
          eventAtElse = true;
        }
	else
	{
          checkIfElse(aStmt, elseLhSigs);
        }
      }
    }
    else if (ivl_statement_type(fCls) == IVL_ST_WAIT)
    {
      line = ivl_stmt_lineno(fCls);
      file = ivl_stmt_file(fCls);
      eventAtElse = true;
    }
    else
    {
      checkIfElse(fCls, elseLhSigs);
    }
  }

  rule = 1113;
  if (table[rule][sAct] == "yes")
  {
    if (eventAtIf != eventAtElse)
    {
      printViolation(rule, line, file);
    }
  }

  map<ivl_signal_t, ivl_statement_t>::iterator itr;
  if (tCls)
  {
    line = ivl_stmt_lineno(tCls);
    file = ivl_stmt_file(tCls);
    for (itr = ifLhSigs.begin(); itr != ifLhSigs.end(); itr++)
    {
      ivl_signal_t ifSig = itr->first;
      if (elseLhSigs.find(ifSig) == elseLhSigs.end())
      {
        rule = 1022;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_signal_basename(ifSig));
        }
      }
      else
      {
        rule = 1025;
        if (table[rule][sAct] == "yes")
        {
          ivl_statement_t ifStmt = ifLhSigs[ifSig];
          ivl_statement_t elStmt = elseLhSigs[ifSig];
          if (ivl_statement_type(ifStmt) != ivl_statement_type(elStmt))
          {
            printViolation(rule, line, file, ivl_signal_basename(ifSig));
          }
        }
      }
    }
  }
  if (fCls)
  {
    line = ivl_stmt_lineno(fCls);
    file = ivl_stmt_file(tCls);
    for (itr = elseLhSigs.begin(); itr != elseLhSigs.end(); itr++)
    {
      ivl_signal_t elSig = itr->first;
      if (ifLhSigs.find(elSig) == ifLhSigs.end())
      {
        rule = 1022;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, ivl_signal_basename(elSig));
        }
      }
    }
  }

  ivl_signal_t lpIdx = NULL;
  if (tCls)
  {
    checkProcesStatement(table, tCls, lpIdx, sigLst, lhSigs);
  }
  if (fCls)
  {
    checkProcesStatement(table, fCls, lpIdx, sigLst, lhSigs);
  }
}

void checkMemoryisReadandWrittenatSameTime(map<int, map<string, string>> &table, ivl_statement_t net, set<ivl_signal_t> &lhSigs)
{
  set<ivl_signal_t> dummy;
  ivl_signal_t mummy = NULL;
  traverseExpression(table, ivl_stmt_rval(net), mummy, mummy, lhSigs, dummy);
}

void checkBlockStatements(map<int, map<string, string>> &table, ivl_statement_t net, ivl_signal_t loopVar, set<ivl_signal_t> &sigLst, set<ivl_signal_t> &sigSet, bool edge)
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
    ivl_statement_type_t stmtTyp = ivl_statement_type(aStmt);
    switch (stmtTyp)
    {
      case IVL_ST_CASSIGN:
      {
        ProceduralContinuousAssignmentNotSynthesizable(table, aStmt); 
        SignalAssignedToSelf(table, aStmt, loopVar, sigLst, sigSet);
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
        ivl_expr_t rhs = ivl_stmt_rval(aStmt);
        checkDirectInputOutput(table, aStmt);
        SignalAssignedToSelf(table, aStmt, loopVar, sigLst, sigSet);
        DelayControl(table, aStmt, sigLst, sigSet);
        checkNetStuck(table, aStmt);
        checkIntegerNegative(table, aStmt);
        checkNonConstShiftAmt(table, rhs, true);
        functionCalledInAnAlwaysBlock(table, rhs);
        if (stmtTyp == IVL_ST_ASSIGN)
          blkStmtFound = true;
	else
          nblkStmtFound = true;
      }
      break;
      case IVL_ST_CONDIT:
      {
        moreIfCaseForRepWhileStmt++;
        ivl_signal_t lvSigDum = NULL;
        ivl_signal_t loopIdx = NULL;
        ivl_expr_t condExpr = ivl_stmt_cond_expr(aStmt);
        traverseExpression(table, condExpr, lvSigDum, loopIdx, sigSet, sigLst);
        checkUnsignedVector(table, condExpr);
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
        ivl_signal_t lvSigDum = NULL;
        ivl_signal_t loopIdx = NULL;
        ivl_expr_t loopCond = ivl_stmt_cond_expr(aStmt);
        traverseExpression(table, loopCond, lvSigDum, loopIdx, sigSet, sigLst);
        ivl_statement_t loopStmt = ivl_stmt_sub_stmt(aStmt);
        checkProcesStatement(table, loopStmt, loopIdx, sigLst, sigSet);
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
      case IVL_ST_CASER:
      {
        checkCaseLabels(table, aStmt, sigLst, sigSet);
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

int checkWidthRHS(ivl_expr_t rhsExpr, char op, bool firsTime = false)
{
  static int width;
  if (firsTime)
    width = 0;

  switch (ivl_expr_type(rhsExpr))
  {
    case IVL_EX_UNARY:
    {
      checkWidthRHS(ivl_expr_oper1(rhsExpr), ivl_expr_opcode(rhsExpr));
    }
    break;
    case IVL_EX_SELECT:
    {
      checkWidthRHS(ivl_expr_oper1(rhsExpr), op);
    }
    break;
    case IVL_EX_BINARY:
    {
      char code = ivl_expr_opcode(rhsExpr);
      checkWidthRHS(ivl_expr_oper1(rhsExpr), code);
      checkWidthRHS(ivl_expr_oper2(rhsExpr), code);
    }
    break;
    case IVL_EX_TERNARY:
    {
      checkWidthRHS(ivl_expr_oper1(rhsExpr), '?');
      checkWidthRHS(ivl_expr_oper2(rhsExpr), '?');
      checkWidthRHS(ivl_expr_oper3(rhsExpr), '?');
    }
    break;
    case IVL_EX_NUMBER:
    {
      const char *numBits = ivl_expr_bits(rhsExpr);
      int constWidth = strlen(numBits);
      numBits = strrchr(numBits, '1');
      if (numBits)
        constWidth = constWidth - strlen(numBits) + 1;
      if (op == '*')
      {
          width += constWidth;
      }
      if (constWidth > width)
      {
        if (op == '+')
          width = constWidth + 1;
        else
          width = constWidth;
      }

    }
    break;
    default:
    {
      if (op == '*')
      {
          width += ivl_expr_width(rhsExpr);
      }
      if (ivl_expr_width(rhsExpr) > width)
      {
        if (op == '+')
          width = ivl_expr_width(rhsExpr) + 1;
        else
          width = ivl_expr_width(rhsExpr);
      }
    }
    break;
  }
  return width;
}

void checkBothSides(map<int, map<string, string>> &table, ivl_statement_t net)
{
  int rule = 1045;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(net);
  const char *file = ivl_stmt_file(net);

  int lhsWidth = 0;
  for (unsigned idx = 0; idx < ivl_stmt_lvals(net); idx++)
  {
    lhsWidth += ivl_lval_width(ivl_stmt_lval(net, idx));
    ivl_signal_t lhSig = ivl_lval_sig(ivl_stmt_lval(net, idx));
    if (lhSig && (ivl_signal_width(lhSig) != ivl_lval_width(ivl_stmt_lval(net, idx))))
    {
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, ivl_signal_basename(lhSig));
      }
    }
  }
  rule = 1109;
  if (table[rule][sAct] == "yes")
  {
    ivl_expr_t rhs = ivl_stmt_rval(net);
    int rhsWidth = checkWidthRHS(rhs, 0, true);
    if (rhsWidth > lhsWidth)
    {
      printViolation(rule, line, file, lhsWidth);
    }
  }
}

void checkProcesStatement(map<int, map<string, string>> &table, ivl_statement_t net, ivl_signal_t &loopVar, set<ivl_signal_t> &sensitivityList, set<ivl_signal_t> &lhSigs, bool edge, bool firsTime)
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
      ivl_statement_t procStmt = ivl_stmt_sub_stmt(net);
      if (edge)
        checkComboInSequential(table, procStmt);

      checkNestedEvents(table, net, firsTime);
      checkProcesStatement(table, procStmt, loopVar, sensitivityList, lhSigs, edge);
      if (!firsTime)
      {
        rule = 1250;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file);
        }
      }
    }
    break; 
    case IVL_ST_CONDIT:
    {
      ivl_signal_t lvSigDum = NULL;
      ivl_signal_t loopIdx = NULL;
      ivl_expr_t condExpr = ivl_stmt_cond_expr(net);
      traverseExpression(table, condExpr, lvSigDum, loopIdx, lhSigs, sensitivityList);
      checkUnsignedVector(table, condExpr);
      checkConditClauses(table, net, sensitivityList, lhSigs);
    }
    break;
    case IVL_ST_BLOCK:
    {
      MissingProcessLabelName(table, net);
      checkBlockStatements(table, net, loopVar, sensitivityList, lhSigs, edge);
    }
    break; 
    case IVL_ST_CASE:
    case IVL_ST_CASER:
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
      ivl_expr_t rhs = ivl_stmt_rval(net);
      DelayControl(table, net, sensitivityList, lhSigs);
      checkDirectInputOutput(table, net);
      SignalAssignedToSelf(table, net, loopVar, sensitivityList, lhSigs);
      checkNetStuck(table, net);
      checkNonConstShiftAmt(table, rhs, true);
      checkIntegerNegative(table, net);
      functionCalledInAnAlwaysBlock(table, rhs);
      checkBothSides(table, net);
    }
    break; 
    case IVL_ST_CASSIGN:
    {
      ProceduralContinuousAssignmentNotSynthesizable(table, net); 
      SignalAssignedToSelf(table, net, loopVar, sensitivityList, lhSigs);
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
      ivl_signal_t lvSigDum = NULL;
      ivl_signal_t loopIdx = NULL;
      ivl_expr_t loopCond = ivl_stmt_cond_expr(net);
      traverseExpression(table, loopCond, lvSigDum, loopIdx, lhSigs, sensitivityList);
      ivl_statement_t loopStmt = ivl_stmt_sub_stmt(net);
      checkProcesStatement(table, loopStmt, loopIdx, sensitivityList, lhSigs);
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

void checkUnasndVar(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> &alLhSigs)
{
  if (net)
  {
    ivl_statement_type_t code = ivl_statement_type(net);
    switch (code)
    { 
      case IVL_ST_WAIT:
      {
        ivl_statement_t aStmt = ivl_stmt_sub_stmt(net);
        checkUnasndVar(table, aStmt, alLhSigs);
      }
      break; 
      case IVL_ST_CONDIT:
      {
        ivl_statement_t tCls = ivl_stmt_cond_true(net);
        checkUnasndVar(table, tCls, alLhSigs);
        ivl_statement_t fCls = ivl_stmt_cond_false(net);
        if (fCls)
          checkUnasndVar(table, fCls, alLhSigs);
      }
      break;
      case IVL_ST_BLOCK:
      {
        unsigned noStmt = ivl_stmt_block_count(net);
        for (unsigned idx = 0; idx < noStmt; idx++)
        {
          ivl_statement_t aStmt = ivl_stmt_block_stmt(net, idx);
          checkUnasndVar(table, aStmt, alLhSigs);
        }
      }
      break; 
      case IVL_ST_CASE:
      case IVL_ST_CASEX:
      case IVL_ST_CASEZ:
      {
        unsigned casCnt = ivl_stmt_case_count(net);
        for (unsigned idx = 0; idx < casCnt; idx++) 
        {
          ivl_statement_t aStmt = ivl_stmt_case_stmt(net, idx);
          checkUnasndVar(table, aStmt, alLhSigs);
        }
      }
      break; 
      case IVL_ST_ASSIGN:
      case IVL_ST_CASSIGN:
      case IVL_ST_ASSIGN_NB:
      {
        for (unsigned idx = 0; idx < ivl_stmt_lvals(net);  idx++)
        {
          ivl_lval_t lv =  ivl_stmt_lval(net, idx);
          ivl_signal_t lvSig =  ivl_lval_sig(lv);
          if (lvSig)
          {
            set<ivl_signal_t> dummy;
            ivl_signal_t lpIdxDummy = NULL;
            traverseExpression(table, ivl_stmt_rval(net), lvSig, lpIdxDummy, alLhSigs, dummy, false);
          }
        }
      }
      break; 
      case IVL_ST_DELAY:
      case IVL_ST_DELAYX:
      {
        ivl_statement_t aStmt = ivl_stmt_sub_stmt(net);
        checkUnasndVar(table, aStmt, alLhSigs);
      }
      break;
      default:
      {
      }
      break;
    } 
  }
}
