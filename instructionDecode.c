/* @author Matthew Spydell
 * MIPS Instruction Decoder
 */

void instructionDecode(int* instruction) {
  /* R-format instruction masks */
  uint32_t opcodeMask = 0xFC000000;
  uint32_t rsMask = 0x03E00000;
  uint32_t rtMask = 0x001F0000;
  uint32_t rdMask = 0x0000F800;
  uint32_t shamtMask = 0x000007C0;
  uint32_t functMask = 0x0000003F;

  /* I-format additional instruction mask */
  uint32_t immMask = 0x0000FFFF;

  /* J-format additional instruction mask */
  uint32_t addMask = 0x03FFFFFF;

  ID_EX.opcodeShadow = (*instruction & opcodeMask)>>26;

  /* instruction format is determined by opcode */
  if (ID_EX.opcodeShadow == 0) {
    ID_EX.rsShadow = (*instruction & rsMask)>>21;
    ID_EX.rtShadow = (*instruction & rtMask)>>16;
    ID_EX.rdShadow = (*instruction & rdMask)>>11;
    ID_EX.shamtShadow = (*instruction & shamtMask)>>6;
    ID_EX.functShadow = (*instruction & functMask);
  } else if (ID_EX.opcodeShadow == 0x2 || ID_EX.opcodeShadow == 0x3) {
    ID_EX.addressShadow = (*instruction & addMask);
  } else {
    ID_EX.rsShadow = (*instruction & rsMask)>>21;
    ID_EX.rdShadow = (*instruction & rtMask)>>16; //rd is rt for I-format instructions
    ID_EX.immShadow = (*instruction & immMask);
  }

}
