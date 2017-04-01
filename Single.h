// IF = Instruction Fetch

// Instruction Memory
uint32_t memory[];

// Program Counter Register
uint32_t * $pc;

// specifies ALU operation , decides next instruction
struct control_unit {

    bool RegWrite;
    bool ALUSrc;
    unsigned int ALUOp;
    bool MemWrite;
    bool MemtoReg;
    bool MemRead;
    bool Branch ;
    bool Zero ;
    bool PCSrc;
    bool RegDst;

} controlUnit;

struct instruction {

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

} currentInst;

void IF()
{
    // $pc reads memory address
    $pc = memory;

    ID( &currentInst, $pc);
}

// Instruction Decode and Reading Registers

// Register File
// R[0] always zero
// Can contain signed numbers in registers
int32_t R[31] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};

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
void ID(struct instruction *inst, uint32_t * pc)
{
    currentInst.opcode =  ( (*pc & opcode_mask) >> 26 );
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

// clo,clz,mul,madd,maddu,msub,msub  R
    case 0x1c   :

        currentInst.Rform = true;
        currentInst.shamt = ( (*pc & shamt_mask) >> 6);
        currentInst.func = *pc & func_mask;

          // read register files
        currentInst.rs = R[( (*pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*pc & rt_mask) >> 16)];
        currentInst.rd = R[( (*pc & rd_mask) >> 11)];

        // Set Control Lines
        controlUnit.RegWrite = 1;
        controlUnit.ALUSrc = 0;
        controlUnit.ALUOp = 10;
        controlUnit.MemWrite = 0;
        controlUnit.MemtoReg = 0;
        controlUnit.MemRead = 0;
        controlUnit.Branch = 0;
        controlUnit.Zero = 0;
        controlUnit.PCSrc = 0;
        controlUnit.RegDst = 0;

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
        currentInst.rs = R[( (*pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*pc & imm_mask_i));

        // Set Control Lines
        controlUnit.RegWrite = 1;
        controlUnit.ALUSrc = 1;
        controlUnit.ALUOp = 0;
        controlUnit.MemWrite = 0;
        controlUnit.MemtoReg = 0;
        controlUnit.MemRead = 0;
        controlUnit.Branch = 0;
        controlUnit.Zero = 0;
        controlUnit.PCSrc = 0;
        controlUnit.RegDst = 0;

        break;


// lb
    case 0x20   :

// lbu
    case 0x24   :

// lh
    case 0x21   :

// lhu
    case 0x25   :


// lw
    case 0x23   :

// lwcl, swcl
    case 0x31   :

// lwl
    case 0x22   :

// lwr
    case 0x26   :

// ll
    case 0x30   :

        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*pc & imm_mask_i));

        // Set Control Lines
        controlUnit.RegWrite = 1;
        controlUnit.ALUSrc = 1;
        controlUnit.ALUOp = 00;
        controlUnit.MemWrite = 0;
        controlUnit.MemtoReg = 1;
        controlUnit.MemRead = 1;
        controlUnit.Branch = 0;
        controlUnit.Zero = 0;
        controlUnit.PCSrc = 0;
        controlUnit.RegDst = 0;

        break;


//  sb
    case 0x28   :

//  sh
    case 0x29   :

//  sw
    case 0x2b   :

//  sdcl
    case 0x3d   :

//  swl
    case 0x2a   :

//  swr
    case 0x2e   :

//  sc
    case 0x38   :

        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*pc & imm_mask_i));

        // Set Control Lines
        controlUnit.RegWrite = 0;
        controlUnit.ALUSrc = 1;
        controlUnit.ALUOp = 00;
        controlUnit.MemWrite = 1;
        controlUnit.MemtoReg = 0;
        controlUnit.MemRead = 0;
        controlUnit.Branch = 0;
        controlUnit.Zero = 0;
        controlUnit.PCSrc = 0;
        controlUnit.RegDst = 0;

        break;

// beg  I
    case 0x4    :

// bgez, bgezal,bltzal,teqi,tgei,tgeiu,tlti,tltiu I
    case 0x1    :

// bgtz I
    case 0x7    :

// blez I
    case 0x6    :

