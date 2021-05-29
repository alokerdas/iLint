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

bool isTriState(ivl_lpm_t netMux)
{
  bool triState = false;
  // e ? i2 : 1'bz type of definition will be MUX lpm
  for(unsigned j = 0 ; j < 2 ; j++)
  {
    ivl_nexus_t muxIn = ivl_lpm_data(netMux, j);
    unsigned connect = ivl_nexus_ptrs(muxIn);
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t inCon = ivl_nexus_ptr(muxIn, i);
      ivl_signal_t aSig = ivl_nexus_ptr_sig(inCon);
      if(aSig && ivl_signal_local(aSig) && (connect == 2))
        triState = true;
    }
  }
  return triState;
}

void checkTristateBuffers(map<int, map<string, string> > & table, ivl_lpm_t & net)
{
  int rule = 0;
  const char *sAct = "active";
  unsigned line = ivl_lpm_lineno(net);
  const char *file = ivl_lpm_file(net);
  if (ivl_lpm_type(net) == IVL_LPM_MUX)
  {
    const char *outSigName = NULL;
    ivl_nexus_t muxOut = ivl_lpm_q(net);
    ivl_nexus_ptr_t outConn = ivl_nexus_ptr(muxOut, 0);
    ivl_signal_t outSig = ivl_nexus_ptr_sig(outConn);
    if (outSig)
    {
      outSigName = ivl_signal_basename(outSig);
    }
    if (isTriState(net))
    {
      rule = 1179;
      if (table[rule][sAct] == "yes")
      {
        const char *patt = "*_z";
        if(fnmatch(patt, outSigName, 0))
        {
          printViolation(rule, line, file, outSigName);
        }
      }
      rule = 1007;
      if (table[rule][sAct] == "yes")
      {
        unsigned joints = ivl_nexus_ptrs(muxOut);
        for(unsigned k = 1 ; k < joints ; k++)
        {
          outConn = ivl_nexus_ptr(muxOut, k);
          ivl_lpm_t anLPM = ivl_nexus_ptr_lpm(outConn);
          if (anLPM && (anLPM != net))
          {
            if (ivl_lpm_q(anLPM) == muxOut)
              printViolation(rule, line, file, outSigName);
          }
          ivl_net_logic_t aLogic = ivl_nexus_ptr_log(outConn);
          if (aLogic)
          {
            if (ivl_logic_pin(aLogic, 0) == muxOut)
              printViolation(rule, line, file, outSigName);
          }
        }
      }
    }
    else
    {
      rule = 1194;
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, outSigName);
      }
    }
  }
}

void checkClockSet(map<int, map<string, string> > & table,ivl_lpm_t & net)
{
  int rule = 1161;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    ivl_nexus_t ckNex = NULL;
    if (ivl_lpm_type(net) == IVL_LPM_FF)
    {
      ckNex = ivl_lpm_clk(net);
    }
    if (ivl_lpm_type(net) == IVL_LPM_LATCH)
    {
      ckNex = ivl_lpm_enable(net);
    }
    if (ckNex)
    {
      const char *ckSigName = NULL;
      unsigned connect = ivl_nexus_ptrs(ckNex);
      for(unsigned i = 0 ; i < connect ; i++)
      {
        ivl_nexus_ptr_t ckCon = ivl_nexus_ptr(ckNex, i);
        ivl_signal_t ckSig = ivl_nexus_ptr_sig(ckCon);
        if(ckSig)
        {
          ckSigName = ivl_signal_basename(ckSig);
        }
        ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(ckCon);
        if(anLpm)
        {
          if (table[rule][sAct] == "yes")
          {
            unsigned line = ivl_lpm_lineno(anLpm);
            const char *file = ivl_lpm_file(anLpm);
            if ((ivl_lpm_type(anLpm) == IVL_LPM_FF) &&
                ((ivl_lpm_sync_set(anLpm) == ckNex) ||
                 (ivl_lpm_async_set(anLpm) == ckNex)))
            {
              printViolation(rule, line, file, ckSigName);
            }
            if ((ivl_lpm_type(anLpm) == IVL_LPM_LATCH) &&
                (ivl_lpm_enable(anLpm) == ckNex))
            {
              printViolation(rule, line, file, ckSigName);
            }
            if ((ivl_lpm_type(anLpm) == IVL_LPM_MUX) &&
                (ivl_lpm_select(anLpm) == ckNex))
            {
              printViolation(rule, line, file, ckSigName);
            }
          }
        }   
      }
    }
  }
}

void checkNoSetReset(map<int, map<string, string> > & table, ivl_lpm_t & net)
{
  int rule = 1149;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(net);
  const char *file = ivl_lpm_file(net);
  if (table[rule][sAct] == "yes")
  {
    if (ivl_lpm_type(net) == IVL_LPM_FF)
    {
      bool setFound = false;
      bool clrFound = false;
      if (ivl_lpm_async_clr(net) || ivl_lpm_sync_clr(net))
        clrFound = true;
      if (ivl_lpm_async_set(net) || ivl_lpm_sync_set(net))
        setFound = true;
      if(!(setFound || clrFound))
        printViolation(rule, line, file, ivl_lpm_basename(net)); 
    }  
  }
}

void checkComboInSequential(map<int, map<string, string> > & table, ivl_statement_t stmt)
{
  int rule = 1139;
  const char *sAct = "active";
  int line = ivl_stmt_lineno(stmt);
  const char *file = ivl_stmt_file(stmt);
  if (table[rule][sAct] == "yes")
  {
    ivl_expr_t anExpr = NULL;
    switch (ivl_statement_type(stmt))
    {
      case IVL_ST_FORCE:
      case IVL_ST_ASSIGN:
      case IVL_ST_CASSIGN:
      case IVL_ST_CONTRIB:
      case IVL_ST_ASSIGN_NB:
      {
        anExpr = ivl_stmt_rval(stmt);
      }
      break;
      case IVL_ST_CONDIT:
      {
        ivl_statement_t tCls = ivl_stmt_cond_true(stmt);
        if (tCls)
          checkComboInSequential(table, tCls);
        ivl_statement_t fCls = ivl_stmt_cond_false(stmt);
        if (fCls)
          checkComboInSequential(table, fCls);
        anExpr = ivl_stmt_cond_expr(stmt);
      }
      break;
      case IVL_ST_BLOCK:
      {
        unsigned noStmt = ivl_stmt_block_count(stmt);
        for (unsigned idx = 0; idx < noStmt; idx++)
        {
          ivl_statement_t stBlk = ivl_stmt_block_stmt(stmt, idx);
          checkComboInSequential(table, stBlk);
        }
      }
      break;
      case IVL_ST_UTASK:
      case IVL_ST_DISABLE:
      {
      }
      break;
      case IVL_ST_STASK:
      {
      }
      break;
      case IVL_ST_DELAY:
      case IVL_ST_DELAYX:
      {
      }
      break;
      case IVL_ST_WHILE:
      case IVL_ST_DO_WHILE:
      {
      }
      break;
      case IVL_ST_WAIT:
      {
      }
      break;
      case IVL_ST_NOOP:
      case IVL_ST_REPEAT:
      {
      }
      break;
      case IVL_ST_TRIGGER:
      {
      }
      break;
      default:
      {
      }
      break;
    }
    if (anExpr && (ivl_expr_type(anExpr) == IVL_EX_BINARY ||
                   ivl_expr_type(anExpr) == IVL_EX_TERNARY))
    {
      printViolation(rule, line, file);
    }
  }
}

void checkFallingActiveClock(map<int, map<string, string> > & table,ivl_lpm_t & lpm)
{
  int rule = 1040;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (table[rule][sAct] == "yes")
  {
    if (ivl_lpm_type(lpm) == IVL_LPM_FF)
    {
      if (ivl_lpm_negedge(lpm))
      {
        ivl_nexus_t clkNex = ivl_lpm_clk(lpm);
        if (clkNex)
        {
          ivl_nexus_ptr_t aConn = ivl_nexus_ptr(clkNex, 0);
          ivl_signal_t clkSig = ivl_nexus_ptr_sig(aConn);
          if (clkSig)
          {
            const char *clkName = ivl_signal_basename(clkSig);
            printViolation(rule, line, file, clkName);
          }
        }
      }
    }
  }
}

