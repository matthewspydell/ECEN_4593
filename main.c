#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registerFile.h"
#include "pipelineRegisters.h"
#include "instructionFetch.h"
#include "instructionDecode.h"
#include "instructionExecute.h"
#include "memoryAccess.h"
#include "writeBack.h"

// setup memory
uint32_t memory[];  // words differ by 4 bytes

// $pc points to first memory address
$pc = memory[];

/* in between each instruction move data from shadow pipeline registers
 * to actual pipeline registers to check basic functionality
 */
int main()
{
    memory[0] = 0x00af8020;

    instructionFetch($pc);  // fetch instruction

    flipShadow();

    instructionDecode();  // decode instruction

    flipShadow();

    instructionExecute(); // execute instruction

    flipShadow();

    memoryAccess(); // access memory depending on instruction

    flipShadow();

    writeBack();  // write back to register file depending on instruction

    return 0;
}
