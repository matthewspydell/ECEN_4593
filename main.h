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

// setup memory and cache
int mainMemory[1200]; // word aligned memory

struct cache_element {
    bool v;
    bool d;
    uint32_t tag;
};


// initialize cache
#define CACHESIZE 2
#define BLOCKSIZE 1
bool icache;
bool read;
struct cache_element icash[CACHESIZE][BLOCKSIZE];
struct cache_element dcash[CACHESIZE][BLOCKSIZE];


// setup variables to keep track while program runs
int clockCycles;
bool stallPipe;
bool pcBranch;
bool printing;

#endif /* MAIN_H */