ivl_lpm_t traverseTillFF(ivl_nexus_t nex, string pinName, set<ivl_net_logic_t> &trvrsGates)
{
  ivl_lpm_t found = NULL;
  unsigned connection = ivl_nexus_ptrs(nex);
  for(unsigned i = 0; i < connection; i++)
  {
    ivl_nexus_ptr_t sCon = ivl_nexus_ptr(nex, i);
    ivl_net_logic_t Gat = ivl_nexus_ptr_log(sCon);
    if(Gat)
    {
      if (trvrsGates.find(Gat) == trvrsGates.end())
      {
        trvrsGates.insert(Gat);
        if (pinName == "OUT")
        {
          unsigned pins = ivl_logic_pins(Gat);
          for (int i = 1; i < pins; i++)
          {
            ivl_nexus_t aJoint = ivl_logic_pin(Gat, i);
            if (aJoint == nex)
            {
              found = NULL;
              break;
            }
            else
              found = traverseTillFF(aJoint, pinName, trvrsGates);
          }
        }
        else
        {
          ivl_nexus_t outNex = ivl_logic_pin(Gat, 0);
          if (outNex != nex)
          {
            found = traverseTillFF(outNex, pinName, trvrsGates);
          }
        }
      }
      else
      {
        //combinational loop
      }
    }
    ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(sCon);
    if (anLpm)
    {
      if (ivl_lpm_type(anLpm) == IVL_LPM_MUX)
      {
        ivl_nexus_t selNex = ivl_lpm_select(anLpm);
        if (selNex == nex)
        {
          ivl_nexus_t outNex = ivl_lpm_q(anLpm);
          found = traverseTillFF(outNex, "DATA", trvrsGates);
        }
      }
      if (ivl_lpm_type(anLpm) == IVL_LPM_FF)
      {
        if (pinName == "DATA")
        {
          ivl_nexus_t dNex = ivl_lpm_data(anLpm, 0);
          if (dNex == nex)
          {
            found = anLpm;
          }
        }
        if (pinName == "SYNCCLR")
        {
          ivl_nexus_t scNex = ivl_lpm_sync_clr(anLpm);
          if (scNex == nex)
          {
            found = anLpm;
          }
        }
        if (pinName == "SYNCSET")
        {
          ivl_nexus_t ssNex = ivl_lpm_sync_set(anLpm);
          if (ssNex == nex)
          {
            found = anLpm;
          }
        }
        if (pinName == "ASYNCCLR")
        {
          ivl_nexus_t ascNex = ivl_lpm_async_clr(anLpm);
          if (ascNex == nex)
          {
            found = anLpm;
          }
        }
        if (pinName == "ASYNCSET")
        {
          ivl_nexus_t assNex = ivl_lpm_async_set(anLpm);
          if (assNex == nex)
          {
            found = anLpm;
          }
        }
        if (pinName == "CLOCK")
        {
          ivl_nexus_t assNex = ivl_lpm_clk(anLpm);
          if (assNex == nex)
          {
            found = anLpm;
          }
        }
        if (pinName == "OUT")
        {
          ivl_nexus_t outNex = ivl_lpm_q(anLpm);
          if (outNex == nex)
          {
            found = anLpm;
          }
        }
      }
    }
    if (found)
      break;
  }
  return found;
}

void checkInputsFromDifferentClockSource(map<int, map<string, string> > & table, ivl_net_logic_t & combGate)
{
  int rule = 1284;
  const char *sAct = "active";
  unsigned line = ivl_logic_lineno(combGate);
  const char *file = ivl_logic_file(combGate);
  if (table[rule][sAct] == "yes")
  {
    unsigned noInputs = ivl_logic_pins(combGate);
    if (noInputs > 2)
    {
      ivl_nexus_t ckNex = NULL;
      for (int i = 1; i < noInputs; i++)
      {
        set<ivl_net_logic_t> gateList;
        ivl_nexus_t inputNex = ivl_logic_pin(combGate, i);
        ivl_lpm_t foundFF = inputNex ? traverseTillFF(inputNex, "OUT", gateList) : NULL;
        if (ckNex)
        {
          if (ivl_lpm_clk(foundFF) != ckNex)
            printViolation(rule, line, file, ivl_logic_basename(combGate));
        }
        else
          ckNex = foundFF ? ivl_lpm_clk(foundFF) : NULL;
      }
    }
  }
}

void checkSyncAsyncReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    ivl_signal_t clrSig = NULL;
    const char *clrSigName = NULL;
    ivl_nexus_t clrNex = ivl_lpm_async_clr(lpm);
    unsigned connect = clrNex ? ivl_nexus_ptrs(clrNex) : 0;
    for(unsigned i = 0 ; i < connect ; i++)
    {
      if (i >= ivl_nexus_ptrs(clrNex))
        continue;
      ivl_nexus_ptr_t clrCon = ivl_nexus_ptr(clrNex, i);
      clrSig = clrSig ? clrSig : ivl_nexus_ptr_sig(clrCon);
      if(clrSig)
      {
        clrSigName = ivl_signal_basename(clrSig);
      }
      ivl_net_logic_t aLogic = ivl_nexus_ptr_log(clrCon);
      if (aLogic)
      {
        if (ivl_signal_local(clrSig))
        {
          if (ivl_logic_type(aLogic) == IVL_LO_NOT)
          {
            clrNex = ivl_logic_pin(aLogic, 1);
	    clrSig = NULL;
	    i = -1; // so that it bocome 0 in the next iteration
	    continue; //cant change connect, no need
          }
        }
      }
    }
    rule = 1031;
    if (table[rule][sAct] == "yes")
    {
      set<ivl_net_logic_t> gateList;
      ivl_lpm_t foundFF = clrNex ? traverseTillFF(clrNex, "SYNCCLR", gateList) : NULL;
      if (foundFF)
      {
        file = ivl_lpm_file(foundFF);
        line = ivl_lpm_lineno(foundFF);
        printViolation(rule, line, file, clrSigName);  
      }
    }
    rule = 1163;
    if (table[rule][sAct] == "yes")
    {
      set<ivl_net_logic_t> gateList;
      ivl_lpm_t foundFF = clrNex ? traverseTillFF(clrNex, "OUT", gateList) : NULL;
      if (foundFF)
      {
        file = ivl_lpm_file(foundFF);
        line = ivl_lpm_lineno(foundFF);
        printViolation(rule, line, file, clrSigName);
      }
    }
    rule = 1164;
    if (table[rule][sAct] == "yes")
    {
      set<ivl_net_logic_t> gateList;
      ivl_lpm_t foundFF = clrNex ? traverseTillFF(clrNex, "DATA", gateList) : NULL;
      if (foundFF)
      {
        file = ivl_lpm_file(foundFF);
        line = ivl_lpm_lineno(foundFF);
        printViolation(rule, line, file, clrSigName);
      }
    }
  }
}

ivl_signal_t traverseBackward(ivl_nexus_t aNex)
{
  ivl_signal_t piFound = NULL;
  unsigned connections = aNex ? ivl_nexus_ptrs(aNex) : 0;
  for (int j = 0; j < connections; j++)
  {
    ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aNex, j);
    ivl_signal_t aSig = ivl_nexus_ptr_sig(aConn);
    if (aSig && ((ivl_signal_port(aSig) == IVL_SIP_INOUT) ||
                 (ivl_signal_port(aSig) == IVL_SIP_INPUT)))
    {
      piFound = aSig;
      break;
    }
    ivl_net_logic_t aLog = ivl_nexus_ptr_log(aConn);
    if(aLog && (ivl_nexus_ptr_pin(aConn) == 0))
    {
      unsigned pins = ivl_logic_pins(aLog);
      for (int i = 1; i < pins; i++)
      {
        ivl_nexus_t aJoint = ivl_logic_pin(aLog, i);
        piFound = traverseBackward(aJoint);
	if (piFound)
          break;
      }
    }
    ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(aConn);
    if(anLpm && (ivl_lpm_q(anLpm) == aNex))
    {
      if ((ivl_lpm_type(anLpm) != IVL_LPM_FF) &&
          (ivl_lpm_type(anLpm) != IVL_LPM_LATCH))
      {
        unsigned lpmSize = 2;
        for (int i = 0; i < lpmSize; i++)
        {
          ivl_nexus_t inLpm = ivl_lpm_data(anLpm, i); 
          piFound = traverseBackward(inLpm);
          if (piFound)
            break;
        }
      }
    }
    ivl_branch_t aBrnc = ivl_nexus_ptr_branch(aConn);
    if(aBrnc && (ivl_branch_terminal(aBrnc, 0) == aNex))
    {
      ivl_nexus_t inBrnc = ivl_branch_terminal(aBrnc, 1); 
      piFound = traverseBackward(inBrnc);
	if (piFound)
          break;
    }
    ivl_switch_t aSwc = ivl_nexus_ptr_switch(aConn);
    if(aSwc && (ivl_switch_b(aSwc) == aNex))
    {
      ivl_nexus_t inSwc = ivl_switch_a(aSwc); 
      piFound = traverseBackward(inSwc);
	if (piFound)
          break;
    }
  }
  return piFound;
}

void checkSignalUnconnected(map<int, map<string, string> > &table, ivl_signal_t aSig)
{
  int rule = 1299;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_port(aSig) == IVL_SIP_NONE)
    {
      unsigned line = ivl_signal_lineno(aSig);
      const char *file = ivl_signal_file(aSig);
      const char *aSigName = ivl_signal_basename(aSig);
      unsigned elements = ivl_signal_array_count(aSig);
      for (int i = 0; i < elements; i++)
      {
        ivl_nexus_t aJoint = ivl_signal_nex(aSig, i);
        bool inFound = traverseBackward(aJoint);
        if (!inFound)
          printViolation(rule, line, file, aSigName, "input");
      }
    }
  }
}

