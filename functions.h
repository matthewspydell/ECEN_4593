
void decode( )
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

// clo,clz,mul,madd,maddu,msub,msub  R
    case 0x1c   :
        currentInst.Rform = true;
        currentInst.rs = ( (*$pc & rs_mask) >> 21);
        currentInst.rt = ( (*$pc & rt_mask) >> 16);
        currentInst.rd = ( (*$pc & rd_mask) >> 11);
        currentInst.shamt = ( (*$pc & shamt_mask) >> 6);
        currentInst.func = ( (*$pc & func_mask));

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
        currentInst.rs = ( (*$pc & rs_mask) >> 21);
        currentInst.rt = ( (*$pc & rt_mask) >> 16);
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


default:    // exception for unrecognized instruction!


}


}
