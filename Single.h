#include "Load_Program.h"
#include "pipe_registers.h"
#include <stdbool.h>

bool branch = false;
bool first_pass = true;
unsigned int count_down = 3;

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


    printf("Current PC: %u",$pc);

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

    printf("\n \n MACHINE CODE : %x\n",memory[IF_ID.id_pc]);

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

        // 2nd half clock cycle
        ID_EX_shadow.ex_inst.rs = R[ID_EX_shadow.RegisterRs];
        ID_EX_shadow.ex_inst.rt = R[ID_EX_shadow.RegisterRt];

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

        // 2nd half clock cycle , read registers
        ID_EX_shadow.ex_inst.rs = R[ID_EX_shadow.RegisterRs];
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

        ID_EX_shadow.ex_signext = sign_ext(ID_EX_shadow.ex_inst.iImm);
        ID_EX_shadow.dest_rt = (memory[IF_ID.id_pc] & rt_mask) >> 16;

        // 2nd half clock cycle, read register file
        ID_EX_shadow.ex_inst.rs = R[ID_EX_shadow.RegisterRs];
        ID_EX_shadow.ex_inst.rt = R[ID_EX_shadow.RegisterRt];

        branch = true;

        break;

 /**************************     J-format Instructions ******************************************/

// j
    case 0x2    :

// jal  J
    case 0x3    :

        ID_EX_shadow.ex_inst.Jform = true;
        ID_EX_shadow.ex_inst.jImm = ( (memory[IF_ID.id_pc] & imm_mask_j));
        branch = true;

        break;
    }

    ID_EX_shadow.ex_pc = IF_ID.id_pc;
    ID_EX_shadow.next_pc = IF_ID.next_pc;


printf("\n\nopcode: %d \n",ID_EX_shadow.ex_inst.opcode );
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


if(branch == true)
{
    switch(IF_ID.id_inst.opcode)
    {
        // beq
        case 4:
            printf("\n OPERATION: beq\n");
            printf("CHECK : %u = %u\n", ID_EX_shadow.ex_inst.rs, ID_EX_shadow.ex_inst.rt);

            if( ID_EX_shadow.ex_inst.rs == ID_EX_shadow.ex_inst.rt){

                printf("BRANCH TAKEN \n");
                hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                printf("branch address: %d\n\n", hold_branch_address );
                break;
            }
            else
                printf("Branch NOT Taken\n");
                break;

        case 1:

                //bltz
                 printf("\n OPERATION: bltz\n");
                printf("CHECK : %u < 0 \n",ID_EX_shadow.ex_inst.rs );

                 if( ID_EX_shadow.ex_inst.rs < 0){

                    printf("BRANCH TAKEN\n\n");
                    hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                    printf("branch address: %d\n\n", hold_branch_address );
                    break;
                    }

                 else
                    printf("Branch NOT Taken\n");
                    break;


        //  bgtz
        case 7:
                printf("\n OPERATION: bltz\n");
                printf("CHECK: %u > 0 \n",ID_EX_shadow.ex_inst.rs);

               if( ID_EX.ex_inst.rs > 0){

                       printf("BRANCH TAKEN\n\n");
                       hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                       printf("branch address: %d\n\n", hold_branch_address);
                    break;
                   }
                else
                    printf("Branch NOT Taken\n");
                    break;

        //  blez
        case 6:
                printf("\n OPERATION: blez\n");
                printf("CHECK: %u <= 0\n",ID_EX_shadow.ex_inst.rs );

                if( ID_EX.ex_inst.rs <= 0)
                    {
                        printf("BRANCH TAKEN\n\n");
                        hold_branch_address = ( (ID_EX_shadow.ex_signext ) + (IF_ID.next_pc) );
                        printf("branch address: %d \n\n", hold_branch_address );
                        break;
                    }
                else
                    printf("Branch NOT Taken\n");
                    break;

        //  bne
        case 5:
            printf("\n OPERATION: bne\n");
            printf("CHECK: %u != %u \n",ID_EX_shadow.ex_inst.rs, ID_EX_shadow.ex_inst.rs );

            if( ID_EX_shadow.ex_inst.rs != ID_EX_shadow.ex_inst.rs )
                {
                    printf("BRANCH TAKEN\n\n");
                    hold_branch_address = ( (int)( ID_EX_shadow.ex_signext ) + (signed)(IF_ID.next_pc) );
                    printf("branch address: %d= %d + %d \n\n", hold_branch_address, ID_EX_shadow.ex_signext, IF_ID.next_pc );
                    break;
                }
            else
                printf("Branch NOT Taken\n");
                break;

    // j
    case 0x2    :
                        printf("Operation: jump \n");
                        hold_branch_address = (IF_ID.next_pc & 0xF0000000) | (ID_EX.ex_inst.jImm << 2);
                        printf("\n jump address: %x\n",  hold_branch_address);
                        break;

    // jal  J
    case 0x3    :

                        R[31] = IF_ID.next_pc;
                        printf("\n Operation: jal, store next pc = %x \n",R[31]);
                        hold_branch_address = (IF_ID.next_pc & 0xF0000000) | (ID_EX.ex_inst.jImm << 2);
                        printf("\n jump address: %x\n",  hold_branch_address);
                        break;

    }
}

}