void checkReconvClock(map<int, map<string, string> > & table, ivl_net_logic_t & gate)
{
  int rule = 1209;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    unsigned nPins = ivl_logic_pins(gate);
    if (nPins > 2)
    {
      int line = ivl_logic_lineno(gate);
      const char *file = ivl_logic_file(gate);
      ivl_nexus_t pin1 = ivl_logic_pin(gate, 1);
      ivl_nexus_t pin2 = ivl_logic_pin(gate, 2);
      ivl_signal_t piFound1 = traverseBackward(pin1);
      if (piFound1)
      {
        set<ivl_net_logic_t> gateList;
        ivl_lpm_t ifPIclock = traverseTillFF(ivl_signal_nex(piFound1, 0), "CLOCK", gateList);
	piFound1 = ifPIclock ? piFound1 : NULL;
      }
      ivl_signal_t piFound2 = traverseBackward(pin2);
      if (piFound2)
      {
        set<ivl_net_logic_t> gateList;
        ivl_lpm_t ifPIclock = traverseTillFF(ivl_signal_nex(piFound2, 0), "CLOCK", gateList);
	piFound2 = ifPIclock ? piFound2 : NULL;
      }
      if (piFound1 && (piFound1 == piFound2))
      {
        printViolation(rule, line, file, ivl_signal_basename(piFound1));
      }
      if (nPins > 3)
      {
        ivl_nexus_t pin3 = ivl_logic_pin(gate, 3);
        ivl_signal_t piFound3 = traverseBackward(pin3);
        if (piFound3)
        {
          set<ivl_net_logic_t> gateList;
          ivl_lpm_t ifPIclock = traverseTillFF(ivl_signal_nex(piFound3, 0), "CLOCK", gateList);
	  piFound3 = ifPIclock ? piFound3 : NULL;
        }
        if (piFound3 && ((piFound1 == piFound3) || (piFound2 == piFound3)))
        {
          printViolation(rule, line, file, ivl_signal_basename(piFound3));
        }
      }
    }
  }
}

void checkTestClockPrimaryInput(map<int, map<string, string> > & table, ivl_lpm_t & net)
{
  int rule = 1156;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    ivl_nexus_t aNex = NULL;
    if (ivl_lpm_type(net) == IVL_LPM_FF)
    {
      aNex = ivl_lpm_clk(net);
    }
    if (ivl_lpm_type(net) == IVL_LPM_LATCH)
    {
      aNex = ivl_lpm_enable(net);
    }
    if (aNex)
    {
      const char *aSigName = NULL;
      unsigned connect = ivl_nexus_ptrs(aNex);
      for(unsigned i = 0 ; i < connect ; i++)
      {
        ivl_nexus_ptr_t aCon = ivl_nexus_ptr(aNex, i);
        ivl_signal_t aSig = ivl_nexus_ptr_sig(aCon);
        if (aSig)
        {
          aSigName = ivl_signal_basename(aSig);
        }
        ivl_net_logic_t aLog = ivl_nexus_ptr_log(aCon);
        if (aLog && (ivl_nexus_ptr_pin(aCon) == 0))
        {
          unsigned pins = ivl_logic_pins(aLog);
          for (int i = 1; i < pins; i++)
          {
            ivl_nexus_t aJoint = ivl_logic_pin(aLog, i);
            if (traverseBackward(aJoint))
            {
              int line = ivl_lpm_lineno(net);
              const char *file = ivl_lpm_file(net);
              printViolation(rule, line, file, aSigName);
            }
          }
        }
      }
    }
  }
}

void traverseForward(ivl_nexus_t aNex, ivl_signal_t piSig, int aRule)
{
  unsigned connections = 0;
  if (aNex)
    connections = ivl_nexus_ptrs(aNex);

  for (int j = 0; j < connections; j++)
  {
    ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aNex, j);
    ivl_signal_t aSig = ivl_nexus_ptr_sig(aConn);
    if (aSig && (aSig != piSig))
    {
      if (ivl_signal_port(aSig) == IVL_SIP_INOUT ||
          ivl_signal_port(aSig) == IVL_SIP_OUTPUT)
      {
        int line = ivl_signal_lineno(piSig);
        const char *file = ivl_signal_file(piSig);
        printViolation(aRule, line, file, ivl_signal_basename(piSig), ivl_signal_basename(aSig));
	break;
      }
    }
    ivl_net_logic_t aLog = ivl_nexus_ptr_log(aConn);
    if(aLog)
    {
      ivl_nexus_t outLog = ivl_logic_pin(aLog, 0); 
      if (outLog != aNex)
        traverseForward(outLog, piSig, aRule);
    }
    ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(aConn);
    if(anLpm)
    {
      if (ivl_lpm_type(anLpm) == IVL_LPM_FF)
      {
        continue;
      }
      ivl_nexus_t outLpm = ivl_lpm_q(anLpm); 
      if (outLpm != aNex)
        traverseForward(outLpm, piSig, aRule);
    }
    ivl_branch_t aBrnc = ivl_nexus_ptr_branch(aConn);
    if(aBrnc)
    {
      ivl_nexus_t outBrnc = ivl_branch_terminal(aBrnc, 0); 
      if (outBrnc != aNex)
        traverseForward(outBrnc, piSig, aRule);
    }
    ivl_switch_t aSwc = ivl_nexus_ptr_switch(aConn);
    if(aSwc)
    {
      ivl_nexus_t outSwc = ivl_switch_b(aSwc); 
      if (outSwc != aNex)
        traverseForward(outSwc, piSig, aRule);
    }
  }
}

void checkClockSignalOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 1157;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    ivl_nexus_t aNex = NULL;
    if (ivl_lpm_type(lpm) == IVL_LPM_FF)
    {
      aNex = ivl_lpm_clk(lpm);
    }
    if (ivl_lpm_type(lpm) == IVL_LPM_LATCH)
    {
      aNex = ivl_lpm_enable(lpm);
    }
    if (aNex)
    {
      unsigned connect = ivl_nexus_ptrs(aNex);
      for(unsigned i = 0 ; i < connect ; i++)
      {
        ivl_nexus_ptr_t aCon = ivl_nexus_ptr(aNex, i);
        ivl_signal_t aSig = ivl_nexus_ptr_sig(aCon);
        if (aSig)
        {
          traverseForward(aNex, aSig, rule);
        }
      }
    }
  }
}

void checkResetSignalOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 1208;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    ivl_nexus_t aNex = NULL;
    if (ivl_lpm_type(lpm) == IVL_LPM_FF)
    {
      aNex = ivl_lpm_sync_clr(lpm);
      aNex = aNex ? aNex : ivl_lpm_async_clr(lpm);
    }
    if (aNex)
    {
      unsigned connect = ivl_nexus_ptrs(aNex);
      for(unsigned i = 0 ; i < connect ; i++)
      {
        ivl_nexus_ptr_t aCon = ivl_nexus_ptr(aNex, i);
        ivl_signal_t aSig = ivl_nexus_ptr_sig(aCon);
        if (aSig)
        {
          traverseForward(aNex, aSig, rule);
        }
      }
    }
  }
}

void checkCombinationalPiPoPath(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  if (ivl_signal_port(mySig) == IVL_SIP_INPUT ||
      ivl_signal_port(mySig) == IVL_SIP_INOUT)
  {
    int rule = 1124;
    const char *sAct = "active";
    if (table[rule][sAct] == "yes")
    {
      bool regFound = false;
      unsigned elements = ivl_signal_array_count(mySig);
      for (int i = 0; i < elements; i++)
      {
        ivl_nexus_t aJoint = ivl_signal_nex(mySig, i);
        traverseForward(aJoint, mySig, rule);
      }
    }
  }
}