// bne  I
    case 0x5    :
        currentInst.Iform = true;

        // read register file
        currentInst.rs = R[( (*pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*pc & rt_mask) >> 16)];

        currentInst.iImm = ( (*pc & imm_mask_i));

        // Set Control Lines
        controlUnit.RegWrite = 0;
        controlUnit.ALUSrc = 0;
        controlUnit.ALUOp = 01;
        controlUnit.MemWrite = 0;
        controlUnit.MemtoReg = 0;
        controlUnit.MemRead = 0;
        controlUnit.Branch = 1;
        controlUnit.Zero = 0;
        controlUnit.PCSrc = 0;
        controlUnit.RegDst = 0;


        break;

 /**************************     J-format Instructions ******************************************/

// j
    case 0x2    :

// jal  J
    case 0x3    :

        currentInst.Jform = true;
        currentInst.jImm = ( (*pc & imm_mask_j));

         // Set Control Lines
        controlUnit.RegWrite = 0;
        controlUnit.ALUSrc = 0;
        controlUnit.ALUOp = 0;
        controlUnit.MemWrite = 0;
        controlUnit.MemtoReg = 0;
        controlUnit.MemRead = 0;
        controlUnit.Branch = 0;
        controlUnit.Zero = 0;
        controlUnit.PCSrc = 0;
        controlUnit.RegDst = 0;

        break;




    }

    // increment pc for later
    $pc += 4;

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



}

// only occurs for I format ?
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


// global registers for ALU use
int32_t lo = 0;
int32_t hi = 0;

uint32_t lou = 0;
uint32_t hiu = 0;

int64_t result = 0;
uint64_t resultu = 0;
int64_t concat = 0;
uint64_t concatu = 0;

