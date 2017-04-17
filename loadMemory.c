/*
 * @author matthewspydell
 */

 #include "loadMemory.h"

 void loadMemory(*memory, *program) {
   for (int i=0; i<size(program); i++) {
     (memory+i) = *(program+i); // copy program into memory
   }
 }
