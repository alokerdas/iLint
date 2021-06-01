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
#include "general.h"
#include "lint.h"

int checkProcess(ivl_process_t pros, void *X)
{
  int fail = 0;
  set <ivl_signal_t> senLst;
  set <ivl_signal_t> sigSet;
  ivl_signal_t loopIdx = NULL;
  chkProsVoid *voidVar = (chkProsVoid*) X;
  if (voidVar->firsTime)
  {
    IBlock(voidVar->configTable, pros);
    checkProcesStatement(voidVar->configTable, ivl_process_stmt(pros), loopIdx, senLst, sigSet, false, true);
    voidVar->allAssnSigs.insert(sigSet.begin(), sigSet.end());
  }
  else
  {
    checkUnasndVar(voidVar->configTable, ivl_process_stmt(pros), voidVar->allAssnSigs);
  }
  return fail;
}

int draw_scope_port(map<int, map<string, string> > & table, ivl_scope_t scope)
{
  if (ivl_scope_type(scope) == IVL_SCT_MODULE)
  {
    checkNOoutput(table, scope);
    checkModuleName(table, scope);
    checkEmptyModule(table, scope);
  }

  if (ivl_scope_type(scope) == IVL_SCT_TASK)
  {
    checkTaskName(table, scope);
  }

  if (ivl_scope_type(scope) == IVL_SCT_FUNCTION)
  {
    checkFunctionName(table, scope);
    ivl_signal_t retVal = ivl_scope_port(scope, 0);
    checkIntegerFunction(table, retVal);
  }

  if (ivl_scope_type(scope) == IVL_SCT_FORK)
  {
    checkForkStatementNotSynthesizable(table, scope);
  }

  if (ivl_scope_is_cell(scope))
  {
    checkLibraryCellInst(table, scope);
  }

  unsigned scopePorts = ivl_scope_ports(scope);
  for (int i = 0; i < scopePorts; i++)
  {
    int rule = 1313;
    const char *sAct = "active";
    if (table[rule][sAct] == "yes")
    {
      int aline = ivl_scope_lineno(scope);
      const char *afile = ivl_scope_file(scope);
      const char *portSig = NULL;
      if (ivl_scope_type(scope) == IVL_SCT_MODULE)
        const char *portSig = ivl_scope_mod_module_port_name(scope, i);
      // there is no suitable API to implement 1313
      //printViolation(rule, aline, afile, portSig);
    }
  }

  unsigned sigs = ivl_scope_sigs(scope);
  for (int i = 0; i < sigs; i++)
  {
    ivl_signal_t mySig = ivl_scope_sig(scope, i);
    if (!ivl_signal_local(mySig))
    {
      checkPin(table, mySig);
      checkWire(table, mySig);  
      checkMemory(table, mySig);  
      checkNetType(table, mySig);  
      checkRegister(table, mySig);
      SignalAssigned(table, mySig);
      checkDriveSynth(table, mySig);
      checkSignalName(table, mySig);
      checkSignalNoLoad(table, mySig);
      checkVariableName(table, mySig);  
      checkSignalDrivers(table, mySig);
      checkSignalDrivConst(table, mySig);  
      checkSignalUnconnected(table, mySig);
      checkVhdlReservedWords(table, mySig); 
      checkBiDirecTionalPort(table, mySig);  
      variable_synthesizeble(table, mySig);
      checkInoutInputOnlyShift(table, mySig);
      checkIntegerConcatenation(table, mySig);  
      checkCombinationalPiPoPath(table, mySig);  
      checkAssignmentInputSignal(table, mySig);  
      checkSystemVerilogReservedWords(table, mySig);  
      SignalConnectedBothInputOutputPorts(table, mySig);
    }
  }

  unsigned logics = ivl_scope_logs(scope);
  for (int j  = 0; j < logics; j++)
  {
    ivl_net_logic_t gate = ivl_scope_log(scope, j);
    checkGATE(table, gate);
    checkGatePrefixSuffix(table, gate);
    checkUDPInstanceNotSynthesizable(table, gate);
    checkReconvClock(table, gate);
    checkInputsFromDifferentClockSource(table, gate);
  }

  unsigned lpms = ivl_scope_lpms(scope);
  for (int k = 0; k < lpms; k++)
  {
    ivl_lpm_t lpm = ivl_scope_lpm(scope, k);
    checkLPM(table, lpm);
    checkTriEnb(table, lpm);
    checkGlitch(table, lpm);
    checkClockSet(table, lpm);
    checkNoSetReset(table, lpm);
    checkSetDataInput(table, lpm);
    checkClockSeqLogic(table, lpm);
    checkEnableSetReset(table, lpm);
    checkSyncAsyncReset(table, lpm);
    checkNonConstDivisor(table, lpm);
    checkRegPrefixSuffix(table, lpm);
    checkSetPrefixSuffix(table, lpm);
    checkTristateBuffers(table, lpm);
    checkResetPrefixSuffix(table, lpm);
    checkClockSignalOutput(table, lpm);
    checkResetSignalOutput(table, lpm);
    checkFallingActiveClock(table, lpm);
    checkClockActiveBothEdges(table, lpm);
    checkTestClockPrimaryInput(table, lpm);
    checkLatchNamePrefixSuffix(table, lpm);
    checkSpecialSignalBitSelect(table, lpm);
    checkPossibleLossofCarryorBorrow(table, lpm);
    checkSpecialTypePortConnectedtoanExpression(table, lpm);
  }

  unsigned params = ivl_scope_params(scope);
  for (int k  = 0; k < params; k++)
  {
    ivl_parameter_t param  = ivl_scope_param(scope, k);
    checkPMETER(table, param);
    checkPmeterWidth(table, param);
    checkPmeterNameLength(table, param);
    checkPmeterPrefixSuffix(table, param); 
  }

  for (int j = 0; j < ivl_scope_events(scope); j++)
  { 
    ivl_event_t evt = ivl_scope_event(scope, j);
    if (ivl_event_nany(evt) || ivl_event_npos(evt) || ivl_event_nneg(evt))
    {
      set<ivl_signal_t> dummy;
      checkEvent(table, evt, dummy);
      checkEdgeNonEdge(table, evt);
      checkActiveSignalName(table, evt);
    }
    else
    {
      int rule = 1080;
      const char *sAct = "active";
      if (table[rule][sAct] == "yes")
      {
        int aline = ivl_event_lineno(evt);
        const char *afile = ivl_event_file(evt);
        printViolation(rule, aline, afile, ivl_event_basename(evt));
      }
    }
  }

  int timPreci;
  int timUnit = ivl_scope_time_units(scope);
  if (timUnit)
  {
    timPreci = ivl_scope_time_precision(scope);
  }
  else
  {
    int rule = 1235;
    const char *sAct = "active";
    if (table[rule][sAct] == "yes")
    {
      int aline = ivl_scope_lineno(scope);
      const char *afile = ivl_scope_file(scope);
      printViolation(rule, aline, afile, ivl_scope_name(scope));
    }
  }

  unsigned nScope = ivl_scope_childs(scope);
  for (int k = 0; k < nScope; k++)
  {
    ivl_scope_t aScope = ivl_scope_child(scope, k);
    draw_scope_port(table, aScope);
  }
  return 0;
}

