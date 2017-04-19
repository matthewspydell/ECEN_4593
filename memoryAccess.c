/*
 * @author matthewspydell
 */

#include "memoryAccess.h"

 void memoryAccess() {
   MEM_WB.opcodeShadow = EX_MEM.opcode; // move opcode to next pipeline register
   MEM_WB.memReadShadow = EX_MEM.memRead; // move memRead indicator to next pipeline register

   switch (EX_MEM.opcode) {
     case 0x24:  // alu output from previous pipeline holds address location
      // load byte (unsigned)
      MEM_WB.rdShadow = (0xFF & mainMemory[EX_MEM.aluOutput]);
      break;
     case 0x25:  // alu output from previous pipeline holds address location
      // load halfword (unsigned)
      MEM_WB.rdShadow = (0xFFFF & mainMemory[EX_MEM.aluOutput]);
      break;
     case 0x23:  // alu output from previous pipeline holds address location
      // load word
      MEM_WB.rdShadow = mainMemory[EX_MEM.aluOutput];
      break;
     case 0x28:  // store byte, this is the end of this type of instruction
      // store byte from register rd in memory
      mainMemory[EX_MEM.aluOutput] = (0xFF & R[EX_MEM.rd]);
      break;
     case 0x29:  // store halfword, this is the end of this type of instruction
      // store halfword from register rd in memory
      mainMemory[EX_MEM.aluOutput] = (0xFFFF & R[EX_MEM.rd]);
      break;
     case 0x2B:  // store word, this is the end of this type of instruction
      // store word from register rd in memory
      mainMemory[EX_MEM.aluOutput] = R[EX_MEM.rd];
      break;
     default:
      // if no memory access move rd, aluOutput to next pipeline register
      MEM_WB.rdShadow = EX_MEM.rd;
      MEM_WB.aluOutputShadow = EX_MEM.aluOutput;
   }
 }
