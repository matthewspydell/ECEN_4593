/* @author Matthew Spydell
 * MIPS Instruction Decoder
 */

void instructionDecode(int* instruction) {
  /* R-format instruction masks */
  int opcodeMask = 0xFC000000;
  int rsMask = 0x03E00000;
  int rtMask = 0x001F0000;
  int rdMask = 0x0000F800;
  int shamtMask = 0x000007C0;
  int functMask = 0x0000003F;
  
  /* I-format additional instruction mask */
  int immMask = 0x0000FFFF;
  
  /* J-format additional instruction mask */
  int addMask = 0x03FFFFFF;
  
  opcodeRegister = (*instruction & opcodeMask)>>26;
  
  /* instruction format is determined by opcode */
  if (opcodeRegister == 0) {
    rsRegister = (*instruction & rsMask)>>21;
    rtRegister = (*instruction & rtMask)>>16;
    rdRegister = (*instruction & rdMask)>>11;
    shamt = (*instruction & shamtMask)>>6;
    funct = (*instruction & functMask);
  } else if (opcodeRegister == 0x2 || opcodeRegister == 0x3) {
    address = (*instruction & addMask);  
  } else {
    rsRegister = (*instruction & rsMask)>>21;
    rdRegister = (*instruction & rtMask)>>16; //rd is rt for I-format instructions
    imm = (*instruction & immMask);
  }
  
}
