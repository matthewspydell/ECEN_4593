/* @author matthewspydell
 * This file creates all the various structures that will represent the
 * pipeline registers IF/ID, ID/EX, EX/MEM, and MEM/WB
 */

#ifndef PIPELINEREGISTERS_H
#define PIPELINEREGISTERS_H

#include "main.h"

/* instruction-fetch | instruction-decode register */
struct IFID {
  int instructionShadow;
  int instruction;
} IF_ID;
// create and initialize IF/ID pipeline register


/* instruction-decode | execute register */
struct IDEX {
  int opcodeShadow;
  int rsShadow;
  int rsValueShadow;
  int rtShadow;
  int rtValueShadow;
  int rdShadow;
  int rdValueShadow;
  int shamtShadow;
  int functShadow;
  int immShadow;
  bool memReadShadow;
  int opcode;
  int rs;
  int rsValue;
  int rt;
  int rtValue;
  int rd;
  int rdValue;
  int shamt;
  int funct;
  int imm;
  bool memRead;
} ID_EX;
// create and initialize ID/EX pipeline register


/* execute | memory register */
struct EXMEM {
  int opcodeShadow;
  int rdShadow;
  int rdValueShadow;
  int aluOutputShadow;
  bool memReadShadow;
  int offsetShadow;
  int opcode;
  int rd;
  int rdValue;
  int aluOutput;
  bool memRead;
  int offset;
} EX_MEM;
// create and initialize EX/MEM pipeline register


/* memory | write-back register */
struct MEMWB {
  int opcodeShadow;
  int rdShadow;
  int rdValueShadow;
  int aluOutputShadow;
  bool memReadShadow;
  int opcode;
  int rd;
  int rdValue;
  int aluOutput;
  bool memRead;
} MEM_WB;
// create and initialize MEM_WB pipeline register


#endif /* PIPELINEREGISTERS_H */

