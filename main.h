/*
 * @author matthewspydell
 */

#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "registerFile.h"
#include "pipelineRegisters.h"

// setup main memory
int mainMemory[1200]; // word aligned memory

// setup variables to keep track while program runs
int clockCycles;
bool stallPipe;
bool pcBranch;
bool printing;

#endif /* MAIN_H */