uint32_t ALU(struct instruction *inst )
{

    uint32_t math_result = 0;
    uint32_t mem_addr = 0;
    uint32_t branch_target = 0;

    uint32_t mem_index = 0;

    switch(inst->opcode) {

        case 0x0:

                switch(inst->func)
                {
                    //  add
                    case 0x20:
                        return math_result = inst->rs  + inst->rt;

                    //  addu
                    case 0x21:
                        return math_result = inst->rs + inst->rt;

                    //  and
                    case 0x24:
                        return math_result = inst->rs & inst->rt;

                    // div
                    case 0x1a:
                        lo = inst->rs / inst->rt;
                        hi = inst->rs % inst->rt;
                        return 0;

                    //  divu
                    case 0x1b:
                        lou = inst->rs / inst->rt;
                        hiu = inst->rs % inst->rt;
                        return 0;

                    // mult
                    case 0x18:
                        result = inst->rs * inst->rt;
                        lo = result;
                        hi = (result>>32);
                        return 1;

                    // multu
                    case 0x19:
                        resultu = inst->rs * inst->rt;
                        lou = resultu;
                        hiu = (resultu>>32);
                        return 1;

                    // nor
                    case 0x27:
                        return math_result = !(inst->rs | inst->rt);

                    // or
                    case 0x25:
                        return math_result = inst->rs | inst->rt ;

                    // sll
                    case 0:
                        return math_result = inst->rt << inst->shamt;

                    // sllv
                    case 4:
                        return math_result = inst->rt << inst->rs;

                    // sra
                    case 3:
                        return math_result = inst->rt >> inst->shamt;

                    // srav
                    case 7:
                        return math_result = inst->rt >> inst->rs;

                    // srl
                    case 2:
                        return math_result = inst->rt >> inst->shamt;

                    // srlv
                    case 6:
                        return math_result = inst->rt >> inst->rs;

                    // sub
                    case 0x22:
                        return math_result = inst->rs - inst->rt;

                    //  subu
                    case 0x23:
                        return math_result = inst->rs - inst->rt;

                    // xor
                    case 0x26:
                        return math_result = (inst->rs != inst->rt);

                    // slt
                    case 0x2a:
                        return math_result = (inst->rs < inst->rt) ? 1 : 0 ;

                    //  sltu
                    case 0x2b:
                        return math_result = (inst->rs < inst->rt) ? 1 : 0 ;

                // end function switch
                }
        // addi
        case 8:
            return math_result = inst->rs + inst->iImm;

        // addiu
        case 9:
            return math_result = inst->rs + inst->iImm;

        //  andi
        case 0xc:
            return math_result = inst->rs & inst->iImm ;

        case 0x1c:

            switch(inst->func)
            {
                //  clo
                case 0x21:
                    return math_result = __builtin_clz( ~(inst->rs) );

                //  clz
                case 0x20:
                    return math_result = __builtin_clz(inst->rs);

                // mul
                case 2:
                    return math_result = inst->rs * inst->rt;

                // madd
                case 0:
                    result = inst->rs * inst->rt;
                    concat = hi;
                    concat << 32;
                    concat = concat+lo;
                    result += concat;
                    return 2;

                // maddu
                case 1:
                    resultu = inst->rs * inst->rt;
                    concatu = hiu;
                    concatu << 32;
                    concatu = concatu+lou;
                    resultu += concatu;
                    return 3;

                // msub
                case 4:
                    result = inst->rs * inst->rt;
                    concat = hi;
                    concat << 32;
                    concat = concat+lo;
                    result -= concat;
                    return 4;

        // end opcode 0x1c switch
            }

        // ori
        case 0xd:
            return math_result = inst->rs | inst->iImm;

        // xori
        case 0xe:
            return math_result = (inst->rs != inst->iImm);

        // lui
        case 0xf:
            // ?????

        // slti
        case 0xa:
            return math_result = (inst->rs < inst->iImm) ? 1 : 0 ;

        // sltiu
        case 0xb:
            return math_result = (inst->rs < inst->rt) ? 1 : 0 ;

        // beq
        case 4:
            if( inst->rs == inst->rt)
            {
                // set control unit to branch
                controlUnit.Zero=1;
                return;
            }
            else
                return;

        case 1:

            switch(inst->rt)
                {

                //bltz
                case 0:
                       if( inst->rs < 0)
                            {
                              // set control unit to branch
                              controlUnit.Zero = 1;
                              return;
                            }
                        else
                           return;

                //bgez
                case 1:
                    if( inst->rs >= 0)
                       {
                        // set control unit to branch
                        controlUnit.Zero = 1;
                        return;
                       }
                    else
                        return;

                //bgezal
                case 0x11:
                    if( inst->rs >= 0)
                        {
                        // set control unit to branch
                        controlUnit.Zero = 1;
                        return;
                        }
                    else
                        return;

                //bltzal
                case 0x10:
                    if( R[currentInst.rs] < 0)
                        {
                        // set control unit to branch
                        controlUnit.Zero = 1;

                        // save address of next instruction
                        R[31] = $pc;

                        return;
                        }
                    else
                       return;

            // end op:1 switch
                }

        //  bgtz
        case 7:
               if( inst->rs > 0)
                   {
                     // set control unit to branch
                     controlUnit.Zero = 1;
                     return;
                   }
                else
                    return;
        //  blez
        case 6:
                  if( inst->rs <= 0)
                    {
                     // set control unit to branch
                     controlUnit.Zero = 1;
                     return;
                    }
                else
                    return;

        //  bne
        case 5:
            if( inst->rs != inst->rt )
                {
                 // set control unit to branch
                 controlUnit.Zero = 1;
                 return;
                }
            else
                return;

        //  lb
        case 0x20:

        //  lbu
        case 0x24:

        //  lh
        case 0x21:

        //  lhu
        case 0x25:

        //  lw
        case 0x23:
            return mem_addr = (sign_ext(inst->iImm) + inst->rs);

        //  lwl
        case 0x22:
            // ????

        //  lwr
        case 0x26:
            // ????

        //  ll
        case 0x30:
            // ????

        // sb
        case 0x28:

        // sh
        case 0x29:

        // sw
        case 0x2b:
            return mem_addr = inst->rs + sign_ext(inst->iImm) ;


  /* swl
        case 0x2a:
            // ????

        // swr
        case 0x2e:
            // ????

        // sc
        case 0x38:
            // ???? */

    // opcode switch
        }

}

// Data Memory
uint32_t data_mem[];


