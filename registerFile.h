/*
 * @author matthewspydell
 */

#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "main.h"

// Register File Mappings

// program counter register setup
uint32_t $pc;
uint32_t $sp;
uint32_t $fp;

// 32 registers, each 32 bits wide
uint32_t R[32];


#endif /* REGISTERFILE_H */

