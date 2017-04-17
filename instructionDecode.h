/* @author matthewspydell
 *
 */
 
#ifndef instructionDecode_h
#define instructionDecode_h

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

/* instruction pointer holds address of IF/ID pipeline
 * register that holds the next instruction
 */
uint32_t* instruction = &(IF_ID.instruction);

/* this function decodes the MIPS instruction and
 * fills in the ID/EX pipeline registers
 */
void instructionDecode(int* instruction);

#endif
