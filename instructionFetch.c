/*
 * @author matthewspydell
 */

#include "instructionFetch.h"
#include "memAccess.h"

void instructionFetch(uint32_t $pc) {
    IF_ID.instructionShadow = mainMemory[$pc];
    //memAccess(true, true, $pc);
    if (printing) {
        printf("\nInstruction Fetch Stage\nInstruction = 0x%x\n", mainMemory[$pc]);
    }
}


