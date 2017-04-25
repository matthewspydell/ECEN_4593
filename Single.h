#include "Load_Program.h"
#include "pipe_registers.h"
#include <stdbool.h>
int cycle_count=0;
bool stallPipe_RS = false;
bool stallPipe_RT = false;

bool send_RS = false;
bool send_RT = false;

bool jump = false;


bool branch = false;
bool check_branch = false;
bool first_pass = true;
unsigned int count_down = 4;

bool forward_Rs_ex = false;
int32_t rd_to_rs = 0;

bool forward_Rt_ex = false;
int32_t rd_to_rt = 0;

bool forward_Rs_mem = false;
int32_t mem_rd_rs = 0;

bool forward_Rt_mem = false;
int32_t mem_rd_rt = 0;

unsigned int $pc, $sp, $fp, hold_branch_address;

// Register File
int32_t R[31] ={0};


void IF()
{


  //  printf("Current PC: %u",$pc);

    // initialize all instruction fields to zero/false
    IF_ID_shadow.id_inst.opcode =  ( ( (memory[$pc]) & opcode_mask) >> 26 );
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


    IF_ID_shadow.id_pc = $pc;
    IF_ID_shadow.next_pc = $pc+1;

}


// Decode 1st half clock cycle
// Read registers 2nd half clock cycle
void ID()
{
    ID_EX_shadow.ex_inst = IF_ID.id_inst;

    switch(IF_ID.id_inst.opcode) {

/**************************     R-format Instructions ******************************************/

// arithmetic R
    case 0x00   :

        // 1st clock cycle
        ID_EX_shadow.ex_inst.Rform = true;
        ID_EX_shadow.ex_inst.shamt = ( (memory[IF_ID.id_pc] & shamt_mask) >> 6);
        ID_EX_shadow.ex_inst.func = memory[IF_ID.id_pc] & func_mask;

        ID_EX_shadow.RegisterRs = ( (memory[IF_ID.id_pc] & rs_mask) >> 21);
        ID_EX_shadow.RegisterRt= ( (memory[IF_ID.id_pc] & rt_mask) >> 16);

        ID_EX_shadow.RegisterRd = ((memory[IF_ID.id_pc] & rd_mask) >> 11);
        ID_EX_shadow.dest_rd = ((memory[IF_ID.id_pc] & rd_mask) >> 11);

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


        // 1st clock cycle
         ID_EX_shadow.ex_inst.Iform = true;

        ID_EX_shadow.RegisterRs = ( (memory[IF_ID.id_pc] & rs_mask) >> 21);
        ID_EX_shadow.ex_inst.iImm = ( (memory[IF_ID.id_pc] & imm_mask_i));

        ID_EX_shadow.ex_signext = sign_ext(ID_EX_shadow.ex_inst.iImm);

        ID_EX_shadow.RegisterRt = (memory[IF_ID.id_pc] & rt_mask) >> 16;
        ID_EX_shadow.dest_rt = (memory[IF_ID.id_pc] & rt_mask) >> 16;
        ID_EX_shadow.RegisterRd = 0;
        ID_EX_shadow.memRead = false;

        if( (IF_ID.id_inst.opcode == 0x24) || (IF_ID.id_inst.opcode == 0x25) || (IF_ID.id_inst.opcode == 0x23) )
        {
            ID_EX_shadow.memRead = true;
        }

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
        ID_EX_shadow.ex_inst.Iform = true;

        // 1st half clock cycle
        ID_EX_shadow.RegisterRs = ( (memory[IF_ID.id_pc] & rs_mask) >> 21);
        ID_EX_shadow.ex_inst.iImm = ( (memory[IF_ID.id_pc] & imm_mask_i));
        ID_EX_shadow.RegisterRt = ( (memory[IF_ID.id_pc] & rt_mask) >> 16);
        ID_EX_shadow.RegisterRd = 0;

        ID_EX_shadow.ex_signext = sign_ext(ID_EX_shadow.ex_inst.iImm);
        ID_EX_shadow.dest_rt = (memory[IF_ID.id_pc] & rt_mask) >> 16;

        // 2nd half clock cycle, read register file
        ID_EX_shadow.ex_inst.rs = R[ID_EX_shadow.RegisterRs];
        ID_EX_shadow.ex_inst.rt = R[ID_EX_shadow.RegisterRt];

        check_branch = true;

        break;

 /**************************     J-format Instructions ******************************************/

// j
    case 0x2    :
                        ID_EX_shadow.ex_inst.Jform = true;
                        ID_EX_shadow.ex_inst.jImm = ( (memory[IF_ID.id_pc] & imm_mask_j));
                        //                  printf("Operation: jump \n");
                        $pc = (IF_ID.next_pc & 0xF0000000) | (ID_EX_shadow.ex_inst.jImm );
        //              printf("\n jump address: %x\n",  hold_branch_address);
                        jump = true;
                        break;

// jal  J
    case 0x3    :

        jump = true;
        ID_EX_shadow.ex_inst.Jform = true;
        ID_EX_shadow.ex_inst.jImm = ( (memory[IF_ID.id_pc] & imm_mask_j));
        R[31] = (IF_ID.next_pc+1) << 2;
      //                  printf("\n Operation: jal, store next pc = %x \n",R[31]);
        $pc = (IF_ID.next_pc & 0xF0000000) | (ID_EX_shadow.ex_inst.jImm);
        printf("\nJAL   jump address: %d\n",  $pc);

        break;
    }

    ID_EX_shadow.ex_pc = IF_ID.id_pc;
    ID_EX_shadow.next_pc = IF_ID.next_pc;


if(check_branch == true)
{
     // hazard protection and forwarding for branch

     // if lw then branch, insert two bubbles

     if( (ID_EX_shadow.RegisterRs ==  ID_EX.dest_reg) && (ID_EX.dest_reg != 0 ) )
     {
         stallPipe_RS = true;
     }
     if( (ID_EX_shadow.RegisterRt ==  ID_EX.dest_reg) && (ID_EX.dest_reg != 0) )
     {
         stallPipe_RT = true;
     }

check_branch = false;

    switch(IF_ID.id_inst.opcode)
    {
        // beq
        case 4:

            if(send_RS == true)
                {
                    ID_EX_shadow.ex_inst.rs = EX_MEM.alu_result;
                    send_RS = false;
                }
            if (send_RT == true)
            {
                ID_EX_shadow.ex_inst.rt = EX_MEM.alu_result;
                send_RT = false;
            }

            if( (ID_EX_shadow.ex_inst.rs == ID_EX_shadow.ex_inst.rt) && (!stallPipe_RS ) && (!stallPipe_RT) ) {

                hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                branch = true;
                break;
            }
            else if ((!stallPipe_RS) && (!stallPipe_RT) )
               {
                branch = false;
                break; }

            else
                break;

        case 1:

                //bltz

                if(send_RS == true)
                {
                    ID_EX_shadow.ex_inst.rs = EX_MEM.alu_result;
                    send_RS = false;
                }

                 if( (ID_EX_shadow.ex_inst.rs < 0) && (!stallPipe_RS )  ) {

                    hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                    branch = true;
                    break;
                    }

                 else if (!stallPipe_RS)
                   {
                    branch = false;
                    break; }

                else
                    break;


        //  bgtz
        case 7:

                if(send_RS == true)
                {
                    ID_EX_shadow.ex_inst.rs = EX_MEM.alu_result;
                    send_RS = false;
                }

               if( (ID_EX_shadow.ex_inst.rs > 0) && (!stallPipe_RS ) ) {
                       hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                       branch = true;
                    break;
                   }
                else if (!stallPipe_RS)
                    {branch = false;
                    break;}
                else
                    break;

        //  blez
        case 6:

            if(send_RS == true)
                {
                    ID_EX_shadow.ex_inst.rs = EX_MEM.alu_result;
                    send_RS = false;
                }

                if( (ID_EX_shadow.ex_inst.rs <= 0) && (!stallPipe_RS ))
                    {
                        hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                        branch = true;
                        break;
                    }
                else if (!stallPipe_RS)
                    {branch = false;
                    break;}

                else
                    break;

        //  bne
        case 5:

            if(send_RS == true)
                {
                    ID_EX_shadow.ex_inst.rs = EX_MEM.alu_result;
                    send_RS = false;
                }
            if (send_RT == true)
            {
                ID_EX_shadow.ex_inst.rt = EX_MEM.alu_result;
                send_RT = false;
            }

            if( (ID_EX_shadow.ex_inst.rs != ID_EX_shadow.ex_inst.rt ) && (!stallPipe_RS) && (!stallPipe_RT) )
                {
                    hold_branch_address = ( ( ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                    branch = true;
                    break;
                }
            else if ((!stallPipe_RS) && (!stallPipe_RT))
                {branch = false;
                break;}

            else
                break;
    }


}

            // check for jump register
    if((IF_ID.id_inst.opcode == 0) & (ID_EX_shadow.ex_inst.func == 8) )
       {
           jump = true;
           $pc = (ID_EX_shadow.ex_inst.rs >> 2);
            printf("JR\n");
       }

    if(ID_EX_shadow.ex_inst.Rform == true)
    {
        ID_EX_shadow.dest_reg = ID_EX_shadow.RegisterRd;
    }
    if(ID_EX_shadow.ex_inst.Iform == true)
    {
        ID_EX_shadow.dest_reg = ID_EX_shadow.RegisterRt;
    }

}



// Execute operation or calculate address
// nop performs sll 0,0,0
void EX()
{

    EX_MEM_shadow.mem_inst.rt = R[ID_EX.RegisterRt];
    EX_MEM_shadow.mem_inst.rs = R[ID_EX.RegisterRs];


    EX_MEM_shadow.dest_rd = ID_EX.dest_rd;
    EX_MEM_shadow.dest_rt = ID_EX.dest_rt;

    EX_MEM_shadow.mem_pc = ID_EX.ex_pc;
    EX_MEM_shadow.next_pc = ID_EX.next_pc;

    EX_MEM_shadow.RegisterRd = ID_EX.RegisterRd;
    EX_MEM_shadow.RegisterRs = ID_EX.RegisterRs;
    EX_MEM_shadow.RegisterRt = ID_EX.RegisterRt;

    EX_MEM_shadow.memRead = ID_EX.memRead;

        // Check if data forwarding is needed
    if( (EX_MEM.RegWrite == true ) && (EX_MEM.dest_reg !=0) && (EX_MEM.dest_reg == ID_EX.RegisterRs) )
    {
        forward_Rs_ex = true;
        rd_to_rs = EX_MEM.alu_result;
        EX_MEM_shadow.RegWrite = false;
    }

    if( (EX_MEM.RegWrite == true) && (EX_MEM.dest_reg  != 0) && (EX_MEM.dest_reg == ID_EX.RegisterRt))
    {
        forward_Rt_ex = true;
        rd_to_rt = EX_MEM.alu_result;
        EX_MEM.RegWrite = false;
    }
    if((MEM_WB.RegWrite == true) && (MEM_WB.dest_reg !=0) && (MEM_WB.dest_reg == ID_EX.RegisterRs))
    {
        forward_Rs_mem = true;
        mem_rd_rs = MEM_WB.wb_alu_result;
        MEM_WB.RegWrite = false;
    }
     if((MEM_WB.RegWrite == true) && (MEM_WB.dest_reg !=0) && (MEM_WB.dest_reg == ID_EX.RegisterRt))
    {
        forward_Rt_ex = true;
        mem_rd_rt = MEM_WB.wb_alu_result;
        MEM_WB.RegWrite = false;
    }


    switch(ID_EX.ex_inst.opcode) {

        case 0x0:

                if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }

                if (forward_Rt_ex == true)
                    {
                        ID_EX.ex_inst.rt = rd_to_rt;
                        forward_Rt_ex = false;
                    }
                if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
                if(forward_Rt_mem == true)
                {
                    ID_EX.ex_inst.rt = mem_rd_rt;
                    forward_Rt_mem = false;
                }

                EX_MEM_shadow.RegWrite = true;
                switch(ID_EX.ex_inst.func)
                {
                    //  add
                    case 0x20:

          //              printf("\n OPERATION: add \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs  + ID_EX.ex_inst.rt;
            //            printf(" %d = %d + %d \n", EX_MEM_shadow.alu_result,ID_EX.ex_inst.rs,ID_EX.ex_inst.rt );
                        break;

                    //  addu
                    case 0x21:

              //          printf("\n OPERATION: addu \n");
                        EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs + (unsigned int)ID_EX.ex_inst.rt;
                //        printf(" %u = %u + %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.rt );

                        break;

                    //  and
                    case 0x24:
                  //       printf("\n OPERATION: and \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs & ID_EX.ex_inst.rt;
                    //    printf(" %d = %d & %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // nor
                    case 0x27:
                      //   printf("\n OPERATION: nor \n");
                        EX_MEM_shadow.alu_result = !(ID_EX.ex_inst.rs | ID_EX.ex_inst.rt);
                        //printf(" %d = !(%d | %d) \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // or
                    case 0x25:
                    //    printf("\n OPERATION: or \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs | ID_EX.ex_inst.rt ;
                      //  printf(" %d = %d | %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // sll
                    case 0:
                       //  printf("\n OPERATION: sll \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rt << ID_EX.ex_inst.shamt;
                         //printf(" %d = %d << %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rt, ID_EX.ex_inst.shamt );
                        break;

                    // srl
                    case 2:
                      //   printf("\n OPERATION: srl \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rt >> ID_EX.ex_inst.shamt;
                        //printf(" %d = %d >> %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rt, ID_EX.ex_inst.shamt );
                        break;

                    // sub
                    case 0x22:
             //           printf("\n OPERATION: sub \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs - ID_EX.ex_inst.rt;
               //         printf(" %d = %d - %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    //  subu
                    case 0x23:
                 //        printf("\n OPERATION: subu \n");
                        EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs - (unsigned int)ID_EX.ex_inst.rt;
                   //     printf(" %u = %u - %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.rt );
                        break;

                    // xor
                    case 0x26:
                     //   printf("\n OPERATION: xor \n");
                        EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs != ID_EX.ex_inst.rt);
                       // printf(" %d = %d != %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // slt
                    case 0x2a:
                   //      printf("\n OPERATION: slt \n");
                        EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < ID_EX.ex_inst.rt) ? 1 : 0 ;
                     //   printf(" %d = %d < %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    //  sltu
                    case 0x2b:
                       //  printf("\n OPERATION: sltu \n");
                        EX_MEM_shadow.alu_result = ((unsigned int)ID_EX.ex_inst.rs < (unsigned int)ID_EX.ex_inst.rt) ? 1 : 0 ;
                         //printf(" %u = %u < %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.rt );
                        break;

                // end switch
                }break;

        // addi
        case 8:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }

            EX_MEM_shadow.RegWrite = true;
     //       printf("\n OPERATION: addi \n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs + ID_EX.ex_signext;
       //     printf(" %d = %d + %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // addiu
        case 9:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
         //   printf("\n OPERATION: addiu \n");
            EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs + ID_EX.ex_signext;
           // printf(" %d = %d + %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.iImm );
            break;

        //  andi
        case 0xc:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
    //        printf("\n OPERATION: andi\n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs & ID_EX.ex_signext ;
      //       printf(" %d = %d & %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // ori
        case 0xd:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
        //    printf("\n OPERATION: ori\n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs | ID_EX.ex_signext;
          //  printf(" %d = %d | %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // xori
        case 0xe:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
        //    printf("\n OPERATION: xori\n");
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs != ID_EX.ex_signext);
          //  printf(" %d = %d != %d \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // lui
        case 0xf:
            EX_MEM_shadow.RegWrite = true;
            //printf("\n OPERATION: lui\n");
         //   printf("immediate value: %u\n", ID_EX.ex_inst.iImm);
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.iImm);
            EX_MEM_shadow.alu_result = ((EX_MEM_shadow.alu_result << 16) & 0xFFFF0000 );
           // printf("alu result: %d = %d << 16  \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.iImm );
            break;

        // slti
        case 0xa:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
     //       printf("\n OPERATION: slti\n");
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < ID_EX.ex_signext) ? 1 : 0 ;
       //     printf("result = %d < %d \n",ID_EX.ex_inst.rs, ID_EX.ex_signext);
            break;

        // sltiu
        case 0xb:
            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
         //   printf("\n OPERATION: sltiu\n");
            EX_MEM_shadow.alu_result = ((unsigned int)ID_EX.ex_inst.rs < (unsigned int)ID_EX.ex_inst.iImm) ? 1 : 0 ;
           // printf("result = %u < %u \n",ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.iImm);
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

            if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex = false;
                    }
            if (forward_Rs_mem == true)
                {
                    ID_EX.ex_inst.rs = mem_rd_rs;
                    forward_Rs_mem = false;
                }
            EX_MEM_shadow.RegWrite = true;
       //     printf(" \n load or store instruction\n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs + ID_EX.ex_signext ;
         //   printf("\n\n address calculated: %d = %d + %d \n\n",EX_MEM_shadow.alu_result,ID_EX.ex_inst.rs,ID_EX.ex_signext);
            break;

    // opcode switch
        }

         EX_MEM_shadow.mem_inst = ID_EX.ex_inst;

    if(ID_EX.ex_inst.Iform == true)
    {
        EX_MEM_shadow.dest_reg = ID_EX.RegisterRt;

    }
    else
          EX_MEM_shadow.dest_reg = ID_EX.RegisterRd;



}

