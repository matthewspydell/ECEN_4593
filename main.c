#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "registerFile.h"
#include "pipelineRegisters.h"
#include "instructionDecode.h"

// Memory
uint32_t memory[];          // words differ by 4 bytes

// $pc points to first memory address
uint32_t* $pc = memory;


// Masks for decode
uint32_t byte_mask = 0x000000FF;
uint32_t half_mask = 0x0000FFFF;

uint32_t opcode_mask = 0xFC000000;      // for all formats

uint32_t rs_mask =     0x03E00000;      // for R & I formats
uint32_t rt_mask =     0x001F0000;

uint32_t rd_mask =     0x0000F800;      // for R formats
uint32_t shamt_mask =  0x000007C0;
uint32_t func_mask  =  0x0000003F;

uint32_t imm_mask_j =  0x03FFFFFF;      // for J formats
uint32_t imm_mask_i =  0x0000FFFF;      // for I formats

struct instruction {

   uint32_t opcode;
   uint32_t rs;
   uint32_t rt;
   uint32_t rd;
   uint32_t shamt;
   uint32_t func;
   uint32_t iImm;
   uint32_t jImm;
   bool Rform;
   bool Iform;
   bool Jform;

} currentInst;


void decode( )
{
    currentInst.opcode =  ( (*$pc & opcode_mask) >> 26 );
    currentInst.rs = ( (*$pc & rs_mask) >> 21);
    currentInst.rt = ( (*$pc & rt_mask) >> 16);
    currentInst.rd = ( (*$pc & rd_mask) >> 11);
    currentInst.shamt = ( (*$pc & shamt_mask) >> 6);
    currentInst.func = ( (*$pc & func_mask));
    currentInst.iImm = ( (*$pc & imm_mask_i));
    currentInst.jImm = ( (*$pc & imm_mask_j));
    currentInst.Rform = false;
    currentInst.Iform = false;
    currentInst.Jform = false;

    switch(currentInst.opcode) {

// arithmetic R
    case 0x00  :
        currentInst.Rform = true;
        currentInst.rs = ( (*$pc & rs_mask) >> 21);
        currentInst.rt = ( (*$pc & rt_mask) >> 16);
        currentInst.rd = ( (*$pc & rd_mask) >> 11);
        currentInst.shamt = ( (*$pc & shamt_mask) >> 6);
        currentInst.func = ( (*$pc & func_mask));

        break;

// addi I
    case 0x08  :
        currentInst.Iform = true;
        currentInst.rs = ( (*$pc & rs_mask) >> 21);
        currentInst.rt = ( (*$pc & rt_mask) >> 16);
        currentInst.iImm = ( (*$pc & imm_mask_i));

        break;




}


}


int main()
{
    memory[0] = 0x00af8020;
    decode();

    return 0;
}
