// IF = Instruction Fetch

// Instruction Memory
uint32_t memory[];


uint32_t * $pc;

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

    // increment pc for later
    $pc += 4;

    ID(&currentInst, $pc);
}

// Instruction Decode and Reading Registers

// Register File
// R[0] always zero
// Can contain signed numbers in registers
int32_t R[31] = {0};

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

          // read register files
        currentInst.rs = R[( (*pc & rs_mask) >> 21)];
        currentInst.rt = R[( (*pc & rt_mask) >> 16)];
        currentInst.rd = R[( (*pc & rd_mask) >> 11)];

        currentInst.shamt = ( (*pc & shamt_mask) >> 6);
        currentInst.func = ( (*pc & func_mask));

        break;

/**************************     I-format Instructions ******************************************/

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

        break;

 /**************************     J-format Instructions ******************************************/

// j
    case 0x2    :

// jal  J
    case 0x3    :

        currentInst.Jform = true;
        currentInst.jImm = ( (*pc & imm_mask_j));

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
}