void checkSpecialSignalBitSelect(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  unsigned line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    ivl_nexus_t clrNex = ivl_lpm_sync_clr(lpm);
    if (!clrNex)
    {
      clrNex = ivl_lpm_async_clr(lpm);
    }
    if (clrNex)
    {
      unsigned connect = ivl_nexus_ptrs(clrNex);
      for(unsigned i = 0 ; i < connect ; i++)
      {
        ivl_nexus_ptr_t clrCon = ivl_nexus_ptr(clrNex, i);
        ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(clrCon);
        if(anLpm && (ivl_lpm_type(anLpm) == IVL_LPM_PART_VP))
        {
          if (ivl_lpm_q(anLpm) == clrNex)
          {
            const char *inVectSigName = NULL;
            ivl_nexus_t inVectNex = ivl_lpm_data(anLpm, 0);
            for(unsigned j = 0 ; j < ivl_nexus_ptrs(inVectNex); j++)
            {
              ivl_nexus_ptr_t inVectCon = ivl_nexus_ptr(inVectNex, j);
              ivl_signal_t inVectSig = ivl_nexus_ptr_sig(inVectCon);
              if(inVectSig)
              {
                inVectSigName = ivl_signal_basename(inVectSig);
              }
            }
            rule = 1114;
            if (table[rule][sAct] == "yes")
            {
              printViolation(rule, line, file, inVectSigName);
            }
          }
        }
      }
    }
  }
  if (ivl_lpm_type(lpm) == IVL_LPM_MUX)
  {
    ivl_nexus_t slctNex = ivl_lpm_select(lpm);
    unsigned connect = ivl_nexus_ptrs(slctNex);
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t slctCon = ivl_nexus_ptr(slctNex, i);
      ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(slctCon);
      if(anLpm)
      {
        if (ivl_lpm_type(anLpm) == IVL_LPM_PART_VP)
        {
          if (ivl_lpm_q(anLpm) == slctNex)
          {
            const char *inVectSigName = NULL;
            ivl_nexus_t inVectNex = ivl_lpm_data(anLpm, 0);
            for(unsigned j = 0 ; j < ivl_nexus_ptrs(inVectNex); j++)
            {
              ivl_nexus_ptr_t inVectCon = ivl_nexus_ptr(inVectNex, j);
              ivl_signal_t inVectSig = ivl_nexus_ptr_sig(inVectCon);
              if(inVectSig)
              {
                inVectSigName = ivl_signal_basename(inVectSig);
              }
            }
            rule = 1114;
            if (table[rule][sAct] == "yes")
            {
              printViolation(rule, line, file, inVectSigName);
            }
          }
        }
        if (ivl_lpm_type(anLpm) == IVL_LPM_CMP_NE)
        {
          for(unsigned k = 0 ; k < 2; k++)
          {
            const char *inNESigName = NULL;;
            ivl_nexus_t inNENex = ivl_lpm_data(anLpm, 0);
            for(unsigned j = 0 ; j < ivl_nexus_ptrs(inNENex); j++)
            {
              ivl_nexus_ptr_t inNECon = ivl_nexus_ptr(inNENex, j);
              ivl_signal_t inNESig = ivl_nexus_ptr_sig(inNECon);
              if(inNESig && (ivl_signal_width(inNESig) > 1))
              {
                rule = 1122;
                if (table[rule][sAct] == "yes")
                {
                  inNESigName = ivl_signal_basename(inNESig);
                  printViolation(rule, line, file, inNESigName);
                }
              }
            }
          }
        }
      }
      ivl_net_logic_t aLgic = ivl_nexus_ptr_log(slctCon);
      if(aLgic && (ivl_logic_type(aLgic) == IVL_LO_NOT)
               && (ivl_nexus_ptr_pin(slctCon) == 0))
      {
        ivl_nexus_t inLgicNex = ivl_logic_pin(aLgic, 1);
        for(unsigned i = 0 ; i < ivl_nexus_ptrs(inLgicNex) ; i++)
        {
          ivl_nexus_ptr_t inLgicCon = ivl_nexus_ptr(inLgicNex, i);
          ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(inLgicCon);
          if(anLpm && (ivl_lpm_type(anLpm) == IVL_LPM_PART_VP))
          {
            if (ivl_lpm_q(anLpm) == inLgicNex)
            {
              const char *inVectSigName = NULL;
              ivl_nexus_t inVectNex = ivl_lpm_data(anLpm, 0);
              for(unsigned j = 0 ; j < ivl_nexus_ptrs(inVectNex); j++)
              {
                ivl_nexus_ptr_t inVectCon = ivl_nexus_ptr(inVectNex, j);
                ivl_signal_t inVectSig = ivl_nexus_ptr_sig(inVectCon);
                if(inVectSig)
                {
                  inVectSigName = ivl_signal_basename(inVectSig);
                }
              }
              rule = 1114;
              if (table[rule][sAct] == "yes")
              {
                printViolation(rule, line, file, inVectSigName);
              }
            }
          }
        }
      }
    }
  }
}

void checkEnableSetReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    ivl_signal_t setSig = NULL;
    const char *setSigName = NULL;
    ivl_nexus_t setNex = ivl_lpm_async_set(lpm);
    unsigned connect = setNex ? ivl_nexus_ptrs(setNex) : 0;
    for(unsigned i = 0 ; i < connect ; i++)
    {
      if (i >= ivl_nexus_ptrs(setNex))
        continue;
      ivl_nexus_ptr_t setCon = ivl_nexus_ptr(setNex, i);
      setSig = setSig ? setSig : ivl_nexus_ptr_sig(setCon);
      setSigName = setSig ? ivl_signal_basename(setSig) : NULL;
      ivl_net_logic_t aLogic = ivl_nexus_ptr_log(setCon);
      if (aLogic)
      {
        if (ivl_signal_local(setSig))
        {
          if (ivl_logic_type(aLogic) == IVL_LO_NOT)
          {
            setNex = ivl_logic_pin(aLogic, 1);
	    setSig = NULL;
	    i = -1; // so that it bocome 0 in the next iteration
	    continue; //cant change connect, no need
          }
        }
      }
    }
    rule = 1111;
    if (table[rule][sAct] == "yes")
    {
      set<ivl_net_logic_t> gateList;
      ivl_lpm_t foundFF = setNex ? traverseTillFF(setNex, "ASYNCCLR", gateList) : NULL;
      gateList.clear();
      foundFF = foundFF ? foundFF : setNex ? traverseTillFF(setNex, "SYNCCLR", gateList) : NULL;
      if (foundFF)
      {
        file = ivl_lpm_file(foundFF);
        line = ivl_lpm_lineno(foundFF);
        printViolation(rule, line, file, setSigName);
      }
    }
    rule = 1148;
    if (table[rule][sAct] == "yes")
    {
      set<ivl_net_logic_t> gateList;
      ivl_lpm_t foundFF = setNex ? traverseTillFF(setNex, "OUT", gateList) : NULL;
      if (foundFF)
      {
        file = ivl_lpm_file(foundFF);
        line = ivl_lpm_lineno(foundFF);
        printViolation(rule, line, file, setSigName);
      }
    }
    rule = 1165;
    if (table[rule][sAct] == "yes")
    {
      set<ivl_net_logic_t> gateList;
      ivl_lpm_t foundFF = setNex ? traverseTillFF(setNex, "SYNCSET", gateList) : NULL;
      if (foundFF)
      {
        file = ivl_lpm_file(foundFF);
        line = ivl_lpm_lineno(foundFF);
        printViolation(rule, line, file, setSigName);
      }
    }
  }
}

void checkSetDataInput(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    const char *setSigName = NULL;
    ivl_nexus_t setNex = ivl_lpm_async_set(lpm);
    if (!setNex)
    {
      setNex = ivl_lpm_sync_set(lpm);
      if (!setNex)
        return;
    }
    ivl_signal_t setSigStore = NULL;
    unsigned connect = ivl_nexus_ptrs(setNex);
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t setCon = ivl_nexus_ptr(setNex, i);
      ivl_signal_t setSig = ivl_nexus_ptr_sig(setCon);
      if(setSig)
      {
        setSigStore = setSig;
        setSigName = ivl_signal_basename(setSig);
      }
      ivl_net_logic_t aLogic = ivl_nexus_ptr_log(setCon);
      if (aLogic)
      {
	if (ivl_signal_local(setSigStore))
	{
          if (ivl_logic_type(aLogic) == IVL_LO_NOT)
	  {
            rule = 1110;
            if (table[rule][sAct] == "yes")
            {
              set<ivl_net_logic_t> gateList;
              ivl_nexus_t inNot = ivl_logic_pin(aLogic, 1);
              ivl_lpm_t foundFF = traverseTillFF(inNot, "DATA", gateList);
              if (foundFF)
              {
                file = ivl_lpm_file(foundFF);
                line = ivl_lpm_lineno(foundFF);
                ivl_nexus_ptr_t inConn = ivl_nexus_ptr(inNot, 0);
                ivl_signal_t inSig = ivl_nexus_ptr_sig(inConn);
                if (inSig)
                  setSigName = ivl_signal_basename(inSig);
                printViolation(rule, line, file, setSigName);
	      }
	    }
	  }
	}
      }
      ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(setCon);
      if(anLpm)
      {
        if (ivl_lpm_data(anLpm, 0) == setNex)
        {
          rule = 1110;
          if (table[rule][sAct] == "yes")
          {
            file = ivl_lpm_file(anLpm);
            line = ivl_lpm_lineno(anLpm);
            printViolation(rule, line, file, setSigName);
          }
        }
      }
    }
  }
}

