/*
 * @author matthewspydell
 */

 #include "loadMemory.h"

 void loadMemory(int* memory, int* program, size_t programLength) {
   for (int i=0; i<programLength; i++) {
     memory[i] = program[i]; // copy program into memory
   }
 }
