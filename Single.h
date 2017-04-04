// pipeline registers

#include "Load_Program.h"


// Program Counter Register
unsigned int *$pc;


 typedef struct INSTRUCTION{

   uint32_t opcode;
   uint32_t rs;
   uint32_t rt;
   uint32_t rd;
   uint32_t shamt;
   uint32_t func;
   int32_t iImm;
   uint32_t jImm;
   bool Rform;
   bool Iform;
   bool Jform;

} instruction ;

 instruction currentInst;

struct if_id
{
  unsigned int p_counter;
  instruction copy_inst;

} IF_ID_shadow, IF_ID;


// index 0=$pc+4 value      1=address of instruction struct
uint32_t ID_EX[], ID_EX_shadow[];

// index 0=$pc+4 value      1=address of instruction struct
uint32_t EX_MEM[], EX_MEM_shadow [];

// index 0=$pc+4 value      1=address of instruction struct
uint32_t MEM_WB[], MEM_WB_shadow[];



void IF()
{
    // $pc reads memory address
    $pc = &memory;
    $pc += program_starting_address;

    printf("machine code: %x\n\n",*$pc);

    ID();
}

// Instruction Decode and Reading Registers

// Register File
// R[0] always zero
// Can contain signed numbers in registers
int32_t R[31] = {1};

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



// decode & read register file
// returns address of next instructions $pc+4
void ID()
{
    currentInst.opcode =  ( (*$pc & opcode_mask) >> 26 );
    currentInst.rs = 0;
    currentInst.rt = 0;
    currentInst.rd = 0;
    currentInst.shamt = 0;
    currentInst.func = 0;
    currentInst.iImm = 0;
    currentInst.jImm = 0;
    currentInst.Rform = false;
    currentInst.Iform = false;
    currentInst.Jform = false;

    switch(currentInst.opcode) {

/**************************     R-format Instructions ******************************************/

// arithmetic R
    case 0x00   :

        currentInst.Rform = true;
        currentInst.shamt = ( (*$pc & shamt_mask) >> 6);
        currentInst.func = *$pc & func_mask;

          // read register files
        currentInst.rs = R[( (*$pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*$pc & rt_mask) >> 16)];
        currentInst.rd = R[( (*$pc & rd_mask) >> 11)];


        break;

/**************************     I-format Instructions ******************************************/

// These perform operation and put result into rt
// addi I
    case 0x08   :

// addiu I
    case 0x09   :

// andi I
    case 0xc    :

// ori  I
    case 0xd    :

// xori I
    case 0xe    :

// lui  I
    case 0xf    :

// slti I
    case 0xa    :

//  sltiu   I
    case 0xb    :

        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*$pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*$pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*$pc & imm_mask_i));


        break;


// lbu
    case 0x24   :

// lhu
    case 0x25   :

// lw
    case 0x23   :

        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*$pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*$pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*$pc & imm_mask_i));

        break;


//  sb
    case 0x28   :

//  sh
    case 0x29   :

//  sw
    case 0x2b   :

        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*$pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*$pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*$pc & imm_mask_i));

        break;

// beg  I
    case 0x4    :

/// add bltz

// bgtz I
    case 0x7    :

// blez I
    case 0x6    :

// bne  I
    case 0x5    :
        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*$pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*$pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*$pc & imm_mask_i));

        break;

 /**************************     J-format Instructions ******************************************/

// j
    case 0x2    :

// jal  J
    case 0x3    :

        currentInst.Jform = true;
        currentInst.jImm = ( (*$pc & imm_mask_j));

        break;
    }

printf("opcode: %d \n",currentInst.opcode );
printf("rs: %d \n",currentInst.rs);
printf("rt: %d \n",currentInst.rt);
printf("rd:%d \n",currentInst.rd);
printf("shamt: %d \n", currentInst.shamt);
printf("func: %d \n", currentInst.func);
printf("iImm: %d \n", currentInst.iImm);
printf("jImm: %d \n", currentInst.jImm);
printf("R-format: ");
printf( currentInst.Rform ? "true" : "false");
printf("\n");
printf("I-format: ");
printf( currentInst.Iform ? "true" : "false");
printf("\n");
printf("J-format: ");
printf( currentInst.Jform ? "true" : "false");
printf("\n");

 ++$pc;

IF_ID_shadow.p_counter = $pc;
IF_ID_shadow.copy_inst = currentInst;

}