void checkSignalNoLoad(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1104;
  const char *sAct = "active";
  if ((table[rule][sAct] == "yes") && (ivl_signal_port(mySig) != IVL_SIP_OUTPUT))
  {
    int line = ivl_signal_lineno(mySig);
    const char *file = ivl_signal_file(mySig);
    const char* sigName = ivl_signal_basename(mySig);

    bool hasLoad = false;
    unsigned elements = ivl_signal_array_count(mySig);
    for (int i = 0; i < elements; i++)
    {
      ivl_nexus_t aJoint = ivl_signal_nex(mySig, i);
      unsigned connections = 0;
      if (aJoint)
        connections = ivl_nexus_ptrs(aJoint);

      for (int j = 0; j < connections; j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
        if (ivl_nexus_ptr_sig(aConn) && (ivl_nexus_ptr_sig(aConn) != mySig))
        {
          hasLoad = true;
	  break;
        }
        if (ivl_nexus_ptr_lpm(aConn))
        {
          ivl_lpm_t anLPM = ivl_nexus_ptr_lpm(aConn);
          if ((ivl_lpm_q(anLPM) != aJoint))
          {
            hasLoad = true;
	    break;
          }
        }
        if (ivl_nexus_ptr_pin(aConn))
	{
          hasLoad = true;
	  break;
	}
      }
    }
    if (!hasLoad)
    {
      printViolation(rule, line, file, sigName);
    }
  }
}

void checkNetStuck(map<int, map<string, string> > & table, ivl_statement_t & myStmt)
{
  int rule = 1103; // same as 1109, not implemented
  const char *sAct = "active";
  int line = ivl_stmt_lineno(myStmt);
  const char *file = ivl_stmt_file(myStmt);
  if (table[rule][sAct] == "yes")
  {
    for (unsigned idx = 0 ;  idx < ivl_stmt_lvals(myStmt) ;  idx += 1)
    {
      ivl_lval_t lv =  ivl_stmt_lval(myStmt, idx);
      ivl_signal_t lvSig =  ivl_lval_sig(lv);
      if (lvSig)
      {
        const char *lvSigName = ivl_signal_basename(lvSig);
        ivl_expr_t rvExp = ivl_stmt_rval(myStmt);
        ivl_expr_t opr1 = NULL;
        if (rvExp && ivl_expr_type(rvExp) == IVL_EX_BINARY)
        {
          char expOpcd = ivl_expr_opcode(rvExp);
          ivl_expr_t opr1 = ivl_expr_oper1(rvExp);
          ivl_expr_t opr2 = ivl_expr_oper2(rvExp);
          ivl_signal_t opr1Sig = NULL;
          ivl_signal_t opr2Sig = NULL;
          unsigned opr1Val = 0;
          unsigned opr2Val = 0;
          char opCd1 = '\0';
          char opCd2 = '\0';

          switch (ivl_expr_type(opr1))
          {
            case IVL_EX_UNARY:
            {
              opCd1 = ivl_expr_opcode(opr1);
              ivl_expr_t unOpr1 = ivl_expr_oper1(opr1);
              if (ivl_expr_type(unOpr1) == IVL_EX_SIGNAL)
              {
                opr1Sig = ivl_expr_signal(unOpr1);
              }
  	    }
            break;
            case IVL_EX_SIGNAL:
            {
              opr1Sig = ivl_expr_signal(opr1);
  	    }
            break;
            case IVL_EX_ULONG:
            case IVL_EX_NUMBER:
            {
              opr1Val = ivl_expr_uvalue(opr1);
  	    }
            break;
            default:
            {
              opr1Val = 0;
              opr1Sig = NULL;
  	    }
            break;
  	  }
          switch (ivl_expr_type(opr2))
  	  {
            case IVL_EX_UNARY:
            {
              opCd2 = ivl_expr_opcode(opr2);
              ivl_expr_t unOpr2 = ivl_expr_oper1(opr2);
              if (ivl_expr_type(unOpr2) == IVL_EX_SIGNAL)
  	      {
                opr2Sig = ivl_expr_signal(unOpr2);
  	      }
  	    }
            break;
            case IVL_EX_SIGNAL:
            {
              opr2Sig = ivl_expr_signal(opr2);
  	    }
            break;
            case IVL_EX_ULONG:
            case IVL_EX_NUMBER:
            {
              opr2Val = ivl_expr_uvalue(opr2);
  	    }
            break;
            default:
            {
              opr2Val = 0;
              opr2Sig = NULL;
  	    }
            break;
  	  }

          if (opr1Sig && opr2Sig && (opr1Sig == opr2Sig))
          {
            switch (expOpcd)
  	    {
              case '|':
              case '&':
              case 'A':
              {
  	        if (((opCd1 == '~') && (opCd2 == '\0')) ||
                    ((opCd1 == '\0') && (opCd2 == '~')))
  	        {
                  printViolation(rule, line, file, lvSigName);
                }
              }
  	      break;
              case 'X':
              case '^':
              {
                printViolation(rule, line, file, lvSigName);
              }
              break;
              default:
              {
              }
              break;
            }
          }
  	  else
  	  {
            if (opr1Sig)
  	    {
              if (opr2Val)
  	      {
                if ((expOpcd == '|') || (expOpcd == 'N'))
  	        {
                  printViolation(rule, line, file, lvSigName);
  	        }
  	      }
  	      else
  	      {
                if ((expOpcd == '&') || (expOpcd == 'A'))
  	        {
                  printViolation(rule, line, file, lvSigName);
  	        }
  	      }
  	    }
  	    else if (opr2Sig)
  	    {
              if (opr1Val)
  	      {
                if ((expOpcd == '|') || (expOpcd == 'N'))
  	        {
                  printViolation(rule, line, file, lvSigName);
  	        }
  	      }
  	      else
  	      {
                if ((expOpcd == '&') || (expOpcd == 'A'))
  	        {
                  printViolation(rule, line, file, lvSigName);
  	        }
  	      }
  	    }
  	    else
  	    {
              printViolation(rule, line, file, lvSigName);
  	    }
  	  }
        }
        if (rvExp && ((ivl_expr_type(rvExp) == IVL_EX_ULONG) ||
                      (ivl_expr_type(rvExp) == IVL_EX_NUMBER)))
        {
          printViolation(rule, line, file, lvSigName);
        }
      }
    }
  }
}

void checkSignalDrivConst(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1099;
  const char *sAct = "active";
  if (table[rule][sAct] == "yes")
  {
    unsigned elements = ivl_signal_array_count(mySig);
    for (int i = 0; i < elements; i++)
    {
      ivl_nexus_t aNex = ivl_signal_nex(mySig, i);
      unsigned connections = 0;
      if (aNex)
        connections = ivl_nexus_ptrs(aNex);

      for(unsigned j = 0 ; j < connections ; j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aNex, j);
        ivl_net_const_t aConst = ivl_nexus_ptr_con(aConn);
        if(aConst)
        {
          int line = ivl_const_lineno(aConst);
          const char *file = ivl_const_file(aConst);
          printViolation(rule, line, file, ivl_signal_basename(mySig));
        }
      }
    }
  }
}

void checkAssignmentInputSignal(map<int, map<string, string> > & table, ivl_signal_t & mySig)
{
  int rule = 1100;
  const char *sAct = "active";
  int line = ivl_signal_lineno(mySig);
  const char *file = ivl_signal_file(mySig);
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_port(mySig) == IVL_SIP_INPUT)
    {
      unsigned elements = ivl_signal_array_count(mySig);
      for (int i = 0; i < elements; i++)
      {
        ivl_nexus_t aNex = ivl_signal_nex(mySig, i);
        unsigned connections = ivl_nexus_ptrs(aNex);
        for(unsigned j = 0 ; j < connections ; j++)
        {
          ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aNex, j);
          ivl_net_logic_t aLog = ivl_nexus_ptr_log(aConn);
          if(aLog)
          {
            if (ivl_logic_type(aLog) == IVL_LO_BUFZ)
	    {
              if (ivl_nexus_ptr_pin(aConn) == 0)
	      {
                printViolation(rule, line, file, ivl_signal_basename(mySig));
	      }
	    }
          }
        }
      }
    }
  }
}

