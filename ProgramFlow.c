1. PC --> fetch instruction from instruction memory , decode instruction

2. Read registers from register file

3. ALU execute
        -> arithmetic result
        -> memory address for lw/sw
        -> branch target address

4. Access data memory for lw/sw

5. update PC
        --> branch address
        --> increment by 4


#include <stdio.h>

// define #____         switch from 1 to 3 stage pipeline

// Global Variables , Static variables (use $gp to access)

/*  MIPs ISA:    32-bit data = word     instructions = 32 bits      addresses = 32 bits
*
*   $pc    :    program counter, register holding current instruction being executed,
*               where in instruction memory we are
*
*   Memory :    byte addressed , each address identifies as as 1 byte
*               words aligned , address a multiple of 4
*               big endian (most sig byte at least address of word )
*               incrementing memory[] array --> increment by 4 --> shift by 2
*               Index 8 requires offset of 32
*
*   R-format:   op(6) | rs(5) | rt(5) | rd(5) | shamt(5) | funct(6)
*
*   I-format:   op(6) | rs(5) | rt(5) | constant/address(16)    (address --> offset added to base register rs)
*                                                               rt = rd + constant
*                                                               branching
*   J-format:   op(6) | address (26)                            4 most sig bits copied from PC, rest of address from "address" field, last 2 bits 0
*
*   Registers:
*               $a0-$a3     : arguments     : 4-7
*               $v0-$v1     : result values : 2-3
*               $t0-$t7     : temporaries   : 8-15
*               $t8-$t9     : temporaries   : 24-25
*               $s0-$s7     : saved         : 16-23
*               $gp         : global pointer for global variables : 28
*               $sp         : stack pointer : 29
*               $fp         : frame pointer : 30
*               $ra         : return address: 31
*/

/* Clock Simulation
 * State updated at every rising edge
 * Data transfer & computation between clock cycles
 */

 /* Unified Memory
 */

 /* Separate Data & Instruction Memory
 */

 /* Decode Opcode
 *  To determine datapath
 */
 if( ($pc >> 10) == 0 )
 {
     R = true;
 }
 else if ( ($pc >> 10) == )
 {
     I = true;
 }
 else if ( ($pc >> 10) == )
 {
     J = true;
 }


 /* Cache Memory
 *  Immediate access to data, between registers & main memory
 */


 /* Arithmetic Operations
 *  Register Operands
 *  Load values from memory into registers
 *  Store result from register to memory
 */

 /* Sign Extension
 *  addi, lb/lh (extend loaded byte/halfword) , beq/bne (extend displacement)
 *  unsigned values sign extended with 0s
 *  -2 = 1111 1111 1111 1110
 */

// Structures
PC
Instruction Memory
Register File
ALU
Data Memory
Adder for PC

if statements for multiplexors ?