// Execute operation or calculate address
// nop performs sll 0,0,0
void EX()
{

    EX_MEM_shadow.dest_rd = ID_EX.dest_rd;
    EX_MEM_shadow.dest_rt = ID_EX.dest_rt;

    EX_MEM_shadow.mem_pc = ID_EX.ex_pc;
    EX_MEM_shadow.next_pc = ID_EX.next_pc;

    EX_MEM_shadow.RegisterRd = ID_EX.RegisterRd;
    EX_MEM_shadow.RegisterRs = ID_EX.RegisterRs;
    EX_MEM_shadow.RegisterRt = ID_EX.RegisterRt;


    switch(ID_EX.ex_inst.opcode) {

        case 0x0:

                if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
                EX_MEM_shadow.RegWrite = true;
                switch(ID_EX.ex_inst.func)
                {
                    //  add
                    case 0x20:

                        printf("\n OPERATION: add \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs  + ID_EX.ex_inst.rt;
                        printf(" %u = %u + %u \n", EX_MEM_shadow.alu_result,ID_EX.ex_inst.rs,ID_EX.ex_inst.rt );
                        break;

                    //  addu
                    case 0x21:

                        printf("\n OPERATION: addu \n");
                        EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs + (unsigned int)ID_EX.ex_inst.rt;
                        printf(" %u = %u + %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.rt );

                        break;

                    //  and
                    case 0x24:
                         printf("\n OPERATION: and \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs & ID_EX.ex_inst.rt;
                        printf(" %u = %u & %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // nor
                    case 0x27:
                         printf("\n OPERATION: nor \n");
                        EX_MEM_shadow.alu_result = !(ID_EX.ex_inst.rs | ID_EX.ex_inst.rt);
                        printf(" %u = !(%u | %u) \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // or
                    case 0x25:
                        printf("\n OPERATION: or \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs | ID_EX.ex_inst.rt ;
                        printf(" %u = %u | %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // sll
                    case 0:
                         printf("\n OPERATION: sll \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rt << ID_EX.ex_inst.shamt;
                         printf(" %u = %u << %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rt, ID_EX.ex_inst.shamt );
                        break;

                    // srl
                    case 2:
                         printf("\n OPERATION: srl \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rt >> ID_EX.ex_inst.shamt;
                        printf(" %u = %u >> %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rt, ID_EX.ex_inst.shamt );
                        break;

                    // sub
                    case 0x22:
                        printf("\n OPERATION: sub \n");
                        EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs - ID_EX.ex_inst.rt;
                        printf(" %u = %u - %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    //  subu
                    case 0x23:
                         printf("\n OPERATION: subu \n");
                        EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs - (unsigned int)ID_EX.ex_inst.rt;
                        printf(" %u = %u - %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.rt );
                        break;

                    // xor
                    case 0x26:
                        printf("\n OPERATION: xor \n");
                        EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs != ID_EX.ex_inst.rt);
                        printf(" %u = %u != %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    // slt
                    case 0x2a:
                         printf("\n OPERATION: slt \n");
                        EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < ID_EX.ex_inst.rt) ? 1 : 0 ;
                        printf(" %u = %u < %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_inst.rt );
                        break;

                    //  sltu
                    case 0x2b:
                         printf("\n OPERATION: sltu \n");
                        EX_MEM_shadow.alu_result = ((unsigned int)ID_EX.ex_inst.rs < (unsigned int)ID_EX.ex_inst.rt) ? 1 : 0 ;
                         printf(" %u = %u < %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.rt );
                        break;

                // end switch
                }break;

        // addi
        case 8:

             if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: addi \n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs + ID_EX.ex_signext;
            printf(" %u = %u + %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // addiu
        case 9:

             if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: addiu \n");
            EX_MEM_shadow.alu_result = (unsigned int)ID_EX.ex_inst.rs + (unsigned int)ID_EX.ex_inst.iImm;
            printf(" %u = %u + %u \n", EX_MEM_shadow.alu_result, (unsigned int)ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.iImm );
            break;

        //  andi
        case 0xc:

             if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: andi\n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs & ID_EX.ex_signext ;
             printf(" %u = %u & %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // ori
        case 0xd:

             if (forward_Rs_ex== true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: ori\n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs | ID_EX.ex_signext;
            printf(" %u = %u | %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // xori
        case 0xe:

             if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex== false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: xori\n");
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs != ID_EX.ex_signext);
            printf(" %u = %u != %u \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.rs, ID_EX.ex_signext );
            break;

        // lui
        case 0xf:
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: lui\n");
            printf("immediate value: %u\n", ID_EX.ex_inst.iImm);
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.iImm);
            EX_MEM_shadow.alu_result = ((EX_MEM_shadow.alu_result << 16) & 0xFFFF0000 );
            printf("alu result: %u = %u << 16  \n", EX_MEM_shadow.alu_result, ID_EX.ex_inst.iImm );
            break;

        // slti
        case 0xa:
             if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: slti\n");
            EX_MEM_shadow.alu_result = (ID_EX.ex_inst.rs < ID_EX.ex_signext) ? 1 : 0 ;
            printf("result = %u < %u \n",ID_EX.ex_inst.rs, ID_EX.ex_signext);
            break;

        // sltiu
        case 0xb:
             if (forward_Rs_ex == true)
                    {
                        ID_EX.ex_inst.rs = rd_to_rs;
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf("\n OPERATION: sltiu\n");
            EX_MEM_shadow.alu_result = ((unsigned int)ID_EX.ex_inst.rs < (unsigned int)ID_EX.ex_inst.iImm) ? 1 : 0 ;
            printf("result = %u < %u \n",ID_EX.ex_inst.rs, (unsigned int)ID_EX.ex_inst.iImm);
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
                        forward_Rs_ex == false;
                    }
            EX_MEM_shadow.RegWrite = true;
            printf(" \n load or store instruction\n");
            EX_MEM_shadow.alu_result = ID_EX.ex_inst.rs + ID_EX.ex_signext ;
            printf("\n\n address calculated: %d = %d + %d \n\n",EX_MEM_shadow.alu_result,ID_EX.ex_inst.rs,ID_EX.ex_signext);
            break;

    // opcode switch
        }

         EX_MEM_shadow.mem_inst = ID_EX.ex_inst;

    // Check if data forwarding is needed
    if( (EX_MEM_shadow.RegWrite == true ) && (EX_MEM_shadow.RegisterRd !=0) && (EX_MEM_shadow.RegisterRd == ID_EX.RegisterRs) )
    {
        forward_Rs_ex = true;
        rd_to_rs = EX_MEM_shadow.alu_result;
    }

    if( (EX_MEM_shadow.RegWrite == true) && (ID_EX.RegisterRd != 0) && (EX_MEM_shadow.RegisterRd == ID_EX.RegisterRt))
    {

    }

}

// Access memory operand
void MEM()
{
    // do nothing for R-format instructions (wait for WB() )

    MEM_WB_shadow.wb_pc = EX_MEM.mem_pc;
    MEM_WB_shadow.next_pc = EX_MEM.next_pc;
    MEM_WB_shadow.dest_rd = EX_MEM.dest_rd;
    MEM_WB_shadow.dest_rt = EX_MEM.dest_rt;

    unsigned int index = EX_MEM.alu_result;

    switch(EX_MEM.mem_inst.opcode)
    {
        //  lbu
        case 0x24:
            printf("Confirm Operation: lbu\n");
            MEM_WB_shadow.read_data = (memory[index] & 0x000000FF );
            printf("data read: %u\n",MEM_WB_shadow.read_data);
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
            printf("destination register: %u\n", MEM_WB_shadow.dest_reg);
            break;

        //  lhu
        case 0x25:
             printf("Confirm Operation: lhu\n");
            MEM_WB_shadow.read_data = (memory[index] & 0x0000FFFF );
            printf("data read: %u\n",MEM_WB_shadow.read_data);
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
            printf("destination register: %u\n", MEM_WB_shadow.dest_reg);
            break;

        //  lw
        case 0x23:
            printf("Confirm Operation: lw \n");
            MEM_WB_shadow.read_data = memory[index];
            printf("data read: %u\n",MEM_WB_shadow.read_data);
            MEM_WB_shadow.dest_reg = EX_MEM.mem_inst.rt;
            printf("destination register: %u\n", MEM_WB_shadow.dest_rt);
            break;

        // sb
        case 0x28:
            printf("Confirm Operation: sb\n");
            memory[index] = ( EX_MEM.mem_inst.rt & 0x000000FF );
            printf("Data Written : %u", memory[index]);
            break;

        // sh
        case 0x29:
            printf("Confirm Operation: sh\n");
            memory[index] = ( EX_MEM.mem_inst.rt & 0x0000FFFF );
            printf("Data Written : %u", memory[index]);
            break;

        // sw
        case 0x2b:
            printf("Confirm Operation: sw\n");
            memory[index] = EX_MEM.mem_inst.rt;
            printf("Data Written : %u", memory[index]);
            break ;
    }

    MEM_WB_shadow.wb_inst = EX_MEM.mem_inst;
    MEM_WB_shadow.dest_rd = EX_MEM.dest_rd;
    MEM_WB_shadow.dest_rt = EX_MEM.dest_rt;

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
                                printf("\n\n WB(): destination reg = %u   value written = %u\n\n",MEM_WB.dest_rd, MEM_WB.wb_alu_result );
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
                    printf("\n\n WB(): destination reg = %u   value written = %u\n\n",MEM_WB.dest_rt, MEM_WB.wb_alu_result );
                    break;

        /// all load write back to register rt (.dest_reg)
        //  lbu
        case 0x24:

        //  lhu
        case 0x25:

        //  lw
        case 0x23:
                  R[ MEM_WB.dest_rt] = MEM_WB.read_data;
                  printf("\n\n WB(): destination reg = %u   value written = %u\n\n",MEM_WB.dest_rt, MEM_WB.read_data);
                  break;
    }

}

void Move_Shadow_to_Pipeline()
{
    IF_ID = IF_ID_shadow;
    ID_EX = ID_EX_shadow;
    EX_MEM = EX_MEM_shadow;
    MEM_WB = MEM_WB_shadow;

}

void Execute_Clock_Cycle()
{
   IF();
   ID();
   EX();
   MEM();
   WB();
   count_down = count_down-1;
   Move_Shadow_to_Pipeline();
   if(branch == true)
   {
       $pc = hold_branch_address;
       branch = false;
   }
   else
       $pc = $pc+1;

   printf("\n\nPC At End = %d\n",$pc);

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


