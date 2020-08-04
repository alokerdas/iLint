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

int check_process(ivl_process_t pros, void *X)
{
  int fail = 0;
  map<int, map<string, string> > *confTable = (map<int, map<string, string> >*) X;
  map<int, map<string, string> > table = *confTable;
  IBlock(table, pros);

  set <ivl_signal_t> *senLst = new set<ivl_signal_t>;
  checkProcesStatement(table, ivl_process_stmt(pros), senLst);
  delete senLst;
  return fail;
}

int draw_scope_port(map<int, map<string, string> > & table, ivl_scope_t scope)
{
  if (ivl_scope_type(scope) == IVL_SCT_MODULE)
  {
    checkNOoutput(table, scope);
    checkModuleName(table, scope);
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

  if (ivl_scope_is_cell(scope))
  {
    checkLibraryCellInst(table, scope);
  }

  unsigned scopePorts = ivl_scope_ports(scope);
  for (int i = 0; i < scopePorts; i++)
  {
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
      checkVhdlReservedWords(table, mySig); 
      checkBiDirecTionalPort(table, mySig);  
      variable_synthesizeble(table, mySig);
      checkInoutInputOnlyShift(table, mySig);
      checkIntegerConcatenation(table, mySig);  
      checkCombinationalPiPoPath(table, mySig);  
      checkAssignmentInputSignal(table, mySig);  
      checkSystemVerilogReservedWords(table, mySig);  
    }
  }

  unsigned logics = ivl_scope_logs(scope);
  for (int j  = 0; j < logics; j++)
  {
    ivl_net_logic_t gate = ivl_scope_log(scope, j);
    checkGATE(table, gate);
    checkUDPInstanceNotSynthesizable(table, gate);
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
    checkResetPrefixSuffix(table, lpm);
    checkClockSignalOutput(table, lpm);
    checkFallingActiveClock(table, lpm);
    checkClockActiveBothEdges(table, lpm);
    checkTestClockPrimaryInput(table, lpm);
    checkSpecialSignalBitSelect(table, lpm);
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
      checkEvent(table, evt, NULL);
      checkEdgeNonEdge(table, evt);
      checkMultipleClock(table, evt);
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
    printViolation(rule, line, file);
}

int target_design(ivl_design_t des)
{
  map< int, map<string, string> > configTable;
  readConfiguration(configTable);

  printf("\n\n\n");
  FILE *logFptr = openLogFile("lint.log");

  unsigned nroot = 0;
  ivl_scope_t *root_scopes;
  ivl_design_roots(des, &root_scopes, &nroot);

  for (int idx = 0 ;  idx < nroot ;  idx++)
  {
    ivl_scope_t aScope = root_scopes[idx];
    if(nroot > 1)
      checkMultipleTopModule(configTable, aScope);
 
    draw_scope_port(configTable, aScope);
  }
  ivl_design_process(des, check_process, &configTable);

  fprintf(logFptr, "TOTAL NUMBER OF VIOLATIONS ARE: %d.\n", logViolationCount());
  printf("TOTAL NUMBER OF VIOLATIONS ARE: %d.\n", logViolationCount());
  fclose(logFptr);

  return 0;
}
