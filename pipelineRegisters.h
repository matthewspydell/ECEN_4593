/* @author matthewspydell
 * This file creates all the various structures that will represent the
 * pipeline registers IF/ID, ID/EX, EX/MEM, and MEM/WB
 */

#ifndef pipelineRegisters_h
#define pipelineRegisters_h

/* instruction-fetch | instruction-decode register */
struct IFID {
  uint32_t instructionShadow;
  uint32_t instruction;
} IF_ID;
// create and initialize IF/ID pipeline register


/* instruction-decode | execute register */
struct IDEX {
  uint32_t opcodeShadow;
  uint32_t rsShadow;
  uint32_t rtShadow;
  uint32_t rdShadow;
  uint32_t shamtShadow;
  uint32_t functShadow;
  uint32_t immShadow;
  bool memReadShadow;
  uint32_t opcode;
  uint32_t rs;
  uint32_t rt;
  uint32_t rd;
  uint32_t shamt;
  uint32_t funct;
  uint32_t imm;
  bool memRead;
} ID_EX;
// create and initialize ID/EX pipeline register


/* execute | memory register */
struct EXMEM {
  uint32_t opcodeShadow;
  uint32_t rdShadow;
  uint32_t aluOutputShadow;
  bool memReadShadow;
  uint32_t opcode;
  uint32_t rd;
  uint32_t aluOutput;
  bool memRead;
} EX_MEM;
// create and initialize EX/MEM pipeline register


/* memory | write-back register */
struct MEMWB {
  uint32_t opcodeShadow;
  uint32_t rdShadow;
  uint32_t aluOutputShadow;
  bool memReadShadow;
  uint32_t opcode;
  uint32_t rd;
  uint32_t aluOutput;
  bool memRead;
} MEM_WB;
// create and initialize MEM_WB pipeline register

#endif
