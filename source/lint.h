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

extern FILE * openLogFile(const char *fileName = NULL);
extern void readConfiguration(map< int, map<string, string> > & table);
extern int logViolationCount(int incr = 0);
extern void printViolation(int ruleNo, int lineNo, const char* vlogFileName, ...);

/************************ ivl_signal_t net *****************************/
extern void checkSignalName(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSystemVerilogReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkVerilogReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkVhdlReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkBiDirecTionalPort(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkNetType(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignedUnSigned(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkRegister(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkNOoutput(map<int, map<string, string> > & table, ivl_scope_t & mySig);
extern void checkDirectInputOutput(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkInoutInputOnlyShift(map<int, map<string, string> > & table, ivl_signal_t & sig);
extern void show_tristste_other_logic(map<int, map<string, string> > & table,ivl_signal_t & net);


/************************ ivl_lpm_t net *****************************/
extern void checkIntegerConcatenation(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void Expression_Connected_to_an_Instance_Port(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void VariableswithDifferentBitWidthsUsedinConditionalAssignmentBranches(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void  PossibleLossofCarryorBorrowinAddition(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void Possible_Loss_Value_in_Multiplication(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void BitWidthMismatchinLogicalOperation(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void LargeBitWidthofReductionOperand(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void Tri_state_Enable_Mixed_with_Other(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void Combinational_Path_Between_PI_and_PO(map<int, map<string, string> > & table, ivl_lpm_t net);
extern void show_lpm_re_xnor(map<int, map<string, string> > & table, ivl_lpm_t net);



/************************ ivl_parameter_t net *****************************/
extern void checkPMETER(map<int, map<string, string> > & table , ivl_parameter_t & param);

extern void checkIntegerFunction(map<int, map<string, string> > & table, ivl_signal_t & port);
extern void InstanceNameRelatedModuleName(map<int, map<string, string> > & table, ivl_scope_t scope);


/************************ ivl_net_logic_t net *****************************/
extern void checkUDPInstanceNotSynthesizable(map<int, map<string, string> > & table,ivl_net_logic_t & net);
extern void checkUDP(map<int, map<string, string> > & table, ivl_net_logic_t & gate);
extern void Tri_state_Output_Mixed_with_Other_Logic_in_a_Module(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void checkInputsFromDifferentClockSource(map<int, map<string, string> > & table, ivl_net_logic_t & combGate);


/************************ ivl_scope_t net *****************************/
extern void checkLibraryCellInst(map<int, map<string, string> > & table, ivl_scope_t & scope);


/************************ ivl_event_t net *****************************/
extern void checkDuplicate(map<int, map<string, string> > & table, ivl_event_t & evt);


extern const char* LPM_FF_CLOCK_POS(ivl_statement_t net);
extern const char* LPM_FF_OUTPUT(ivl_statement_t net);
extern const char* LPM_FF_INPUT(ivl_statement_t net);
extern const char* LPM_FF_INPUT_IF(ivl_statement_t net);
extern const char* LPM_FF_INPUT_ELSE(ivl_statement_t net);
extern const char* LPM_FF_CLK_EN(ivl_statement_t net);

/************************ API FOR FF ***************************/
extern const char* ivl_lpm_pos_clk_name(ivl_statement_t net);//depreciated 
extern const char* ivl_lpm_neg_clk_name(ivl_statement_t net);//depreciated 
/************************ API FOR RESET NAME ***************************/
extern const char* ivl_lpm_reset(ivl_statement_t net);
extern const char* ivl_lpm_set(ivl_statement_t net);
extern const char* ivl_lpm_low_reset(ivl_statement_t net);
extern const char* ivl_lpm_high_reset(ivl_statement_t net);


extern const char* ivl_lpm_async_reset(ivl_statement_t net);//depreciated 
/************************ API FOR NEXUS NAME ***************************/
extern const char* ivl_name_nexus(ivl_nexus_t nex);
/************************ API FOR NEXUS TYPE ***************************/
extern int ivl_type_nexus(ivl_nexus_t nex);
/************************ API FOR NEXUS WIDTH **************************/
extern int width_of_nexus(ivl_nexus_t nex);
/************************ API FOR POSEDGE  *****************************/
extern const char* ivl_lpm_ff_pos(ivl_statement_t net);//depreciated 
/************************ API FOR NEGEDGE  *****************************/
extern const char* ivl_lpm_ff_neg(ivl_statement_t net);//depreciated 
/************************ API FOR NEGEDGE EVENT ************************/
extern ivl_event_t ivl_lpm_ff_nevt(ivl_statement_t net);
/************************ API FOR POSEDGE EVENT ************************/
extern ivl_event_t ivl_lpm_ff_pevt(ivl_statement_t net);
/************************ API FOR LATCH ************************/
extern const char* ivl_lpm_latch(ivl_statement_t net);


/************************ ivl_statement_t net,ivl_process_t pr  *****************************/
extern void XZCaseLabel(map<int, map<string, string> > & table, ivl_statement_t net);
extern void checkProcesStatement(map<int, map<string, string>> &table, ivl_statement_t net, ivl_signal_t &loopVar, set<ivl_signal_t> &senLst, set<ivl_signal_t> &sigSet, bool eg = false, bool time = false);
extern void checkUnasndVar(map<int, map<string, string>> &table, ivl_statement_t net, set<ivl_signal_t> &allSigSet);
extern void Reset_is_Driven_by_a_Path_with_Potential_Glitch(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t pr);
extern void set_is_Driven_by_Combinational_logic(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t pr);
void Reset_is_Driven_by_by_Combinational_logic(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t pr);
extern void Reset_Signal_Active_High_and_Low(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Set_Signal_Used_as_Data_Input(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Signal_Used_as_Set_Reset(map<int, map<string, string> > & table,ivl_statement_t net);
extern void No_Set_Reset(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Reset_prefix_suffix(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void SeparateClockGenerateCircuit(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void ReconvergedClock(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void ResetSignalFeedsintoPrimaryOutput(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void InferredTristate(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void InferredLatch(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
/************************ ivl_statement_t net *****************************/
extern void ivl_lpm_latch_tristate(map<int, map<string, string> > & table,ivl_statement_t net);
extern void checkComboInSequential(map<int, map<string, string> > & table, ivl_statement_t stmt);
extern void ProceduralContinuousAssignmentNotSynthesizable(map<int, map<string, string>> &table, ivl_statement_t net);
/************************ new_rules ***************************************/ 
extern void UninitializedVariableReferencedinanEdgetriggeredBlock(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void UnAssignedVariableReferencedinanEdgetriggeredBlock(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void UninitializedVariableReferencedinaCombinationalProcess(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void VariableNotFullyAssignedbeforeReferencedinCombinationalProcess(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void checkTimeScaleMissing(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void ObjectsWithSameNameofObjectinOuterScope(map<int, map<string, string> > & table, ivl_signal_t & sig);
extern void Signal_with_Heavy_Fanout_Loading(map<int, map<string, string> > & table, ivl_signal_t & log);
extern void Module_Name_Different_from_File_Name(map<int, map<string, string> > & table, ivl_scope_t & net);
extern void Bit_Width_Mismatch_in_Assignment(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void Constant_Connected_to_Instance(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void Non_constant_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Loop_Variable_Changed_in_For_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Arithmetic_Operations_in_For_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Logical_Operations_in_For_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Filp_flop_Reset_Logic_in_For_LoopL(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Infinite_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Apparent_Loop(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Gate_Instance_Found(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Wait_Statement_Not_Synthesizable(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Case_Equivalence_Operator_Not_Synthesizable(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Event_Control_or_Delay_Statement_Not_Found_in_All_Branches(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Unconventional_File_Extension(map<int, map<string, string> > & table, ivl_scope_t & net);
extern void For_Loop_Detected(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
/************************ ubantu_rules ***************************************/
extern void check_Comment_Not_Found_Following_Port_Declaration(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void check_Interspersed_Input_and_Output_Declarations(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void check_Declare_One_Signal_per_Line_with_Comment(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void check_Macromodule_Not_Synthesizable(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR MACROMODULE ************************/
extern const char* ivl_scope_macromodule(ivl_scope_t net);
extern void check_Defparam_Not_Synthesizable(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR DEFPARAM ************************/
int ivl_defparam_name(ivl_scope_t net);
extern void check_One_Port_per_Line(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR specify ************************/
extern int ivl_scope_specify(ivl_scope_t net);
extern void check_Specify_Block_Not_Synthesizable(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR include ************************/
extern const char* ivl_scope_include(ivl_scope_t net);
extern void check_include_directive(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR CONDITIONAL COMPILER DIRECTIVE ************************/
extern const char* ivl_scope_conditionalcompiler(ivl_scope_t net);
extern void check_conditionalcompiler_directive(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR CONDITIONAL UNKNOWN DIRECTIVE ************************/
extern const char* ivl_scope_unknowndirective(ivl_scope_t net);
extern void check_unkn_directive(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void check_dc_shell(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void check_Define_one_file(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR TRANSLATE ON ************************/
extern const char* ivl_scope_translateon(ivl_scope_t net);
extern int ivl_nested_translateon(ivl_scope_t net);
/************************ API FOR TRANSLATE OFF ************************/
extern const char* ivl_scope_translateoff(ivl_scope_t net);
extern int ivl_nested_translateoff(ivl_scope_t net);
extern void check_Synopsys_Directives_Translate_off_Translate_on(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR FULL_CASE ************************/
extern const char* ivl_scope_full_case(ivl_scope_t net);
extern void check_Synopsys_full_case(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void check_no_comment_end(map<int, map<string, string> > & table, ivl_scope_t & scope);
/************************ API FOR ESCAPE ************************/
extern int ivl_nested_escape(ivl_scope_t net);

//87 lint.h
/**************************************** ivl_scope_t ********************************/
extern void checkMultipleModule(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkModuleName(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkFunctionName(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkTaskName(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkInstanceModuleLibrary(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkTaskSynthesizeble(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkInstanceModuleNmaeCase(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkInstanceModuleNmaeSame(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkEmptyModule(map<int, map<string, string> > & table, ivl_scope_t & myMod);
extern void checkProcessPrefixSuffix(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkProcessNameLength(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkProcessNameCase(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void EdgeTriggerTask(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkForkStatementNotSynthesizable(map<int, map<string, string> > & table, ivl_scope_t scope);
extern void checkInstanceNamePrefixSuffix(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void checkFunctionReturningReal(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void InstanceNameRelatedModuleName(map<int, map<string, string> > & table, ivl_scope_t scope);

/**************************************** ivl_signal_t ********************************/
extern void checkTriStateOtherLogic(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkDirectInputOutput(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkBiDirecTionalPort(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignalDrivConst(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignalDrivers(map<int, map<string, string> > & table, ivl_signal_t & mySig);
void checkPin(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkTRISTATE(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkMULTIPLETRISTATE(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkTristateNonTopModule(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkTristateEnable(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkCOMBINATIONALLOOP(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkNOoutput(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkPortNotConnected(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkNetType(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkNonConstantBitRange(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkAssignmentInputSignal(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkInsufficiant(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkOver(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkLogic1(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkNetStuck(map<int, map<string, string> > & table, ivl_statement_t & myStmt);
extern void checkIntegerConcatenation(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkVhdlReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkVerilogReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSystemVerilogReservedWords(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignalNoLoad(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkCombinationalPiPoPath(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignalPrefixSuffix(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkSignedUnSigned(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkConditionalAssignment(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkWire(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkLogicalOperatorsInSinglebitOperations(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkTestClockPrimaryInput(map<int, map<string, string> > & table, ivl_lpm_t & sig);
extern void checkVariableName(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void UnconventionalVectorRangeDefinition(map<int, map<string, string> > & table, ivl_signal_t & sig);
extern void checkWireNotExplicitlyDeclared(map<int, map<string, string> > & table, ivl_signal_t & sig);
extern void checktriDeclarationDetected(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkIntegerTypeObjectDetected(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void checkInoutInputOnlyShift(map<int, map<string, string> > & table, ivl_signal_t & sig);

extern void checkDriveSynth(map<int, map<string, string> > & table, ivl_signal_t &mySig);


extern void ConditionSigAssignX(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void ReductionOperandBitWidth(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void UPDNameLowerCase(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void EndPointNotGeneratedSingleClockSource(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void BitWidthMismatchLogicalOperation(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void BusDirectionConsistPortBinding(map<int, map<string, string> > & table, ivl_signal_t & mySig);
extern void SignalConnectedBothInputOutputPorts(map<int, map<string, string> > & table, ivl_signal_t mySig);
extern void InputFloating(map<int, map<string, string> > & table, ivl_signal_t & mySig);

/**************************************** ivl_lpm_t ********************************/
extern void checkLPM(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockSignal(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockSignalOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkResetSignalOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkLogic(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkGatedClock(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkInvertedClock(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkBufferedClock(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkResetDrivCombiLogic(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkSetDrivCombiLogic(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkSyncAsyncReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockConst(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockSeqLogic(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkMultipleClock(map<int, map<string, string> > & table, ivl_event_t & evt);
extern void checkResetActiveHighLow(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkSetDataInput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkEnableSetReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkCombinationalPathTooLong(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkClockSignalSet(map<int, map<string, string> > & table,ivl_lpm_t & lpm);
extern void checkSpecialSignalBitSelect(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkTestClockSignalDataInput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkMultipleBitSpecialSignal(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkMultipleClockSource(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkActiveSignalName(map<int, map<string, string> > & table, ivl_event_t & net);
extern void checkActiveHighSignal(map<int, map<string, string> > & table,ivl_lpm_t & lpm);
extern void checkRegisterOutputPrefixSuffix(map<int, map<string, string> > & table,ivl_lpm_t & lpm);
extern void checkRegisterInputPrefixSuffix(map<int, map<string, string> > & table,ivl_lpm_t & lpm);
extern void checkClockReset(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkSyncAsyncSet(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkResetDataInput(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkResetSequential(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkNonConstShiftAmt(map<int, map<string, string>> &table, ivl_expr_t net, bool ft);
extern void checkClockSet(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkBitSelectSensitivity(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkRightHandSideVarSensitivity(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkNonConstDivisor(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkNonConstDivident(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkTestClock2_1Mux(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkClockControl(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkFallingActiveClock(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkResetPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkSetPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkUnsignedVector(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkAsynchronousLoop(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkSynchronousLoop(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkSetDrivenSequentialLogic(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkNoSetReset(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkRegPrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void checkLatchNamePrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkInferredStorageNotinLibrary(map<int, map<string, string> > & table,ivl_lpm_t & lpm);
extern void checkInferredLatch(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkInferredMux(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkAsynchronousSignalNamePrefixSuffix(map<int, map<string, string> > & table,ivl_lpm_t & lpm);
extern void checkRegularActiveHighActiveLowSignal(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkNextStateNamePrefixSuffix(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkMemoryisReadandWrittenatSameTime(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkResetSignalFeedsintoPrimaryOutput(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkReconvergedClock(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkSeparateClockGenerateCircuitinDifferentModules(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkPossibleLossofCarryorBorrow(map<int, map<string, string> > & table,ivl_lpm_t & net);
extern void checkGlitch(map<int, map<string, string> > & table, ivl_lpm_t & lpm);


extern void DualSetRestDetected(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void MultipleReset(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void BothSetResetFound(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void TristateBufferClockPath(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void TristateBusNotGeneratedSingleClockSource(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void SimpleExprAsynSetRst(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void AsynSetRstShouldMostBeginningStatementAlwaysBlock(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void AlwaysffNotUsedSequentialBlocks(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void Always_latchNotUsedSequentialBlocks(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void Use_always_combToModelCombinationalBehavior(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void Arithmetic_RelationalOperationsSharingLargeOperandNotAllowed(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void LargeMultiplierInferred(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void ReportSnakePath(map<int, map<string, string> > & table, ivl_lpm_t & lpm);

/**************************************** ivl_parameter_t ********************************/
void checkPMETER(map<int, map<string, string> > & table, ivl_parameter_t & param);
void checkPmeterWidth(map<int, map<string, string> > & table, ivl_parameter_t & param);
extern void checkPmeterNameLength(map<int, map<string, string> > & table, ivl_parameter_t & param);
extern void checkPmeterPrefixSuffix(map<int, map<string, string> > & table, ivl_parameter_t & param);
/**************************************** ivl_net_logic_t ********************************/
extern void checkcombinational(map<int, map<string, string> > & table, ivl_net_logic_t & gate);
extern void checkCombinationalinSequentialBlock(map<int, map<string, string> > & table, ivl_net_logic_t & gate);
extern void checkGatePrefixSuffix(map<int, map<string, string> > & table, ivl_net_logic_t & gate);
extern void checkReductionOpSingleBitSig(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void checkGATE(map<int, map<string, string> > & table,ivl_net_logic_t & gate);
void checkGATECONSTANT(map<int, map<string, string> > & table,ivl_net_logic_t & gate);
extern void checkInCmpOpPresidence(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void checkMultiBitExpSingleBitExpected(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void checkGateInstance(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void checkTristatePrefixSuffix(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void checkTristateBufferNotCoexistOtherLogic(map<int, map<string, string> > & table, ivl_net_logic_t & gate);
extern void checkInferredTristate(map<int, map<string, string> > & table, ivl_net_logic_t & log);
extern void checkTriStateDisabledShift(map<int, map<string, string> > & table, ivl_net_logic_t & log);
extern void checkSpecialTypePortConnectedtoanExpression(map<int, map<string, string> > & table, ivl_lpm_t &lpm);
extern void checkBitWidthMismatchinBitwiseOperation(map<int, map<string, string> > & table, ivl_net_logic_t & log);
extern void checkReconvClock(map<int, map<string, string> > & table, ivl_net_logic_t & log);
/**************************************** ivl_event_t ********************************/
extern bool checkEvent(map<int, map<string, string> > & table, ivl_event_t & evt, set<ivl_signal_t> &senLst);
extern void checkDuplicate(map<int, map<string, string> > & table, ivl_event_t & evt);
extern void checkEdgeNonEdge(map<int, map<string, string> > & table, ivl_event_t & evt);
extern void checkClockActiveBothEdges(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkNoFallingActiveClock(map<int, map<string, string> > & table, ivl_event_t & evt);
extern void MissingProcessLabelName(map<int, map<string, string> > & table, ivl_statement_t net);
extern void IBlock(map<int, map<string, string> > & table, ivl_process_t proc);
extern void checkTriEnb(map<int, map<string, string> > & table, ivl_lpm_t & lpm);
extern void checkTristateBuffers(map<int, map<string, string> > & table, ivl_lpm_t & net);


extern void KeywordChangedNotSynthesizable(map<int, map<string, string> > & table, ivl_event_t & evt);
extern void IffConstructNotSynthesizable(map<int, map<string, string> > & table, ivl_event_t & evt);

/**************************************** ivl_variable_t ********************************/
extern void variable_synthesizeble(map<int, map<string, string> > & table, ivl_signal_t & net);
extern void DelayControl(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> &senLst, set<ivl_signal_t> &sigSet);
extern void SystemTaskCall(map<int, map<string, string> > & table, ivl_statement_t net);
extern void ForceStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigList = NULL);
extern void ReleaseStatement(map<int, map<string, string> > & table, ivl_statement_t net, set<ivl_signal_t> *sigList = NULL);
extern void SignalAssigned(map<int, map<string, string> > & table, ivl_signal_t sig);
extern void WhileLoop(map<int, map<string, string>> &table, ivl_statement_t net);
extern void checkNestedEvents(map<int, map<string, string> > & table, ivl_statement_t net, bool time = false);
extern void checkRepeatExpression(map<int, map<string, string> > & table, ivl_statement_t net);
extern void checkUserTask(map<int, map<string, string> > & table, ivl_statement_t net);

/**************************************** ivl_memory_t ********************************/
extern void checkMemory(map<int, map<string, string> > & table, ivl_signal_t & mem);
extern void checkMemorySize(map<int, map<string, string> > & table, ivl_memory_t & mem);
extern void checkMemoryBits(map<int, map<string, string> > & table, ivl_memory_t & mem);


extern void UninitializedVariableReferencedinanEdgetriggeredBlock(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void UnAssignedVariableReferencedinanEdgetriggeredBlock(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void UninitializedVariableReferencedinaCombinationalProcess(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void VariableNotFullyAssignedbeforeReferencedinCombinationalProcess(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void checkTimeScaleMissing(map<int, map<string, string> > & table, ivl_scope_t & scope);
extern void ObjectsWithSameNameofObjectinOuterScope(map<int, map<string, string> > & table, ivl_signal_t & sig);
extern void Signal_with_Heavy_Fanout_Loading(map<int, map<string, string> > & table, ivl_signal_t & net);
extern void Module_Name_Different_from_File_Name(map<int, map<string, string> > & table, ivl_scope_t & net);
extern void Bit_Width_Mismatch_in_Assignment(map<int, map<string, string> > & table, ivl_lpm_t & net);
extern void Constant_Connected_to_Instance(map<int, map<string, string> > & table, ivl_net_logic_t & net);
extern void Non_constant_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Loop_Variable_Changed_in_For_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Arithmetic_Operations_in_For_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Logical_Operations_in_For_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Filp_flop_Reset_Logic_in_For_LoopL(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Infinite_Loop(map<int, map<string, string> > & table,ivl_statement_t net);
extern void Apparent_Loop(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Gate_Instance_Found(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Wait_Statement_Not_Synthesizable(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Case_Equivalence_Operator_Not_Synthesizable(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Event_Control_or_Delay_Statement_Not_Found_in_All_Branches(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);
extern void Unconventional_File_Extension(map<int, map<string, string> > & table, ivl_scope_t & net);
extern void For_Loop_Detected(map<int, map<string, string> > & table,ivl_statement_t net,ivl_process_t PR);


#endif
