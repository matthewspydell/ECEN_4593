/*
 * @author Matthew Spydell
 * MIPS Instruction Decoder
 */

#include "instructionDecode.h"

/* these are the various masks used to pull out each value that needs
 * to be loaded into the pipeline registers
 */
// R-format instruction masks
uint32_t opcodeMask = 0xFC000000;
uint32_t rsMask = 0x03E00000;
uint32_t rtMask = 0x001F0000;
uint32_t rdMask = 0x0000F800;
uint32_t shamtMask = 0x000007C0;
uint32_t functMask = 0x0000003F;

// I-format additional instruction mask
uint32_t immMask = 0x0000FFFF;

// J-format additional instruction mask
uint32_t addMask = 0x03FFFFFF;

void instructionDecode() {

  ID_EX.opcodeShadow = (IF_ID.instruction & opcodeMask)>>26;

  // instruction format is determined by opcode
  if (ID_EX.opcodeShadow == 0) { // R-format instruction
    ID_EX.rsShadow = (IF_ID.instruction & rsMask)>>21;
    ID_EX.rtShadow = (IF_ID.instruction & rtMask)>>16;
    ID_EX.rdShadow = (IF_ID.instruction & rdMask)>>11;
    ID_EX.shamtShadow = (IF_ID.instruction & shamtMask)>>6;
    ID_EX.functShadow = (IF_ID.instruction & functMask);
  } else if (ID_EX.opcodeShadow == 0x2 || ID_EX.opcodeShadow == 0x3) { // J-format instruction
    $pc = (IF_ID.instruction & addMask);    // jump to new address
    IF_ID.instructionShadow = mainMemory[$pc];  // fetch new instruction and put in IF_ID pipeline
  } else { // I-format instruction
    ID_EX.rsShadow = (IF_ID.instruction & rsMask)>>21;
    ID_EX.rdShadow = (IF_ID.instruction & rtMask)>>16; // rd == rt for I-format instructions
    ID_EX.immShadow = (IF_ID.instruction & immMask);
    // determine if instruction is a memory read
    if (ID_EX.opcodeShadow == 0x24 || ID_EX.opcodeShadow == 0x25 || ID_EX.opcodeShadow == 0x23) {
      ID_EX.memReadShadow = true;
    } else {ID_EX.memReadShadow = false;}

    ///////////// branch detection ////////////////
    // branch on equal, if(R[rs] == R[rt]) $pc = $pc + 4 + branchAddress
    if (ID_EX.opcodeShadow == 0x04) {
      // hazard protection and forwarding for branch
      if (ID_EX.rd == ID_EX.rsShadow || ID_EX.rd == ID_EX.rdShadow) {
        stallPipe = true;
        $pc --; // load in previous instruction again
      } else if ((EX_MEM.rd == ID_EX.rsShadow || EX_MEM.rd == ID_EX.rdShadow) && EX_MEM.memRead) {
        stallPipe = true;
        $pc --; // load in previous instruction again
      } else if (EX_MEM.rd == ID_EX.rsShadow) {
        R[ID_EX.rsShadow] = EX_MEM.aluOutput;
      } else if (MEM_WB.rd == ID_EX.rsShadow) {
        R[ID_EX.rsShadow] = MEM_WB.aluOutput;
      } else if (EX_MEM.rd == ID_EX.rdShadow) {
        R[ID_EX.rdShadow] = EX_MEM.aluOutput;
      } else if (MEM_WB.rd == ID_EX.rdShadow) {
        R[ID_EX.rdShadow] = MEM_WB.aluOutput;
      }
      // check whether to take branch
      if ((R[ID_EX.rsShadow] == R[ID_EX.rdShadow]) && !stallPipe) {
        $pc = (uint32_t) ((int)($pc) + (int)ID_EX.immShadow);
        pcBranch = true;  // don't increment $pc after the jump
      }
    }
    // branch on not equal, if (R[rs] != R[rt]) $pc = $pc + 4 + branchAddress
    if (ID_EX.opcodeShadow == 0x05) {
      // hazard protection and forwarding for branch
      if (ID_EX.rd == ID_EX.rsShadow || ID_EX.rd == ID_EX.rdShadow) {
        stallPipe = true;
        $pc --; // load in previous instruction again
      } else if ((EX_MEM.rd == ID_EX.rsShadow || EX_MEM.rd == ID_EX.rdShadow) && EX_MEM.memRead) {
        stallPipe = true;
        $pc --; // load in previous instruction again
      } else if (EX_MEM.rd == ID_EX.rsShadow) {
        R[ID_EX.rsShadow] = EX_MEM.aluOutput;
      } else if (MEM_WB.rd == ID_EX.rsShadow) {
        R[ID_EX.rsShadow] = MEM_WB.aluOutput;
      } else if (EX_MEM.rd == ID_EX.rdShadow) {
        R[ID_EX.rdShadow] = EX_MEM.aluOutput;
      } else if (MEM_WB.rd == ID_EX.rdShadow) {
        R[ID_EX.rdShadow] = MEM_WB.aluOutput;
      }
      // check whether to take branch
      if ((R[ID_EX.rsShadow] != R[ID_EX.rdShadow]) && !stallPipe) {
        $pc = (uint32_t) ((int)($pc) + (int)ID_EX.immShadow);
        pcBranch = true;  // don't increment $pc after the jump
      }
    }
  }
}
