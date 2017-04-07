// pipeline registers

#include "Load_Program.h"
#include <stdbool.h>

bool branch = false;

// Program Counter Register
unsigned int *$pc;


 typedef struct INSTRUCTION{

   uint32_t opcode;
   int32_t rs;
   int32_t rt;
   int32_t rd;
   uint32_t shamt;
   uint32_t func;
   int32_t iImm;
   uint32_t jImm;
   bool Rform;
   bool Iform;
   bool Jform;

} instruction ;



struct if_id
{
  unsigned int id_pc;
  instruction id_inst;

} IF_ID_shadow, IF_ID;

struct id_ex
{
    unsigned int ex_pc;
    int32_t ex_signext;
    instruction ex_inst;
}ID_EX_shadow,ID_EX;

struct ex_mem
{
    unsigned int mem_branch_addr, zero_branch,jump_addr;
    int32_t alu_result, mem_reg2;
    instruction mem_inst;
    unsigned int mem_pc;
} EX_MEM_shadow, EX_MEM;

struct mem_wb
{
    int32_t read_data, wb_alu_result, dest_reg;
    instruction wb_inst;
    unsigned int wb_pc;

} MEM_WB_shadow, MEM_WB;

// Register File
// R[0] always zero
// Can contain signed numbers in registers
int32_t R[31] = {1};

// Data Memory
int32_t data_mem[];

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

void IF()
{
    if(branch)
    {
        $pc = EX_MEM.mem_branch_addr;
    }
    else
    {   // $pc reads memory address
        $pc = &memory;
        $pc += program_starting_address;
    }

    IF_ID_shadow.id_pc = ++$pc;

    // initialize all instruction fields to zero/false
    IF_ID_shadow.id_inst.opcode =  ( (*$pc & opcode_mask) >> 26 );
    IF_ID_shadow.id_inst.rs = 0;
    IF_ID_shadow.id_inst.rt = 0;
    IF_ID_shadow.id_inst.rd = 0;
    IF_ID_shadow.id_inst.shamt = 0;
    IF_ID_shadow.id_inst.func = 0;
    IF_ID_shadow.id_inst.iImm = 0;
    IF_ID_shadow.id_inst.jImm = 0;
    IF_ID_shadow.id_inst.Rform = false;
    IF_ID_shadow.id_inst.Iform = false;
    IF_ID_shadow.id_inst.Jform = false;

    printf("machine code: %x\n\n",*$pc);

    Move_Shadow_to_Pipeline();

    ID();
}


// decode & read register file
// returns address of next instructions $pc+4
void ID()
{
    ID_EX_shadow.ex_inst = IF_ID.id_inst;

// check for nop --> make equivalent to add 0,0,0
if(*$pc == 0x00000000)
{
    IF_ID.id_inst.func = 0x20;
}
else{
    switch(IF_ID.id_inst.opcode) {

/**************************     R-format Instructions ******************************************/

// arithmetic R
    case 0x00   :

        ID_EX_shadow.ex_inst.Rform = true;
        ID_EX_shadow.ex_inst.shamt = ( (*$pc & shamt_mask) >> 6);
        ID_EX_shadow.ex_inst.func = *$pc & func_mask;

          // read register files
        ID_EX_shadow.ex_inst.rs = R[( (*$pc & rs_mask) >> 21)];
        ID_EX_shadow.ex_inst.rt = R[( (*$pc & rt_mask) >> 16)];
        ID_EX_shadow.ex_inst.rd = R[( (*$pc & rd_mask) >> 11)];

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

// lbu
    case 0x24   :

// lhu
    case 0x25   :

// lw
    case 0x23   :

//  sb
    case 0x28   :

//  sh
    case 0x29   :

//  sw
    case 0x2b   :

// beg  I
    case 0x4    :

/// add bltz

// bgtz I
    case 0x7    :

// blez I
    case 0x6    :

// bne  I
    case 0x5    :
        ID_EX_shadow.ex_inst.Iform = true;

        // read register file
        ID_EX_shadow.ex_inst.rs = R[( (*$pc & rs_mask) >> 21)];
        ID_EX_shadow.ex_inst.rt = R[( (*$pc & rt_mask) >> 16)];

        ID_EX_shadow.ex_inst.iImm = ( (*$pc & imm_mask_i));

        ID_EX_shadow.ex_signext = sign_ext(ID_EX_shadow.ex_inst.iImm);

        break;

 /**************************     J-format Instructions ******************************************/

// j
    case 0x2    :

// jal  J
    case 0x3    :

        ID_EX_shadow.ex_inst.Jform = true;
        ID_EX_shadow.ex_inst.jImm = ( (*$pc & imm_mask_j));

        break;
    }
}

printf("opcode: %d \n",ID_EX_shadow.ex_inst.opcode );
printf("rs: %d \n",ID_EX_shadow.ex_inst.rs);
printf("rt: %d \n",ID_EX_shadow.ex_inst.rt);
printf("rd:%d \n",ID_EX_shadow.ex_inst.rd);
printf("shamt: %d \n", ID_EX_shadow.ex_inst.shamt);
printf("func: %d \n", ID_EX_shadow.ex_inst.func);
printf("iImm: %d \n", ID_EX_shadow.ex_inst.iImm);
printf("jImm: %d \n", ID_EX_shadow.ex_inst.jImm);
printf("R-format: ");
printf( ID_EX_shadow.ex_inst.Rform ? "true" : "false");
printf("\n");
printf("I-format: ");
printf( ID_EX_shadow.ex_inst.Iform ? "true" : "false");
printf("\n");
printf("J-format: ");
printf( ID_EX_shadow.ex_inst.Jform ? "true" : "false");
printf("\n");

ID_EX_shadow.ex_pc = IF_ID.id_pc;

 Move_Shadow_to_Pipeline();

EX();
}