void checkClockSeqLogic(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    const char *ckSigName = NULL;
    ivl_nexus_t ckNex = ivl_lpm_clk(lpm);
    unsigned connect = ivl_nexus_ptrs(ckNex);
    for(unsigned i = 0 ; i < connect ; i++)
    {
      ivl_nexus_ptr_t ckCon = ivl_nexus_ptr(ckNex, i);
      ivl_signal_t ckSig = ivl_nexus_ptr_sig(ckCon);
      if(ckSig)
      {
        ckSigName = ivl_signal_basename(ckSig);
      }
      ivl_net_logic_t aLogic = ivl_nexus_ptr_log(ckCon);
      if (aLogic)
      {
        if (ivl_nexus_ptr_pin(ckCon))
        {
          rule = 1095; // same as 1158, not implemented
          if (table[rule][sAct] == "yes")
          {
            file = ivl_logic_file(aLogic);
            line = ivl_logic_lineno(aLogic);
            printViolation(rule, line, file, ckSigName);
          }
        }
        else
        {
          rule = 1094;
          if (table[rule][sAct] == "yes")
          {
            file = ivl_lpm_file(lpm);
            line = ivl_lpm_lineno(lpm);
            printViolation(rule, line, file, ckSigName);
          }
        }
      }
      ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(ckCon);
      if(anLpm)
      {
        if (ivl_lpm_q(anLpm) == ckNex)
        {
          file = ivl_lpm_file(lpm);
          line = ivl_lpm_lineno(lpm);
          rule = 1094;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, ckSigName);
          }
          rule = 1280;
          if (table[rule][sAct] == "yes")
          {
            if ((ivl_lpm_type(anLpm) == IVL_LPM_MUX) && isTriState(anLpm))
              printViolation(rule, line, file, ckSigName);
          }
        }
        if (ivl_lpm_data(anLpm, 0) == ckNex)
        {
          rule = 1095; // same as 1158, not implemented
          if (table[rule][sAct] == "yes")
          {
            file = ivl_lpm_file(anLpm);
            line = ivl_lpm_lineno(anLpm);
            printViolation(rule, line, file, ckSigName);
          }
        }
        if ((ivl_lpm_type(anLpm) == IVL_LPM_MUX) &&
            (ivl_lpm_select(anLpm) == ckNex))
        {
          rule = 1146;
          file = ivl_lpm_file(anLpm);
          line = ivl_lpm_lineno(anLpm);
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, ckSigName);
          }
          rule = 1155;
          if (table[rule][sAct] == "yes")
          {
            printViolation(rule, line, file, ckSigName);
          }
        }
        if ((ivl_lpm_type(anLpm) == IVL_LPM_FF) &&
            ((ivl_lpm_sync_clr(anLpm) == ckNex) ||
             (ivl_lpm_async_clr(anLpm) == ckNex)))
        {
          rule = 1146;
          if (table[rule][sAct] == "yes")
          {
            file = ivl_lpm_file(anLpm);
            line = ivl_lpm_lineno(anLpm);
            printViolation(rule, line, file, ckSigName);
          }
        }
      }
    }
  }
}

void checkTriEnb(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  int line = 0;
  const char *file = NULL;
  if (ivl_lpm_type(lpm) == IVL_LPM_MUX)
  {
    ivl_nexus_t datNex = ivl_lpm_data(lpm, 0);
    unsigned connections = ivl_nexus_ptrs(datNex);
    for (int j = 0; j < connections; j++)
    {
      ivl_nexus_ptr_t datConn = ivl_nexus_ptr(datNex, j);
      ivl_signal_t datSig = ivl_nexus_ptr_sig(datConn);
      if (datSig && (ivl_signal_type(datSig) == IVL_SIT_TRI))
      {
        const char *selSigName = NULL;
        ivl_nexus_t selNex = ivl_lpm_select(lpm);
        for (int k = 0; k < ivl_nexus_ptrs(selNex); k++)
        {
          ivl_nexus_ptr_t selConn = ivl_nexus_ptr(selNex, k);
          ivl_signal_t selSig = ivl_nexus_ptr_sig(selConn);
          if (selSig)
          {
            selSigName = ivl_signal_basename(selSig);
          }
          ivl_lpm_t selLpm = ivl_nexus_ptr_lpm(selConn);
          ivl_net_logic_t selLog = ivl_nexus_ptr_log(selConn);
          rule = 1090;
          if (table[rule][sAct] == "yes")
          {
            if (selLpm && (selLpm != lpm))
            {
              line = ivl_lpm_lineno(selLpm);
              file = ivl_lpm_file(selLpm);
              printViolation(rule, line, file, selSigName);
            }
            if (selLog)
            {
              line = ivl_logic_lineno(selLog);
              file = ivl_logic_file(selLog);
              printViolation(rule, line, file, selSigName);
            }
          }
        }

        const char *outSigName = NULL;
        ivl_nexus_t outNex = ivl_lpm_q(lpm);
        for (int l = 0; l < ivl_nexus_ptrs(outNex); l++)
        {
          ivl_nexus_ptr_t outConn = ivl_nexus_ptr(outNex, l);
          ivl_signal_t outSig = ivl_nexus_ptr_sig(outConn);
          if (outSig)
          {
            outSigName = ivl_signal_basename(outSig);
            rule = 1112; // same as 1193, not implemented
            if (table[rule][sAct] == "yes")
            {
              line = ivl_lpm_lineno(lpm);
              file = ivl_lpm_file(lpm);
              printViolation(rule, line, file, outSigName);
            }
          }
          ivl_lpm_t outLpm = ivl_nexus_ptr_lpm(outConn);
          ivl_net_logic_t outLog = ivl_nexus_ptr_log(outConn);
          rule = 1092; // same as 1190, not implemented
          if (table[rule][sAct] == "yes")
          {
            if (outLpm && (outLpm != lpm))
            {
              line = ivl_lpm_lineno(outLpm);
              file = ivl_lpm_file(outLpm);
              printViolation(rule, line, file, outSigName);
            }
            if (outLog)
            {
              line = ivl_logic_lineno(outLog);
              file = ivl_logic_file(outLog);
              printViolation(rule, line, file, outSigName);
            }
          }
        }
      }
    }
  }
}

void checkClockActiveBothEdges(map<int, map<string, string> > &table, ivl_lpm_t &lpm)
{
  int rule = 1041;
  const char *sAct = "active";
  int line = ivl_lpm_lineno(lpm);
  const char *file = ivl_lpm_file(lpm);
  if (table[rule][sAct] == "yes")
  {
    if ((ivl_lpm_type(lpm) == IVL_LPM_FF) && !ivl_lpm_negedge(lpm))
    {
      ivl_nexus_t pckNex = ivl_lpm_clk(lpm);
      ivl_nexus_t aJoint = ivl_lpm_q(lpm);
      unsigned connections = ivl_nexus_ptrs(aJoint);
      for (int j = 0; j < connections; j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
        ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(aConn);
        if (anLpm && (ivl_lpm_type(anLpm) == IVL_LPM_FF) && ivl_lpm_negedge(anLpm))
        {
          ivl_nexus_t nckNex = ivl_lpm_clk(anLpm);
	  if (nckNex == pckNex)
	  {
            const char *clkName = NULL;
	    unsigned ckConns = ivl_nexus_ptrs(nckNex);
            for (int j = 0; j < ckConns; j++)
            {
              ivl_nexus_ptr_t aConn = ivl_nexus_ptr(nckNex, j);
	      ivl_signal_t aSig = ivl_nexus_ptr_sig(aConn);
	      if (aSig)
              {
	        clkName = ivl_signal_basename(aSig);
		break;
	      }
	    }
	    if (!clkName)
	    {
	      clkName = "FOJU";
	      //clkName = ivl_nexus_name(nckNex);
	    }
            printViolation(rule, line, file, clkName);
          }
        }
      }
    }  
  }
}

void checkLibraryCellInst(map<int, map<string, string> > & table, ivl_scope_t & scope)
{
  int rule = 1039;
  const char *sAct = "active";
  int line = ivl_scope_lineno(scope);
  const char *file = ivl_scope_file(scope);
  if (table[rule][sAct] == "yes")
  {
    const char *instName = ivl_scope_basename(scope);
    const char *cellName = ivl_scope_tname(scope);
    printViolation(rule, line, file, instName, cellName);
  }
}

void checkInoutInputOnlyShift(map<int, map<string, string> > & table, ivl_signal_t & sig)
{
  int rule;
  const char *sAct = "active";
  int line;
  const char *file;
  const char *sigName = ivl_signal_basename(sig);
  if(ivl_signal_port(sig) == IVL_SIP_INOUT)
  {
    rule = 1038;
    if (table[rule][sAct] == "yes")
    {
      if(ivl_signal_type(sig) == IVL_SIT_TRI ||
         ivl_signal_type(sig) == IVL_SIT_TRI0 ||
         ivl_signal_type(sig) == IVL_SIT_TRI1 ||
         ivl_signal_type(sig) == IVL_SIT_TRIOR ||
         ivl_signal_type(sig) == IVL_SIT_TRIAND)
      {
        line = ivl_signal_lineno(sig);
        file = ivl_signal_file(sig);
        printViolation(rule, line, file, sigName);
      }
    }

    rule = 1037;
    if (table[rule][sAct] == "yes")
    {
      unsigned elements = ivl_signal_array_count(sig);
      for (int i = 0; i < elements; i++)
      {
        ivl_nexus_t aJoint = ivl_signal_nex(sig, i);
        unsigned connections = ivl_nexus_ptrs(aJoint);
        for (int j = 0; j < connections; j++)
        {
          ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
          ivl_net_logic_t aLog = ivl_nexus_ptr_log(aConn);
          if (aLog && (ivl_nexus_ptr_pin(aConn) == 0))
          {
            line = ivl_logic_lineno(aLog);
            file = ivl_logic_file(aLog);
            printViolation(rule, line, file, sigName);
          }
          ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(aConn);
          if (anLpm && (ivl_lpm_q(anLpm) == aJoint))
          {
            line = ivl_lpm_lineno(anLpm);
            file = ivl_lpm_file(anLpm);
            printViolation(rule, line, file, sigName);
          }
        }
      }
    }
  }
}

