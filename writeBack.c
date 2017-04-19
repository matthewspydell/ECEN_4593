/*
 * @author matthewspydell
 */

#include "writeBack.h"

 void writeBack() {

   switch (MEM_WB.opcode) {
    case 0x00:  // R-format
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x08:  // add immediate, R[rt] = R[rs] + imm (signed)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x09:  // add immediate, R[rt] = R[rs] + imm (unsigned)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x0C:  // and immediate, R[rt] = R[rs] & imm (zero extended)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x24:  // load byte, R[rt] = MEM8(R[rs] + imm) (unsigned)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x25:  // load halfword, R[rt] = MEM16(R[rs] + imm) (unsigned)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x23:  // load word, R[rt] = MEM16(R[rs] + imm)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x0D:  // or immediate, R[rt] = R[rs] | imm (zero extended)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x0A:  // set less than, R[rt] = (R[rs] < imm) ? 1:0 (signed)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x0B:  // set less than, R[rt] = (R[rs] < imm) ? 1:0 (unsigned)
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x28:  // store byte, MEM8(R[rs] + imm) = R[rt]
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x29:  // store halfword, MEM16(R[rs] + imm) = R[rt]
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
    case 0x2B:  // store word, MEM32(R[rs] + imm) = R[rt]
      R[MEM_WB.rd] = MEM_WB.aluOutput;
      break;
   }
 }