// Execute operation or calculate address
// nop performs add 0,0,0
void EX()
{

    switch(ID_EX.ex_inst.opcode) {

        case 0x0:

                switch(ID_EX.ex_inst.func)
                {
                    //  add
                    case 0x20:
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs  + ID_EX.ex_inst.rt;
                        break;

                    //  addu
                    case 0x21:
                        EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs + (unsigned int)ID_EX.ex_inst.rt;
                        break;

                    //  and
                    case 0x24:
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs & ID_EX.ex_inst.rt;
                        break;

                    // nor
                    case 0x27:
                        EX_MEM_shadow.alu_result = !(ID_EX.ex_inst.rs | ID_EX.ex_inst.rt);
                        break;

                    // or
                    case 0x25:
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs | ID_EX.ex_inst.rt ;
                        break;

                    // sll
                    case 0:
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rt << ID_EX.ex_inst.shamt;
                        break;

                    // srl
                    case 2:
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rt >> ID_EX.ex_inst.shamt;
                        break;

                    // sub
                    case 0x22:
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs - ID_EX.ex_inst.rt;
                        break;

                    //  subu
                    case 0x23:
                        EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs - (unsigned int)ID_EX.ex_inst.rt;
                        break;

                    // xor
                    case 0x26:
                        EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs != ID_EX.ex_inst.rt);
                        break;

                    // slt
                    case 0x2a:
                        EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < ID_EX.ex_inst.rt) ? 1 : 0 ;
                        break;

                    //  sltu
                    case 0x2b:
                        EX_MEM_shadow.alu_result = ((unsigned int)ID_EX.ex_inst.rs < (unsigned int)ID_EX.ex_inst.rt) ? 1 : 0 ;
                        break;

                // end switch
                }

        // addi
        case 8:
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs + ID_EX.ex_signext;
            break;

        // addiu
        case 9:
            EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs + (unsigned int)ID_EX.ex_inst.iImm;
            break;

        //  andi
        case 0xc:
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs & ID_EX.ex_signext ;
            break;

        // ori
        case 0xd:
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs | ID_EX.ex_signext;
            break;

        // xori
        case 0xe:
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs != ID_EX.ex_signext);
            break;

        // lui
        case 0xf:
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.iImm);
            EX_MEM_shadow.alu_result = ((EX_MEM_shadow.alu_result << 16) & 0xFFFF0000 );
            break;

        // slti
        case 0xa:
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < ID_EX.ex_signext) ? 1 : 0 ;
            break;

        // sltiu
        case 0xb:
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < (unsigned int)ID_EX.ex_inst.iImm) ? 1 : 0 ;
            break;

        // beq
        case 4:
            if( ID_EX.ex_inst.rs == ID_EX.ex_inst.rt)
            {
                branch = true;
                EX_MEM_shadow.zero_branch = 1;
                EX_MEM_shadow.mem_branch_addr = ( (ID_EX.ex_signext << 2) + (ID_EX.ex_pc) );
                break;
            }
            else
                break;

        case 1:

                //bltz
                       if( ID_EX.ex_inst.rs < 0)
                            {
                               branch = true;
                                EX_MEM_shadow.zero_branch = 1;
                                EX_MEM_shadow.mem_branch_addr = ( (ID_EX.ex_signext << 2) + (ID_EX.ex_pc) );
                              break;
                            }
                        else
                           break;
        //  bgtz
        case 7:
               if( ID_EX.ex_inst.rs > 0)
                   {
                      branch = true;
                       EX_MEM_shadow.zero_branch = 1;
                       EX_MEM_shadow.mem_branch_addr = ( (ID_EX.ex_signext << 2) + (ID_EX.ex_pc) );
                     break;
                   }
                else
                    break;
        //  blez
        case 6:
                  if( ID_EX.ex_inst.rs <= 0)
                    {
                      branch = true;
                       EX_MEM_shadow.zero_branch = 1;
                       EX_MEM_shadow.mem_branch_addr = ( (ID_EX.ex_signext << 2) + (ID_EX.ex_pc) );
                     break;
                    }
                else
                    break;

        //  bne
        case 5:
            if( ID_EX.ex_inst.rs != ID_EX.ex_inst.rt )
                {
                  branch = true;
                   EX_MEM_shadow.zero_branch = 1;
                   EX_MEM_shadow.mem_branch_addr = ( (ID_EX.ex_signext << 2) + (ID_EX.ex_pc) );
                 return;
                }
            else
                break;