void checkMultipleTopModule(map<int, map<string, string> > & table, ivl_scope_t scp)
{
  int rule = 1088;
  const char *sAct = "active";
  int line = ivl_scope_lineno(scp);
  const char *file = ivl_scope_file(scp);
  if (table[rule][sAct] == "yes")
  {
    printViolation(rule, line, file);
  }
}

int target_design(ivl_design_t des)
{
  chkProsVoid proStruct;
  readConfiguration(proStruct.configTable);

  printf("\n\n\n");
  FILE *logFptr = openLogFile("lint.log");

  unsigned nroot = 0;
  ivl_scope_t *root_scopes;
  ivl_design_roots(des, &root_scopes, &nroot);

  for (int idx = 0 ;  idx < nroot ;  idx++)
  {
    ivl_scope_t aScope = root_scopes[idx];
    if(nroot > 1)
      checkMultipleTopModule(proStruct.configTable, aScope);
 
    draw_scope_port(proStruct.configTable, aScope);
  }
  proStruct.firsTime = true;
  ivl_design_process(des, checkProcess, &proStruct);
  proStruct.firsTime = false;
  ivl_design_process(des, checkProcess, &proStruct);

  fprintf(logFptr, "TOTAL NUMBER OF VIOLATIONS ARE: %d.\n", logViolationCount());
  printf("TOTAL NUMBER OF VIOLATIONS ARE: %d.\n", logViolationCount());
  fclose(logFptr);

  return 0;
}