// Access memory operand
void MEM()
{
    // do nothing for R-format instructions (wait for WB() )

    MEM_WB_shadow.wb_pc = EX_MEM.mem_pc;
    MEM_WB_shadow.next_pc = EX_MEM.next_pc;


    MEM_WB_shadow.dest_rd = EX_MEM.dest_rd;
    MEM_WB_shadow.dest_rt = EX_MEM.dest_rt;

    MEM_WB_shadow.RegisterRd = EX_MEM.RegisterRd;
    MEM_WB_shadow.RegisterRs = EX_MEM.RegisterRs;
    MEM_WB_shadow.RegisterRt = EX_MEM.RegisterRt;

    MEM_WB_shadow.memRead = EX_MEM.memRead;

    if(EX_MEM.mem_inst.Iform == true)
    {
        MEM_WB_shadow.dest_reg = EX_MEM.RegisterRt;

    }
    else
         MEM_WB_shadow.dest_reg = EX_MEM.RegisterRd;

    MEM_WB_shadow.wb_inst = EX_MEM.mem_inst;


    unsigned int index = (EX_MEM.alu_result >> 2);

  //  printf("\n Data MemAddr Corrected : %d \n", index);
    MEM_WB_shadow.RegWrite = EX_MEM.RegWrite;

    switch(EX_MEM.mem_inst.opcode)
    {
        //  lbu
        case 0x24:
    //        printf("Confirm Operation: lbu\n");
            MEM_WB_shadow.read_data = (memory[index] & 0x000000FF );
      //      printf("data read: %u\n",MEM_WB_shadow.read_data);
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
        //    printf("destination register: %u\n", MEM_WB_shadow.dest_reg);
            break;

        //  lhu
        case 0x25:
          //   printf("Confirm Operation: lhu\n");
            MEM_WB_shadow.read_data = (memory[index] & 0x0000FFFF );
            //printf("data read: %u\n",MEM_WB_shadow.read_data);
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
        //    printf("destination register: %u\n", MEM_WB_shadow.dest_reg);
            break;

        //  lw
        case 0x23:
          //  printf("Confirm Operation: lw \n");
            MEM_WB_shadow.read_data = memory[index];
            //printf("data read: %u\n",MEM_WB_shadow.read_data);
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
         //   printf("destination register: %u\n", MEM_WB_shadow.dest_rt);
            break;

        // sb
        case 0x28:
           // printf("Confirm Operation: sb\n");
            memory[index] = ( EX_MEM.mem_inst.rt & 0x000000FF );
      //      printf("Data Written : %u", memory[index]);
            break;

        // sh
        case 0x29:
        //    printf("Confirm Operation: sh\n");
            memory[index] = ( EX_MEM.mem_inst.rt & 0x0000FFFF );
          //  printf("Data Written : %u", memory[index]);
            break;

        // sw
        case 0x2b:
          //  printf("Confirm Operation: sw\n");
            memory[index] = EX_MEM.mem_inst.rt;
            //printf("Data Written : %u", memory[index]);
            break ;
    }


    MEM_WB_shadow.wb_alu_result = EX_MEM.alu_result;

}

