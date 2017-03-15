/* @author matthewspydell
 *
 */

void flipShadow() {
  // flip IF/ID pipeline register
  IF_ID.instruction = IF_ID.instructionShadow;

  // flip ID/EX pipeline register
  ID_EX.opcode = ID_EX.opcodeShadow;
  ID_EX.rs = ID_EX.rsShadow;
  ID_EX.rt = ID_EX.rtShadow;
  ID_EX.rd = ID_EX.rdShadow;
  ID_EX.shamt = ID_EX.shamtShadow;
  ID_EX.funct = ID_EX.functShadow;
  ID_EX.imm = ID_EX.immShadow;

  // flip EX/MEM pipeline register
  EX_MEM.rd = EX_MEM.rdShadow;
  EX_MEM.aluOutput = EX_MEM.aluOutputShadow;
  EX_MEM.address = EX_MEM.addressShadow;

  // flip MEM/WB pipeline register
  MEM_WB.rd = MEM_WB.rdShadow;
  MEM_WB.aluOutput = MEM_WB.aluOutputShadow;
  MEM_WB.addressOut = MEM_WB.addressOutputShadow;
}
