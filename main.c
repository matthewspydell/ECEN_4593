#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

#include "registerFile.h"
#include "pipelineRegisters.h"
#include "executeClock.h"

// setup memory
uint32_t mainMemory[1024];  // words aligned memory

// $pc points to first memory address
$pc = mainMemory[5];
$sp = mainMemory[0];
$fp = mainMemory[1];

// setup variables to keep track while program runs
clockCycles = 0;
bool stallPipe = false;

int main()
{
  // program stops when $pc reaches address zero
  while ($pc != 0) {
    executeClock();
    clockCycles++;
  }

  return 0;
}
