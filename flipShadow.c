/*
 * @author matthewspydell
 */

#include "flipShadow.h"

void flipShadow() {
  // flip IF/ID pipeline register if no stall
  if (!stallPipe) {
    IF_ID.instruction = IF_ID.instructionShadow;
  }

  // flip ID/EX pipeline register if no stall
  if (!stallPipe) {
    ID_EX.opcode = ID_EX.opcodeShadow;
    ID_EX.rs = ID_EX.rsShadow;
    ID_EX.rt = ID_EX.rtShadow;
    ID_EX.rd = ID_EX.rdShadow;
    ID_EX.shamt = ID_EX.shamtShadow;
    ID_EX.funct = ID_EX.functShadow;
    ID_EX.imm = ID_EX.immShadow;
    ID_EX.memRead = ID_EX.memReadShadow;
  } else { // if stalled insert noop
    ID_EX.opcode = 0;
    ID_EX.rs = 0;
    ID_EX.rsValue = 0;
    ID_EX.rt = 0;
    ID_EX.rtValue = 0;
    ID_EX.rd = 0;
    ID_EX.rdValue = 0;
    ID_EX.shamt = 0;
    ID_EX.funct = 0;
    ID_EX.imm = 0;
    ID_EX.memRead = false;
  }

  // flip EX/MEM pipeline register
  EX_MEM.rd = EX_MEM.rdShadow;
  EX_MEM.rdValue = EX_MEM.rdValueShadow;
  EX_MEM.aluOutput = EX_MEM.aluOutputShadow;
  EX_MEM.opcode = EX_MEM.opcodeShadow;
  EX_MEM.memRead = EX_MEM.memReadShadow;
  EX_MEM.offset = EX_MEM.offsetShadow;
  EX_MEM.moveControl = EX_MEM.moveControlShadow;

  // flip MEM/WB pipeline register
  MEM_WB.rd = MEM_WB.rdShadow;
  MEM_WB.rdValue = MEM_WB.rdValueShadow;
  MEM_WB.aluOutput = MEM_WB.aluOutputShadow;
  MEM_WB.opcode = MEM_WB.opcodeShadow;
  MEM_WB.memRead = MEM_WB.memReadShadow;
  MEM_WB.moveControl = MEM_WB.moveControlShadow;

  if (!pcBranch && !stallPipe) $pc++;
  pcBranch = false;
  stallPipe = false;
}
