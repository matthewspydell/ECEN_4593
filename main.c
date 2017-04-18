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



int main()
{
    int cycle_count=0;
    int i = 0;

    Initialize_Simulation_Memory();
    clear_shadow();

	  $pc = memory[5];
	  printf("pc = %d\n",$pc);
	  $sp = memory[0];
	  printf("sp = %d\n", $sp);
	  $fp = memory[1];
	  printf("fp = %d\n", $fp);

        R[29]=$sp;
        R[30] = $fp;


do {
		  Execute_Clock_Cycle();
		  cycle_count++;
		  i++; } while ($pc !=  0x00000000);


unsigned int sorted_value = memory[6];
printf("\n\n MEM[6] =  %u \n", sorted_value);

unsigned int num_passes_bubble = memory[7];
printf("\n\n MEM[7] =  %u \n", num_passes_bubble);

unsigned int passes_insert = memory[8];
printf("\n\n MEM[8] =  %u \n", passes_insert);

unsigned int identical = memory[9];
printf("\n\n MEM[9] =  %u \n", identical);


    return 0;
}
