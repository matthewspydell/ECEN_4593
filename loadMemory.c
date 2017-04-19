/*
 * @author matthewspydell
 */

 #include "loadMemory.h"

 void loadMemory(uint32_t* memory, uint32_t* program) {
   for (int i=0; i<sizeof(program); i++) {
     memory[i] = program[i]; // copy program into memory
   }
 }
