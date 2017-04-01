#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Single.h"



// CLOCK
unsigned int cycles = 0;
bool rising_edge = false;

// update state (register file updated if RegWrite=1 on clock edge)
void clock_edge()
{
 rising_edge=true;
}

// program counter gives address to instruction memory
// instruction memory gives instruction
// decode instruction

// read registers

// send in data to ALU, input multiplexor ( Y = S ? I1 : 10; )

// Access data memory for load and store

// update PC ( target address or PC+4), input multiplexor


// update register file
void next_cycle()
{
    $pc+4;
}

int main()
{
   // int16_t test = 0x8000;

   // printf("%x",test);

    memory[0] = 0x00af8020;

    IF();

    uint32_t alu_result;
    alu_result = ALU(&currentInst);

    printf("\n%d\n",alu_result);


//    R[currentInst.rt] = 0x6789ABCD;
//    R[currentInst.rs] = 0;
//
//     R[currentInst.rd] = (R[currentInst.rs] < R[currentInst.rt]) ? 1 : 0 ;
//
//     printf("%d", R[currentInst.rd]);

//sign_ext(test);



    return 0;
}