// Write result back to register
// Update pc
void WB()
{

    if(count_down != 0)
        return;

    switch(MEM_WB.wb_inst.opcode)
    {
        case 0:

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

                                R[ MEM_WB.dest_rd] = MEM_WB.wb_alu_result;
 //                               printf("\n\n WB(): destination reg = %u   value written = %u\n\n",MEM_WB.dest_rd, MEM_WB.wb_alu_result );
                                break;

                // end switch
                } break;

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

                    R[ MEM_WB.dest_rt ] = MEM_WB.wb_alu_result;
   //                 printf("\n\n WB(): destination reg = %u   value written = %u\n\n",MEM_WB.dest_rt, MEM_WB.wb_alu_result );
                    break;

        /// all load write back to register rt (.dest_reg)
        //  lbu
        case 0x24:

        //  lhu
        case 0x25:

        //  lw
        case 0x23:
                  R[ MEM_WB.dest_rt] = MEM_WB.read_data;
     //             printf("\n\n WB(): destination reg = %u   value written = %u\n\n",MEM_WB.dest_rt, MEM_WB.read_data);
                  break;
    }

}

void Move_Shadow_to_Pipeline()
{
if ((!stallPipe_RS) && (!stallPipe_RT)) {

        IF_ID = IF_ID_shadow;

        ID_EX = ID_EX_shadow;
    }
else
{

    ID_EX.ex_inst.func = 0;
    ID_EX.ex_inst.Iform = false;
    ID_EX.ex_inst.iImm = 0;
    ID_EX.ex_inst.Jform = false;
    ID_EX.ex_inst.jImm = 0;
    ID_EX.ex_inst.opcode = 0;
    ID_EX.ex_inst.rd = 0;
    ID_EX.ex_inst.Rform = true ;
    ID_EX.ex_inst.rs = 0;
    ID_EX.ex_inst.rt = 0;
    ID_EX.dest_reg = 0;
    ID_EX.ex_inst.shamt = 0;
    ID_EX.ex_signext = 0;

    if(stallPipe_RS == true)
    {
        send_RS = true;
    }
    if(stallPipe_RT == true)
    {
        send_RT = true;
    }

}

    EX_MEM = EX_MEM_shadow;
    MEM_WB = MEM_WB_shadow;

}

