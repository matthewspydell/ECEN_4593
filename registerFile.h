/*
 * @author matthewspydell
 */

#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "main.h"

// Register File Mappings

// program counter register setup
int $pc;

// 32 registers, each 32 bits wide
int R[32];

#define $sp R[29]
#define $fp R[30]

#endif /* REGISTERFILE_H */

