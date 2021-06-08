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

#ifndef LINT_EXT_H
#define LINT_EXT_H

#include "general.h"

typedef struct tricToPasVoidStar
{
  map<int, map<string, string>> configTable;
  set <ivl_signal_t> allAssnSigs;
  bool firsTime;
} chkProsVoid;

extern int logViolationCount(int incr = 0);
extern FILE * openLogFile(const char *fileName = NULL);
extern void readConfiguration(map< int, map<string, string> > & table);
extern void printViolation(int ruleNo, int lineNo, const char* vlogFileName, ...);

/**************************************** ivl process related ********************************/
extern void IBlock(map<int, map<string, string> > & table, ivl_process_t proc);
extern void checkProcesStatement(map<int, map<string, string>> &table, ivl_statement_t net, ivl_signal_t &loopVar, set<ivl_signal_t> &senLst, set<ivl_signal_t> &sigSet, bool eg = false, bool time = false);
extern void checkUnasndVar(map<int, map<string, string>> &table, ivl_statement_t net, set<ivl_signal_t> &allSigSet);
/**************************************** ivl_event_t ********************************/
extern void checkEdgeNonEdge(map<int, map<string, string> > & table, ivl_event_t & evt);
extern void checkActiveSignalName(map<int, map<string, string> > & table, ivl_event_t & net);
extern bool checkEvent(map<int, map<string, string> > & table, ivl_event_t & evt, set<ivl_signal_t> &senLst);
/**************************************** ivl_parameter_t ********************************/
extern void checkPMETER(map<int, map<string, string> > & table , ivl_parameter_t & param);
extern void checkPmeterWidth(map<int, map<string, string> > & table, ivl_parameter_t & param);
extern void checkPmeterNameLength(map<int, map<string, string> > & table, ivl_parameter_t & param);
extern void checkPmeterPrefixSuffix(map<int, map<string, string> > & table, ivl_parameter_t & param);
/**************************************** ivl_net_logic_t ********************************/
extern void checkGATE(map<int, map<string, string> > & table,ivl_net_logic_t & gate);
extern void checkReconvClock(map<int, map<string, string> > & table, ivl_net_logic_t & log);
extern void checkGatePrefixSuffix(map<int, map<string, string> > & table, ivl_net_logic_t & gate);
extern void checkUDPInstanceNotSynthesizable(map<int, map<string, string> > & table,ivl_net_logic_t & net);
extern void checkInputsFromDifferentClockSource(map<int, map<string, string> > & table, ivl_net_logic_t & combGate);
/************************ ivl_scope_t *****************************/
extern void checkTaskName(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkNOoutput(map<int, map<string, string> > & table, ivl_scope_t & myScp);
extern void checkModuleName(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkEmptyModule(map<int, map<string, string> > & table, ivl_scope_t & myMod);
extern void checkFunctionName(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkLibraryCellInst(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkForkStatementNotSynthesizable(map<int, map<string, string> > & table, ivl_scope_t scope);
/**************************************** ivl_statement_t ********************************/
extern void WhileLoop(map<int, map<string, string>> &table, ivl_statement_t net);
extern void checkUserTask(map<int, map<string, string> > & table, ivl_statement_t net);
extern void SystemTaskCall(map<int, map<string, string> > & table, ivl_statement_t net);
extern void checkNetStuck(map<int, map<string, string> > & table, ivl_statement_t & myStmt);
extern void checkRepeatExpression(map<int, map<string, string> > & table, ivl_statement_t net);
extern void MissingProcessLabelName(map<int, map<string, string> > & table, ivl_statement_t net);
extern void checkComboInSequential(map<int, map<string, string> > & table, ivl_statement_t stmt);
extern void checkNestedEvents(map<int, map<string, string> > & table, ivl_statement_t net, bool time = false);
extern void ProceduralContinuousAssignmentNotSynthesizable(map<int, map<string, string>> &table, ivl_statement_t net);
extern void ForceStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigList = NULL);
extern void ReleaseStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigList = NULL);
extern void DelayControl(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> &senLst, set<ivl_signal_t> &sigSet);
/************************ ivl_signal_t *****************************/
extern void checkPin(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkMemory(map<int, map<string, string> > & table, ivl_signal_t & mem);
extern void checkWire(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void SignalAssigned(map<int, map<string, string> > & table, ivl_signal_t sig);
extern void checkNetType(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkRegister(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkDriveSynth(map<int, map<string, string> > & table, ivl_signal_t &mySig);
extern void checkSignalName(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignalUnconnected(map<int, map<string, string> > &table, ivl_signal_t net);
extern void checkSignalNoLoad(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkVariableName(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignalDrivers(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkIntegerFunction(map<int, map<string, string> > & table, ivl_signal_t & port);
extern void checkSignalDrivConst(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void variable_synthesizeble(map<int, map<string, string> > & table, ivl_signal_t & net);
extern void checkBiDirecTionalPort(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkInoutInputOnlyShift(map<int, map<string, string> > & table, ivl_signal_t & sig);
extern void checkVhdlReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkIntegerConcatenation(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkCombinationalPiPoPath(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkAssignmentInputSignal(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSystemVerilogReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void SignalConnectedBothInputOutputPorts(map<int, map<string, string> > & table, ivl_signal_t mySig);
/**************************************** ivl_lpm_t ********************************/
extern void checkLPM(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkGlitch(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkTriEnb(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockSet(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkNoSetReset(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkSetDataInput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockSeqLogic(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkSyncAsyncReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkEnableSetReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkTristateBuffers(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkRegPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkNonConstDivisor(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkSetPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkResetPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkClockSignalOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkResetSignalOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkFallingActiveClock(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkClockActiveBothEdges(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkLatchNamePrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkTestClockPrimaryInput(map<int, map<string, string> > & table, ivl_lpm_t & sig);
extern void checkSpecialSignalBitSelect(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkPossibleLossofCarryorBorrow(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkSpecialTypePortConnectedtoanExpression(map<int, map<string, string> > & table, ivl_lpm_t &lpm);

#endif
