/*
 * @author Matthew Spydell
 * MIPS Instruction Decoder
 */

#include "instructionDecode.h"

/* these are the various masks used to pull out each value that needs
 * to be loaded into the pipeline registers
 */
// R-format instruction masks
uint32_t opcodeMask = 0xFC000000;
uint32_t rsMask = 0x03E00000;
uint32_t rtMask = 0x001F0000;
uint32_t rdMask = 0x0000F800;
uint32_t shamtMask = 0x000007C0;
uint32_t functMask = 0x0000003F;

// I-format additional instruction mask
uint32_t immMask = 0x0000FFFF;

// J-format additional instruction mask
uint32_t addMask = 0x03FFFFFF;

void instructionDecode() {

  //printf("\nDecode Stage\n");
    
  ID_EX.opcodeShadow = (IF_ID.instruction & opcodeMask)>>26;
  //printf("opcodeShadow = %d\n", ID_EX.opcodeShadow);

  // instruction format is determined by opcode
  if (ID_EX.opcodeShadow == 0) { // R-format instruction
    //printf("R-Format Instruction\n");  
      
    ID_EX.rsShadow = (IF_ID.instruction & rsMask)>>21;
    ID_EX.rtShadow = (IF_ID.instruction & rtMask)>>16;
    ID_EX.rdShadow = (IF_ID.instruction & rdMask)>>11;
    ID_EX.shamtShadow = (IF_ID.instruction & shamtMask)>>6;
    ID_EX.functShadow = (IF_ID.instruction & functMask);
    ID_EX.memReadShadow = false;
    
    /////////// hazard protection ////////////
    if (EX_MEM.memRead == true && (ID_EX.rsShadow == ID_EX.rd || ID_EX.rtShadow == ID_EX.rd)) {
        stallPipe = true;
        //printf("Stalling\n");
    }
    
    if (ID_EX.functShadow == 0x08) {    // jump register
        printf("Jump Register Instruction\n");
        $pc = R[ID_EX.rsShadow] >> 2; // change from byte aligned to word aligned
        printf("New $pc = %d\n", $pc);
        pcBranch = true;  // don't increment $pc after the jump
        // once jump occurs a noop should be inserted in its place to make sure nothing executes
        ID_EX.rsShadow = 0;
        ID_EX.rtShadow = 0;
        ID_EX.rdShadow = 0;
        ID_EX.shamtShadow = 0;
        ID_EX.functShadow = 0;
        ID_EX.memReadShadow = false;
        
        printf("mainMemory[6] = %d\n", mainMemory[6]);
        printf("mainMemory[7] = %d\n", mainMemory[7]);
        printf("mainMemory[8] = %d\n", mainMemory[8]);
        printf("mainMemory[9] = %d\n", mainMemory[9]);
        printf("Clock Cycles = %d\n\n", clockCycles);
        /*
        for(int i=0; i<1200; i++) {
            printf("mainMemory[%d] = 0x%x = %u\n", i, mainMemory[i], mainMemory[i]);
        }
        */
    }
    
  } else if (ID_EX.opcodeShadow == 0x2 || ID_EX.opcodeShadow == 0x3) { // J-format instruction
    printf("J-Format Instruction\n");
    printf("$pc = %d\n", $pc);
    
    
    switch (ID_EX.opcodeShadow) {
    case 0x02:  // PC = JumpAddress
      $pc = (IF_ID.instruction & addMask);    // jump to new address
      pcBranch = true;  // don't increment $pc after the jump
      break;
    case 0x03:  // R[31] = PC + 8; PC = JumpAddress
      printf("Link Address = %d\n", $pc+1);
      R[31] = ($pc + 1) << 2; // byte align before adding to register
      $pc = (IF_ID.instruction & addMask);    // jump to new address
      pcBranch = true;  // don't increment $pc after the jump
      break;
    }
    printf("New $pc = %d\n", $pc);
    // once jump occurs a noop should be inserted in its place to make sure nothing executes
    ID_EX.opcodeShadow = 0;
    ID_EX.rsShadow = 0;
    ID_EX.rtShadow = 0;
    ID_EX.rdShadow = 0;
    ID_EX.immShadow = 0;
    ID_EX.shamtShadow = 0;
    ID_EX.functShadow = 0;
    ID_EX.memReadShadow = false;
    
    printf("mainMemory[6] = %d\n", mainMemory[6]);
    printf("mainMemory[7] = %d\n", mainMemory[7]);
    printf("mainMemory[8] = %d\n", mainMemory[8]);
    printf("mainMemory[9] = %d\n", mainMemory[9]);
    printf("Clock Cycles = %d\n\n", clockCycles);
    
  } else { // I-format instruction
    //printf("I-Format Instruction\n");
      
    ID_EX.rsShadow = (IF_ID.instruction & rsMask)>>21;
    ID_EX.rsValueShadow = R[ID_EX.rsShadow];
    ID_EX.rdShadow = (IF_ID.instruction & rtMask)>>16; // rd == rt for I-format instructions
    ID_EX.rdValueShadow = R[ID_EX.rdShadow];
    ID_EX.immShadow = (IF_ID.instruction & immMask);
    if ((ID_EX.immShadow & 0x00008000) > 1) {
        ID_EX.immShadow = ID_EX.immShadow | 0xFFFF0000; // sign extend if negative
    }
    
    // determine if instruction is a memory load
    if (ID_EX.opcodeShadow == 0x24 || ID_EX.opcodeShadow == 0x25 || ID_EX.opcodeShadow == 0x23) {
        ID_EX.memReadShadow = true;
    } else ID_EX.memReadShadow = false;
    
    // determine if instruction is a memory store
    if (ID_EX.opcodeShadow == 0x28 || ID_EX.opcodeShadow == 0x29 || ID_EX.opcodeShadow == 0x2B) {
        if (ID_EX.memRead == true && ID_EX.rdShadow == ID_EX.rd) {  // if value being loaded ahead is what the next instruction is storing, stall
            stallPipe = true;
            //printf("Stalling\n");
        }
    }

    ///////////// branch detection ////////////////
    if (ID_EX.opcodeShadow == 0x04 || ID_EX.opcodeShadow == 0x05 || ID_EX.opcodeShadow == 0x06 || ID_EX.opcodeShadow == 0x07) {
      // hazard protection and forwarding for branch
      if (((ID_EX.rd == ID_EX.rsShadow) && (ID_EX.rsShadow != 0)) || ((ID_EX.rd == ID_EX.rdShadow) && (ID_EX.rdShadow != 0))) {
        stallPipe = true;
        //printf("Stalling\n");
      } else if ((((ID_EX.rd == ID_EX.rsShadow) && (ID_EX.rsShadow != 0)) || ((ID_EX.rd == ID_EX.rdShadow) && (ID_EX.rdShadow != 0))) && EX_MEM.memRead) {
        stallPipe = true;
        //printf("Stalling\n");
      } else if ((EX_MEM.rd == ID_EX.rsShadow) && (ID_EX.rsShadow != 0)) {
        ID_EX.rsValueShadow = EX_MEM.aluOutput;
      } else if ((MEM_WB.rd == ID_EX.rsShadow) && (ID_EX.rsShadow != 0)) {
        ID_EX.rsValueShadow = MEM_WB.aluOutput;
      } else if ((EX_MEM.rd == ID_EX.rdShadow) && (ID_EX.rdShadow != 0)) {
        ID_EX.rdValueShadow = EX_MEM.aluOutput;
      } else if ((MEM_WB.rd == ID_EX.rdShadow) && (ID_EX.rdShadow != 0)) {
        ID_EX.rdValueShadow = MEM_WB.aluOutput;
      }
      // check whether to take branch
      switch(ID_EX.opcodeShadow) {
          case 0x04:    // branch on equal, if(R[rs] == R[rt]) $pc = $pc + 4 + branchAddress
            if ((ID_EX.rsValueShadow == ID_EX.rdValueShadow) && !stallPipe) {
                $pc = $pc + ID_EX.immShadow;
                pcBranch = true;  // don't increment $pc after the jump
                //printf("Branching\n");
            }
            break;
          case 0x05:    // branch on not equal, if (R[rs] != R[rt]) $pc = $pc + 4 + branchAddress
            if ((ID_EX.rsValueShadow != ID_EX.rdValueShadow) && !stallPipe) {
                $pc = $pc + ID_EX.immShadow;
                pcBranch = true;  // don't increment $pc after the jump
                //printf("Branching\n");
            }
          case 0x06:    // branch on less than or equal to zero (R[rs] <= 0) $pc = $pc + 4 + branchAddress
            if ((ID_EX.rsValueShadow <= 0) && !stallPipe) {
                $pc = $pc + ID_EX.immShadow;
                pcBranch = true;  // don't increment $pc after the jump
                //printf("Branching\n");
            }
            break;
          case 0x07:    // branch on greater than zero (R[rs] > 0) $pc = $pc + 4 + branchAddress
            if ((ID_EX.rsValueShadow > 0) && !stallPipe) {
                $pc = $pc + ID_EX.immShadow;
                pcBranch = true;  // don't increment $pc after the jump
                //printf("Branching\n");
            }
        }
        // once branch is determined a noop should be inserted in its place to make sure nothing executes
        ID_EX.opcodeShadow = 0;
        ID_EX.rsShadow = 0;
        ID_EX.rsValueShadow = 0;
        ID_EX.rtShadow = 0;
        ID_EX.rtValueShadow = 0;
        ID_EX.rdShadow = 0;
        ID_EX.rdValueShadow = 0;
        ID_EX.immShadow = 0;
        ID_EX.shamtShadow = 0;
        ID_EX.functShadow = 0;
        ID_EX.memReadShadow = false;  
    }
  }
}
