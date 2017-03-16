/* @author Matthew Spydell
 * MIPS Instruction Decoder
 */

#include "instructionDecode.h"

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
    ID_EX.addressShadow = (IF_ID.instruction & addMask);
  } else { // I-format instruction
    ID_EX.rsShadow = (IF_ID.instruction & rsMask)>>21;
    ID_EX.rdShadow = (IF_ID.instruction & rtMask)>>16; //rd is rt for I-format instructions
    ID_EX.immShadow = (IF_ID.instruction & immMask);
  }
}
