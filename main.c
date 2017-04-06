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
    int cycle_count;

    Initialize_Simulation_Memory();

    IF();

    //while (1) {

//	  /* initialize state elements */
//
//	  initialize_main_memory();
//	  initialize_icache();
//	  initialize_dcache();
//	  initialize_pipeline();
//	  initialize_analytics();
//
//	  $pc = memory[5];
//	  PCreg.PC = memory[5];
//	  reg[$sp] = memory[0];;
//	  reg[$fp] = memory[1];
	 // cycle_count = 0;
	//  while ($pc != 0x00000000){
		//  Execute_Clock_Cycle();
		//  cycle_count++;
		//  i++;

	//	  if (Total_Instructions > 0) icache_hit_rate = (Total_Instructions - icache_misses) / Total_Instructions;
	//	  if (dcache_hits > 0) dcache_hit_rate = (dcache_hits - dcache_misses) / dcache_hits;
	//	  if (Total_Instructions > 0) CPI = (float)cycle_count / (float) Total_Instructions;
	 // }
  //}

    return 0;
}
