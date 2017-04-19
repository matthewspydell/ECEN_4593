/*
 * @author matthewspydell
 * This file executes a clock cycle which moves data through the pipeline registers
 * and executes each stage of the pipeline
 */

#include "executeClock.h"

 void executeClock() {
   instructionFetch($pc);  // fetch instruction

   writeBack();  // write back to register file depending on instruction

   instructionDecode();  // decode instruction

   instructionExecute(); // execute instruction

   memoryAccess(); // access memory depending on instruction

   flipShadow(); // move data from shadow pipeline register to actual pipeline register
 }
