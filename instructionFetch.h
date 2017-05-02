/*
 * @author matthewspydell
 */

#ifndef INSTRUCTIONFETCH_H
#define INSTRUCTIONFETCH_H

#include "main.h"

/* this function fetches the instruction from memory that pc is pointing to
 * and loads the it into the IF/ID pipeline register
 */
void instructionFetch(uint32_t pc);


#endif /* INSTRUCTIONFETCH_H */

