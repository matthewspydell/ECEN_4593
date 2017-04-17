#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "registerFile.h"
#include "pipelineRegisters.h"
#include "executeClock.h"
#include "loadMemory.h"

// setup memory
uint32_t mainMemory[1024];  // word aligned memory

loadMemory(mainMemory, program1Memory); // load program 1
//loadMemory(mainMemory, program2Memory); // load program 2

// $pc points to first memory address
$pc = mainMemory[5];
$sp = mainMemory[0];
$fp = mainMemory[1];

// setup variables to keep track while program runs
int clockCycles = 0;
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
