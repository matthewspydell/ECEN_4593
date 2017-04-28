/*
 * @author matthewspydell
 */

#include "instructionFetch.h"

void instructionFetch(uint32_t $pc) {
  IF_ID.instructionShadow = mainMemory[$pc];
  
  printf("\nInstruction Fetch Stage\nInstruction = 0x%x\n", mainMemory[$pc]);
}


