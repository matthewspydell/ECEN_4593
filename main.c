#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Single.h"
#include "Load_Program.h"




// program counter gives address to instruction memory
// instruction memory gives instruction
// decode instruction

// read registers

// send in data to ALU, input multiplexor ( Y = S ? I1 : 10; )

// Access data memory for load and store

// update PC ( target address or PC+4), input multiplexor

// ichache always read
//bool main_memory(icache,read,address, &data)
//{
//    struct mem_busy;
//    if(mem_busy);
//    // every clock cycle processor checking cache , hit or miss
//
//}

int main()
{

    int i = 0;

    Initialize_Simulation_Memory();
    clear_pipe();

	  $pc = memory[5];
	  $sp = memory[0];
	  $fp = memory[1];

        R[29]=$sp;
        R[30] = $fp;


do {
		  Execute_Clock_Cycle();
		  cycle_count++;
		  i++; } while ($pc !=  0x00000000);


unsigned int sorted_value = memory[6];
// 112
printf("\n\n MEM[6] =  %u \n", sorted_value);

unsigned int num_passes_bubble = memory[7];
printf("\n\n MEM[7] =  %u \n", num_passes_bubble);

unsigned int passes_insert = memory[8];
printf("\n\n MEM[8] =  %u \n", passes_insert);

unsigned int identical = memory[9];
printf("\n\n MEM[9] =  %u \n", identical);

// Expecting: 600,00
printf("\n\nCYCLES: %d\n\n", cycle_count);


    return 0;
}