int32_t sign_ext(int16_t value)
{
   int32_t new_immediate = value;
     printf("%x\n",value);

   // positive sign bit
   if(((new_immediate >> 16)& 0x0000000F)== 0)
   {

       printf("sign bit equals 0\n");
       // set upper bits to 0's
       new_immediate = (new_immediate & 0x0000FFFF);
       printf("%x\n",new_immediate);
       return new_immediate;
   }

   // only leave sign bit, negative sign bit case
   else if ((((!new_immediate) >> 16)& 0x0000000F) == 0)
   {
       printf("sign bit equals 1\n");
       // set upper bits to 1's
       new_immediate = (new_immediate | 0xFFFF0000);
       printf("%x\n",new_immediate);
       return new_immediate;
   }

   else
   {
       printf("Error in sign extension.");
       return;
   }

}

// Execute operation or calculate address
uint32_t EX(void)
{

    uint32_t math_result = 0;
    uint32_t mem_addr = 0;
    uint32_t branch_target = 0;

    uint32_t mem_index = 0;

// add nop!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// add jal

    switch(currentInst.opcode) {

        case 0x0:

                switch(currentInst.func)
                {
                    //  add
                    case 0x20:
                        return math_result = currentInst.rs  + currentInst.rt;

                    //  addu
                    case 0x21:
                        return math_result = currentInst.rs + currentInst.rt;

                    //  and
                    case 0x24:
                        return math_result = currentInst.rs & currentInst.rt;

                    // nor
                    case 0x27:
                        return math_result = !(currentInst.rs | currentInst.rt);

                    // or
                    case 0x25:
                        return math_result = currentInst.rs | currentInst.rt ;

                    // sll
                    case 0:
                        return math_result = currentInst.rt << currentInst.shamt;

                    // srl
                    case 2:
                        return math_result = currentInst.rt >> currentInst.shamt;

                    // sub
                    case 0x22:
                        return math_result = currentInst.rs - currentInst.rt;

                    //  subu
                    case 0x23:
                        return math_result = currentInst.rs - currentInst.rt;

                    // xor
                    case 0x26:
                        return math_result = (currentInst.rs != currentInst.rt);

                    // slt
                    case 0x2a:
                        return math_result = (currentInst.rs < currentInst.rt) ? 1 : 0 ;

                    //  sltu
                    case 0x2b:
                        return math_result = (currentInst.rs < currentInst.rt) ? 1 : 0 ;

                // end function switch
                }
        // addi
        case 8:
            return math_result = currentInst.rs + currentInst.iImm;

        // addiu
        case 9:
            return math_result = currentInst.rs + currentInst.iImm;

        //  andi
        case 0xc:
            return math_result = currentInst.rs & currentInst.iImm ;

        // ori
        case 0xd:
            return math_result = currentInst.rs | currentInst.iImm;

        // xori
        case 0xe:
            return math_result = (currentInst.rs != currentInst.iImm);

        // lui
        case 0xf:
            // ?????

        // slti
        case 0xa:
            return math_result = (currentInst.rs < currentInst.iImm) ? 1 : 0 ;

        // sltiu
        case 0xb:
            return math_result = (currentInst.rs < currentInst.rt) ? 1 : 0 ;

        // beq
        case 4:
            if( currentInst.rs == currentInst.rt)
            {
                //
                return;
            }
            else
                return;

        case 1:

            switch(currentInst.rt)
                {

                //bltz
                case 0:
                       if( currentInst.rs < 0)
                            {
                              //
                              return;
                            }
                        else
                           return;

            // end op:1 switch
                }

        //  bgtz
        case 7:
               if( currentInst.rs > 0)
                   {
                     //
                     return;
                   }
                else
                    return;
        //  blez
        case 6:
                  if( currentInst.rs <= 0)
                    {
                     //
                     return;
                    }
                else
                    return;

        //  bne
        case 5:
            if( currentInst.rs != currentInst.rt )
                {
                 //
                 return;
                }
            else
                return;

        //  lbu
        case 0x24:

        //  lhu
        case 0x25:

        //  lw
        case 0x23:
            return mem_addr = (sign_ext(currentInst.iImm) + currentInst.rs);

        // sb
        case 0x28:

        // sh
        case 0x29:

        // sw
        case 0x2b:
            return mem_addr = currentInst.rs + sign_ext(currentInst.iImm) ;


    // opcode switch
        }

}

// Access memory operand
void MEM()
{
}

// Write result back to register
void WB()
{
}

void Move_Shadow_to_Pipeline()
{

}

void Execute_Clock_Cycle(void)
{
    IF();
  //  ID();
   // EX();
//    MEM();
//    WB();
}