// all these next instructions just calculate address
        //  lbu
        case 0x24:

        //  lhu
        case 0x25:

        //  lw
        case 0x23:

        // sb
        case 0x28:

        // sh
        case 0x29:

        // sw
        case 0x2b:
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs + ID_EX.ex_signext ;
            printf("\n\n alu result: %x \n\n",EX_MEM_shadow.alu_result);
            break;


        // jal  J ( store this value in register #31), do everything jump does (don't return yet)
        case 0x3    : EX_MEM_shadow.alu_result = ++ID_EX.ex_pc;

        // j
        case 0x2    :
                        EX_MEM_shadow.jump_addr = (ID_EX.ex_pc & 0xF0000000) | (ID_EX.ex_inst.jImm << 2);
                        EX_MEM_shadow.zero_branch = 1;
                        branch = true;
                        break;

    // opcode switch
        }

         EX_MEM_shadow.mem_pc = ID_EX.ex_pc;
         EX_MEM_shadow.mem_inst = ID_EX.ex_inst;
         Move_Shadow_to_Pipeline();

         MEM();

}

// Access memory operand
void MEM()
{
    // do nothing for R-format instructions (wait for WB() )

    MEM_WB_shadow.wb_pc = EX_MEM.mem_pc;

    int32_t base_addr = &data_mem;

    unsigned int index;
    if(EX_MEM.alu_result == 0)
    {
        index = 0;
    }
    else if (EX_MEM.alu_result > base_addr)
    {
        index = EX_MEM.alu_result - base_addr;
    }
    else
        index = base_addr + EX_MEM.alu_result;

    switch(EX_MEM.mem_inst.opcode)
    {
        //  lbu
        case 0x24:
            MEM_WB_shadow.read_data = (data_mem[index] & 0x000000FF );
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
            break;

        //  lhu
        case 0x25:
            MEM_WB_shadow.read_data = (data_mem[index] & 0x0000FFFF );
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
            break;

        //  lw
        case 0x23:
            MEM_WB_shadow.read_data = data_mem[index];
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
            break;

        // sb
        case 0x28:
            data_mem[index] = ( EX_MEM.mem_inst.rt & 0x000000FF );
            break;

        // sh
        case 0x29:
            data_mem[index] = ( EX_MEM.mem_inst.rt & 0x0000FFFF );
            break;

        // sw
        case 0x2b:
            data_mem[index] = EX_MEM.mem_inst.rt;
            break ;
    }

    MEM_WB_shadow.wb_inst = EX_MEM.mem_inst;
    MEM_WB_shadow.wb_alu_result = EX_MEM.alu_result;
    Move_Shadow_to_Pipeline();

    WB();
}

// Write result back to register
void WB()
{


    switch(MEM_WB_shadow.wb_inst.opcode)
    {
        case 0x0:

                switch(ID_EX.ex_inst.func)
                {
                    /// all of the following write back to rd
                    //  add
                    case 0x20:

                    //  addu
                    case 0x21:

                    //  and
                    case 0x24:

                    // nor
                    case 0x27:

                    // or
                    case 0x25:

                    // sll
                    case 0:

                    // srl
                    case 2:

                    // sub
                    case 0x22:

                    //  subu
                    case 0x23:

                    // xor
                    case 0x26:

                    // slt
                    case 0x2a:

                    //  sltu
                    case 0x2b:

                                R[ MEM_WB.wb_inst.rd ] = MEM_WB_shadow.wb_alu_result;
                                break;

                // end switch
                }

        /// all of the following write back to rt
        // addi
        case 8:

        // addiu
        case 9:

        //  andi
        case 0xc:

        // ori
        case 0xd:

        // xori
        case 0xe:

        // lui
        case 0xf:

        // slti
        case 0xa:

        // sltiu
        case 0xb:

                    R[ MEM_WB_shadow.wb_inst.rt ] = MEM_WB_shadow.wb_alu_result;
                    break;

        /// all load write back to register rt (.dest_reg)
        //  lbu
        case 0x24:

        //  lhu
        case 0x25:

        //  lw
        case 0x23:
                  R[ MEM_WB.dest_reg ] = MEM_WB.read_data;
                  break;

        // jal  J ( store this value in register #31), do everything jump does (don't return yet)
        case 0x3    :
                  R[31] = ++MEM_WB.wb_pc;
                  break;
    }

    // increment pc

}

void Move_Shadow_to_Pipeline()
{
    IF_ID = IF_ID_shadow;
    ID_EX = ID_EX_shadow;
    EX_MEM = EX_MEM_shadow;
    MEM_WB = MEM_WB_shadow;
}

void Execute_Clock_Cycle(void)
{
   // IF();
  //  ID();
   // EX();
//    MEM();
//    WB();
}