void checkSpecialTypePortConnectedtoanExpression(map<int, map<string, string> > & table, ivl_lpm_t &lpm)
{
  int rule = 1023;
  const char *sAct = "active";
  int line = 0;
  const char *file = NULL;
  if (table[rule][sAct] == "yes")
  {
    if ((ivl_lpm_type(lpm) == IVL_LPM_MUX) || (ivl_lpm_type(lpm) == IVL_LPM_ARRAY))
    {
      ivl_nexus_t theSelJoint = ivl_lpm_select(lpm);
      unsigned connections = ivl_nexus_ptrs(theSelJoint);
      for (int j = 0; j < connections; j++)
      {
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(theSelJoint, j);
        ivl_net_logic_t aLog = ivl_nexus_ptr_log(aConn);
        if (aLog)
        {
          line = ivl_logic_lineno(aLog);
          file = ivl_logic_file(aLog);
          printViolation(rule, line, file);
        }
        ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(aConn);
        if (anLpm && (anLpm != lpm))
        {
          line = ivl_lpm_lineno(anLpm);
          file = ivl_lpm_file(anLpm);
          printViolation(rule, line, file);
        }
      }
    }
  }
}

void checkNOoutput(map<int, map<string, string> > & table, ivl_scope_t & myScope)
{
  int rule = 1012;
  const char *sAct = "active";
  int line = 0;
  const char *file = NULL;
  const char *modName = NULL;

  if (table[rule][sAct] == "yes")
  {
    unsigned char outPortFound = 0;
    unsigned modulePorts = ivl_scope_mod_module_ports(myScope);
    for (int i = 0; i < modulePorts; i++)
    {
      if ((ivl_scope_mod_module_port_type(myScope, i) == IVL_SIP_OUTPUT) ||
          (ivl_scope_mod_module_port_type(myScope, 1) == IVL_SIP_INOUT))
      {
        outPortFound = 1;
        break; // atleast one output port found
      }
    }
    if (!outPortFound)
    {
      line = ivl_scope_def_lineno(myScope);
      file = ivl_scope_def_file(myScope);
      modName = ivl_scope_tname(myScope);
      printViolation(rule, line, file, modName);
    }
  }
}

ivl_net_logic_t comingFrmComb(map<int, map<string, string> > & table, ivl_nexus_t jnt, bool isClk)
{
  int rule = 0;
  const char *sAct = "active";
  int line = 0;
  const char *file = NULL;
  const char *logicName = NULL;
  ivl_nexus_t aJoint = NULL;
  ivl_nexus_ptr_t aConn = NULL;
  ivl_net_logic_t aLogic = NULL;

  unsigned connections = ivl_nexus_ptrs(jnt);
  for (int j = 0; j < connections; j++)
  {
    aConn = ivl_nexus_ptr(jnt, j);
    if (ivl_nexus_ptr_log(aConn))
    {
      aLogic = ivl_nexus_ptr_log(aConn);
      if (ivl_logic_type(aLogic) == IVL_LO_AND ||
          ivl_logic_type(aLogic) == IVL_LO_NAND ||
          ivl_logic_type(aLogic) == IVL_LO_OR ||
          ivl_logic_type(aLogic) == IVL_LO_NOR ||
          ivl_logic_type(aLogic) == IVL_LO_XOR ||
          ivl_logic_type(aLogic) == IVL_LO_XNOR)
      {
        if (ivl_logic_pin(aLogic, 0) == jnt)
        {
          break;
        }
      } else if (ivl_logic_type(aLogic) == IVL_LO_NOT)
      {
        if (ivl_logic_pin(aLogic, 0) == jnt)
        {
          rule = 1014;
          if (isClk && table[rule][sAct] == "yes")
          {
            line = ivl_logic_lineno(aLogic);
            file = ivl_logic_file(aLogic);
            logicName = ivl_logic_basename(aLogic);
            printViolation(rule, line, file, logicName);
          }
          aJoint = ivl_logic_pin(aLogic, 1);
          aLogic = comingFrmComb(table, aJoint, isClk);
        } else {
          aLogic = NULL;
        }

      } else if (ivl_logic_type(aLogic) == IVL_LO_BUF)
      {
        if (ivl_logic_pin(aLogic, 0) == jnt)
        {
          rule = 1015;
          if (isClk && table[rule][sAct] == "yes")
          {
            line = ivl_logic_lineno(aLogic);
            file = ivl_logic_file(aLogic);
            logicName = ivl_logic_basename(aLogic);
            printViolation(rule, line, file, logicName);
          }
          aJoint = ivl_logic_pin(aLogic, 1);
          aLogic = comingFrmComb(table, aJoint, isClk);
        } else {
          aLogic = NULL;
        }
      } else {
        aLogic = NULL;
      }
    }
  }
  return aLogic;
}

void checkGlitch(map<int, map<string, string> > & table, ivl_lpm_t & lpm)
{
  int rule = 0;
  const char *sAct = "active";
  int line = 0;
  const char *file = NULL;
  const char *logicName = NULL;
  ivl_nexus_t aJoint = NULL;
  ivl_net_logic_t aLogic = NULL;

  if (ivl_lpm_type(lpm) == IVL_LPM_FF)
  {
    if (ivl_lpm_clk(lpm))
    {
      rule = 1013;
      aJoint = ivl_lpm_clk(lpm);
      aLogic = comingFrmComb(table, aJoint, true);
      if (aLogic)
      {
        line = ivl_logic_lineno(aLogic);
        file = ivl_logic_file(aLogic);
        logicName = ivl_logic_basename(aLogic);
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
      }
    }
    if (ivl_lpm_sync_clr(lpm))
    {
      aJoint = ivl_lpm_sync_clr(lpm);
      aLogic = comingFrmComb(table, aJoint, false);
      if (aLogic)
      {
        line = ivl_logic_lineno(aLogic);
        file = ivl_logic_file(aLogic);
        logicName = ivl_logic_basename(aLogic);
        rule = 1011;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
        rule = 1017;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
      }
    }
    if (ivl_lpm_async_clr(lpm))
    {
      aJoint = ivl_lpm_async_clr(lpm);
      aLogic = comingFrmComb(table, aJoint, false);
      if (aLogic)
      {
        line = ivl_logic_lineno(aLogic);
        file = ivl_logic_file(aLogic);
        logicName = ivl_logic_basename(aLogic);
        rule = 1011;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
        rule = 1017;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
      }
    }
    if (ivl_lpm_sync_set(lpm))
    {
      aJoint = ivl_lpm_sync_set(lpm);
      aLogic = comingFrmComb(table, aJoint, false);
      if (aLogic)
      {
        line = ivl_logic_lineno(aLogic);
        file = ivl_logic_file(aLogic);
        logicName = ivl_logic_basename(aLogic);
        rule = 1011;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
        rule = 1016;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
      }
    }
    if (ivl_lpm_async_set(lpm))
    {
      aJoint = ivl_lpm_async_set(lpm);
      aLogic = comingFrmComb(table, aJoint, false);
      if (aLogic)
      {
        line = ivl_logic_lineno(aLogic);
        file = ivl_logic_file(aLogic);
        logicName = ivl_logic_basename(aLogic);
        rule = 1011;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
        rule = 1016;
        if (table[rule][sAct] == "yes")
          printViolation(rule, line, file, logicName);
      }
    }
  }
}

void checkGATE(map<int, map<string, string> > & table, ivl_net_logic_t & gate)
{
  int rule;
  const char *sAct = "active";
  int line = ivl_logic_lineno(gate);
  const char *file = ivl_logic_file(gate);
  const char *logicName = ivl_logic_basename(gate);

  if (ivl_logic_scope(gate))
  {
    if (!ivl_scope_parent(ivl_logic_scope(gate)))
    {
      rule = 1178; // same as 1249, not implemented
      if (table[rule][sAct] == "yes")
      {
        printViolation(rule, line, file, logicName);
      }
    }
  }
  unsigned pins = ivl_logic_pins(gate);
  for (int i = 0; i < pins; i++)
  {
    ivl_nexus_t aJoint = ivl_logic_pin(gate, i);
    unsigned connections = ivl_nexus_ptrs(aJoint);
    for (int j = 0; j < connections; j++)
    {
      ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
      ivl_lpm_t anLPM = ivl_nexus_ptr_lpm(aConn);
      if (anLPM)
      {
        const char *lpmName = ivl_lpm_basename(anLPM);
     // this is not enough, we have to get LPM name from LPM type.
        line = ivl_lpm_lineno(anLPM);
        rule = 1006;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, logicName, lpmName);
        }
      }
      ivl_signal_t aSig = ivl_nexus_ptr_sig(aConn);
      if(aSig && ivl_signal_local(aSig))
      {
        rule = 1241;
        if (table[rule][sAct] == "yes")
        {
          const char *constBits = NULL;
          for (int k = j + 1; k < connections; k++)
          {
            ivl_nexus_ptr_t aConstCon = ivl_nexus_ptr(aJoint, k);
            ivl_net_const_t aConst = ivl_nexus_ptr_con(aConstCon);
	    if (aConst)
	      constBits = ivl_const_bits(aConst);
          }
          line = ivl_logic_lineno(gate);
          printViolation(rule, line, file, logicName, constBits);
        }
      }
    }
  }
}

