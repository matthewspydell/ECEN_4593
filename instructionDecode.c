/* @author Matthew Spydell
 * MIPS Instruction Decoder
 */

#include "instructionDecode.h"

void instructionDecode(int* instruction) {

  ID_EX.opcodeShadow = (*instruction & opcodeMask)>>26;

  // instruction format is determined by opcode
  if (ID_EX.opcodeShadow == 0) { // R-format instruction
    ID_EX.rsShadow = (*instruction & rsMask)>>21;
    ID_EX.rtShadow = (*instruction & rtMask)>>16;
    ID_EX.rdShadow = (*instruction & rdMask)>>11;
    ID_EX.shamtShadow = (*instruction & shamtMask)>>6;
    ID_EX.functShadow = (*instruction & functMask);
  } else if (ID_EX.opcodeShadow == 0x2 || ID_EX.opcodeShadow == 0x3) { // J-format instruction
    ID_EX.addressShadow = (*instruction & addMask);
  } else { // I-format instruction
    ID_EX.rsShadow = (*instruction & rsMask)>>21;
    ID_EX.rdShadow = (*instruction & rtMask)>>16; //rd is rt for I-format instructions
    ID_EX.immShadow = (*instruction & immMask);
  }

}