void Execute_Clock_Cycle()
{
   IF();
   WB();
   ID();
   EX();
   MEM();

   if(count_down > 0)
    count_down = count_down-1;

   Move_Shadow_to_Pipeline();

    if(branch == true)
   {
       $pc = hold_branch_address;
       branch = false;
   }
   else if (jump == true)
   {
       jump = false;
   }
    else
      {
        if((!stallPipe_RS) && (!stallPipe_RT))
        {$pc = $pc+1;}

        stallPipe_RS = false;
        stallPipe_RT = false;

        }

}

void clear_shadow()
{
    IF_ID_shadow.id_inst.func = 0;
    IF_ID_shadow.id_inst.Iform = false;
    IF_ID_shadow.id_inst.iImm = 0;
    IF_ID_shadow.id_inst.Jform = false;
    IF_ID_shadow.id_inst.jImm = 0;
    IF_ID_shadow.id_inst.opcode = 0;
    IF_ID_shadow.id_inst.rd = 0;
    IF_ID_shadow.id_inst.Rform = false;
    IF_ID_shadow.id_inst.rs = 0;
    IF_ID_shadow.id_inst.rt = 0;
    IF_ID_shadow.id_inst.shamt = 0;
    IF_ID_shadow.id_pc = 0;


     ID_EX_shadow.ex_inst.func = 0;
    ID_EX_shadow.ex_inst.Iform = false;
    ID_EX_shadow.ex_inst.iImm = 0;
   ID_EX_shadow.ex_inst.Jform = false;
    ID_EX_shadow.ex_inst.jImm = 0;
    ID_EX_shadow.ex_inst.opcode = 0;
    ID_EX_shadow.ex_inst.rd = 0;
    ID_EX_shadow.ex_inst.Rform = false;
    ID_EX_shadow.ex_inst.rs = 0;
   ID_EX_shadow.ex_inst.rt = 0;
   ID_EX_shadow.ex_inst.shamt = 0;
   ID_EX_shadow.ex_pc = 0;
   ID_EX_shadow.ex_signext = 0;

   EX_MEM_shadow.alu_result = 0;
   EX_MEM_shadow.jump_addr = 0;
   EX_MEM_shadow.mem_branch_addr = 0;
   EX_MEM_shadow.mem_inst.func = 0;
    EX_MEM_shadow.mem_inst.Iform = false;
    EX_MEM_shadow.mem_inst.iImm = 0;
   EX_MEM_shadow.mem_inst.Jform = false;
    EX_MEM_shadow.mem_inst.jImm = 0;
    EX_MEM_shadow.mem_inst.opcode = 0;
    EX_MEM_shadow.mem_inst.rd = 0;
    EX_MEM_shadow.mem_inst.Rform = false;
    EX_MEM_shadow.mem_inst.rs = 0;
   EX_MEM_shadow.mem_inst.rt = 0;
   EX_MEM_shadow.mem_inst.shamt = 0;
   EX_MEM_shadow.mem_pc = 0;
   EX_MEM_shadow.mem_reg2=0;
   EX_MEM_shadow.zero_branch = 0;


   MEM_WB_shadow.dest_reg = 0;
   MEM_WB_shadow.read_data = 0;
   MEM_WB_shadow.wb_alu_result = 0;
      MEM_WB_shadow.wb_inst.func = 0;
    MEM_WB_shadow.wb_inst.Iform = false;
    MEM_WB_shadow.wb_inst.iImm = 0;
   MEM_WB_shadow.wb_inst.Jform = false;
    MEM_WB_shadow.wb_inst.jImm = 0;
   MEM_WB_shadow.wb_inst.opcode = 0;
    MEM_WB_shadow.wb_inst.rd = 0;
    MEM_WB_shadow.wb_inst.Rform = false;
    MEM_WB_shadow.wb_inst.rs = 0;
   MEM_WB_shadow.wb_inst.rt = 0;
   MEM_WB_shadow.wb_inst.shamt = 0;
   MEM_WB_shadow.wb_pc = 0;

}