void SignalAssigned(map<int, map<string, string> > & table, ivl_signal_t aSig)
{
  bool comingFrom = false;
  bool goingTo = false;
  unsigned line = ivl_signal_lineno(aSig);
  const char *file = ivl_signal_file(aSig);
  const char *aSigName = ivl_signal_basename(aSig);
  unsigned elements = ivl_signal_array_count(aSig);
  for (int i = 0; i < elements; i++)
  {
    ivl_nexus_t aNex = ivl_signal_nex(aSig, i);
    unsigned connections = aNex ? ivl_nexus_ptrs(aNex) : 0;
    for (int j = 0; j < connections; j++)
    {
      ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aNex, j);
      ivl_signal_t mySig = ivl_nexus_ptr_sig(aConn);
      if (mySig && (mySig != aSig))
      {
        comingFrom = true;
        goingTo = true;
      }
      ivl_net_logic_t aLog = ivl_nexus_ptr_log(aConn);
      if (aLog)
      {
        if (ivl_nexus_ptr_pin(aConn) == 0)
          comingFrom = true;
        else
          goingTo = true;
      }
      ivl_lpm_t anLpm = ivl_nexus_ptr_lpm(aConn);
      if (anLpm)
      {
        if (ivl_lpm_q(anLpm) == aNex)
          comingFrom = true;
        else
          goingTo = true;
      }
      ivl_branch_t aBrnc = ivl_nexus_ptr_branch(aConn);
      if( aBrnc)
      {
        if (ivl_branch_terminal(aBrnc, 0) == aNex)
          comingFrom = true;
        else
          goingTo = true;
      }
      ivl_switch_t aSwc = ivl_nexus_ptr_switch(aConn);
      if (aSwc)
      {
        if (ivl_switch_b(aSwc) == aNex)
          comingFrom = true;
        else
          goingTo = true;
      }
    }
  }
  const char *sAct = "active";
  int rule = 1058;
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_port(aSig) != IVL_SIP_INPUT)
    {
      if (!comingFrom)
        printViolation(rule, line, file, aSigName);
    }
  }
  rule = 1059;
  if (table[rule][sAct] == "yes")
  {
    if (ivl_signal_port(aSig) != IVL_SIP_OUTPUT)
    {
      if (!goingTo)
        printViolation(rule, line, file, aSigName);
    }
  }
}

void checkPin(map<int, map<string, string> > & table, ivl_signal_t & sig)
{
  int rule = 1005; //same as 1227, 1228, not implemented
  const char *sAct = "active";

  const char* signameHier = ivl_signal_name(sig);
  int line = 0;
  const char *file = NULL;

  if ((table[rule][sAct] == "yes"))
  {
    unsigned sigWidth = ivl_signal_width(sig);
    unsigned elements = ivl_signal_array_count(sig);
    for (int i = 0; i < elements; i++)
    {
      ivl_nexus_t aJoint = ivl_signal_nex(sig, i);
      unsigned connections = 0;
      if (aJoint)
        connections = ivl_nexus_ptrs(aJoint);

      for (int j = 0; j < connections; j++)
      {
        unsigned connWidth = 0;
        ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
        if (ivl_nexus_ptr_con(aConn))
        {
          ivl_net_const_t aConst = ivl_nexus_ptr_con(aConn);
          line = ivl_const_lineno(aConst);
          file = ivl_const_file(aConst);
          connWidth = ivl_const_width(aConst);
        }
        if (ivl_nexus_ptr_log(aConn))
        {
          ivl_net_logic_t aLogic = ivl_nexus_ptr_log(aConn);
          line = ivl_logic_lineno(aLogic);
          file = ivl_logic_file(aLogic);
          connWidth = ivl_logic_width(aLogic);
        }
        if (ivl_nexus_ptr_lpm(aConn))
        {
          ivl_lpm_t anLPM = ivl_nexus_ptr_lpm(aConn);
          line = ivl_lpm_lineno(anLPM);
          file = ivl_lpm_file(anLPM);
          if ((ivl_lpm_type(anLPM) == IVL_LPM_FF))
          {
            if ((ivl_lpm_clk(anLPM) == aJoint) ||
                (ivl_lpm_enable(anLPM) == aJoint) ||
                (ivl_lpm_sync_set(anLPM) == aJoint) ||
                (ivl_lpm_sync_clr(anLPM) == aJoint) ||
                (ivl_lpm_async_set(anLPM) == aJoint) ||
                (ivl_lpm_async_clr(anLPM) == aJoint))
            {
              connWidth = 1;
            }
          }
          if ((ivl_lpm_type(anLPM) == IVL_LPM_MUX) || (ivl_lpm_type(anLPM) == IVL_LPM_ARRAY))
          {
            if (ivl_lpm_select(anLPM) == aJoint)
            {
              connWidth = ivl_lpm_selects(anLPM);
            }
          }
// assertion problem              (ivl_lpm_data(anLPM, 1) == aJoint))
          if ((ivl_lpm_q(anLPM) == aJoint) ||
              (ivl_lpm_data(anLPM, 0) == aJoint))
          {
            connWidth = ivl_lpm_width(anLPM);
          }
        }
        if (ivl_nexus_ptr_switch(aConn))
        {
          ivl_switch_t aSwitch = ivl_nexus_ptr_switch(aConn);
          line = ivl_switch_lineno(aSwitch);
          file = ivl_switch_file(aSwitch);
          connWidth = ivl_switch_width(aSwitch);
          if (!connWidth)
          {
            connWidth = 1;
          }
        }
        if (ivl_nexus_ptr_sig(aConn))
        {
          ivl_signal_t anothrSig = ivl_nexus_ptr_sig(aConn);
          line = ivl_signal_lineno(anothrSig);
          file = ivl_signal_file(anothrSig);
          connWidth = ivl_signal_width(anothrSig);
        }
        if (sigWidth != connWidth)
          printViolation(rule, line, file, signameHier, (connections - 1));
      }
    }
  }
}

void checkSignalDrivers(map<int, map<string, string> > & table, ivl_signal_t & sig)
{
  int rule;
  const char *sAct = "active";

  const char* signameHier = ivl_signal_name(sig);
  int line = ivl_signal_lineno(sig);
  const char *file = ivl_signal_file(sig);

  if (ivl_signal_port(sig) != IVL_SIP_INPUT)
  {
    unsigned elements = ivl_signal_array_count(sig);
    for (int i = 0; i < elements; i++)
    {
      ivl_nexus_t aJoint = ivl_signal_nex(sig, i);
      unsigned connections = 0;
      if (aJoint)
        connections = ivl_nexus_ptrs(aJoint);

      if (connections > 2)
      {
        rule = 1004;
        if (table[rule][sAct] == "yes")
        {
          printViolation(rule, line, file, signameHier, (connections - 1));
        }
        for (int j = 0; j < connections; j++)
        {
          ivl_nexus_ptr_t aConn = ivl_nexus_ptr(aJoint, j);
          ivl_lpm_t anLPM = ivl_nexus_ptr_lpm(aConn);
          if (anLPM)
          {
            line = ivl_lpm_lineno(anLPM);
            if (ivl_lpm_type(anLPM) == IVL_LPM_MUX)
            {
              rule = 1008;
              if (table[rule][sAct] == "yes")
              {
                // e ? i2 : 1'bz type of definition will be MUX lpm
                printViolation(rule, line, file, signameHier);
              }
            }
            if (ivl_lpm_type(anLPM) == IVL_LPM_PART_VP)
            {
	      ivl_nexus_t partInNex = ivl_lpm_data(anLPM, 0);
	      ivl_nexus_t partOutNex = ivl_lpm_q(anLPM);
              for (int k = 0; k < ivl_nexus_ptrs(partOutNex); k++)
              {
                ivl_nexus_ptr_t aPartOutConn = ivl_nexus_ptr(partOutNex, k);
		ivl_signal_t aPartOutSig = ivl_nexus_ptr_sig(aPartOutConn);
                if (aPartOutSig)
	        {
                  for (int l = 0; l < ivl_nexus_ptrs(partInNex); l++)
                  {
                    ivl_nexus_ptr_t aPartInConn = ivl_nexus_ptr(partInNex, l);
		    if (ivl_nexus_ptr_sig(aPartInConn) == aPartOutSig)
	            {
                      rule = 1075;
                      if (table[rule][sAct] == "yes")
                      {
                        printViolation(rule, line, file, signameHier);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}

