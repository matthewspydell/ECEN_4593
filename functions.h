
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

void ALU()
{
    int32_t lo = 0;
    int32_t hi = 0;

    uint32_t lou = 0;
    uint32_t hiu = 0;
    
    uint32_t math_result = 0;
    uint32_t mem_addr = 0;
    uint32_t branch_target = 0;


    //  add
    //  add with overflow
    //  opcode: 0   function: 0x20
    math_result = R[currentInst.rs]+R[currentInst.rt];

    //  addu
    //  addition without overflow
    //  opcode: 0   function: 0x21
    math_result = R[currentInst.rs]+R[currentInst.rt];

    //  addi
    //  add immediate with overflow
    //  opcode: 8
    math_result = R[currentInst.rs]+ currentInst.iImm;

    // addiu
    // add immediate without overflow
    // opcode:  9
    math_result = R[currentInst.rs]+ currentInst.iImm;

    //  and
    //  opcode: 0   function: 0x24
    math_result = R[currentInst.rs] & R[currentInst.rt];

    //  andi
    //  AND immediate
    //  opcode: 0xc
    math_result = R[currentInst.rs] & currentInst.iImm ;

    //  clo
    //  Count leading ones
    //  opcode: 0x1c    function: 0x21
    uint32_t temp = ~(R[currentInst.rs]);
    math_result = __builtin_clz(temp);

    //  clz
    //  count leading zeroes
    //  opcode: 0x1c    function: 0x20
    math_result = __builtin_clz(R[currentInst.rs]);

    // div
    // divide with overflow
    // opcode:  0   function: 0x1a
    lo = R[currentInst.rs]/R[currentInst.rt];
    hi = R[currentInst.rs] % R[currentInst.rt];

    //  divu
    //  divide without oveflow
    //  opcode: 0   function:0x1b
    lou = R[currentInst.rs]/R[currentInst.rt];
    hiu = R[currentInst.rs] % R[currentInst.rt];

    // mult
    // multiply signed
    //  opcode: 0   function:0x18
    int64_t result = R[currentInst.rs]*R[currentInst.rt];
    lo = result;
    hi = (result>>32);


    // multu
    // unsigned multiply
    //  opcode: 0   function:0x19
    uint64_t result = R[currentInst.rs]*R[currentInst.rt];
    lou = result;
    hiu = (result>>32);

    // mul
    // multiply without overflow
    //  opcode: 0x1c    function: 2
    math_result = R[currentInst.rs]*R[currentInst.rt];

    // madd
    // multiply add
    //  opcode: 0x1c    function: 0
    int64_t result = R[currentInst.rs]*R[currentInst.rt];
    int64_t concat = hi;
    concat << 32;
    concat = concat+lo;
    result += concat;

    // maddu
    // unsigned multiply add
    //  opcode: 0x1c    function: 1
    uint64_t result = R[currentInst.rs]*R[currentInst.rt];
    uint64_t concat = hiu;
    concat << 32;
    concat = concat+lou;
    result += concat;

    // msub
    // multiply subtract signed
    //  opcode: 0x1c    function: 4
    int64_t result = R[currentInst.rs]*R[currentInst.rt];
    int64_t concat = hi;
    concat << 32;
    concat = concat+lo;
    result -= concat;

    // nor
    // opcode:  0   function: 0x27
    math_result = !(R[currentInst.rs] | R[currentInst.rt]);

    // or
    // opcode:  0   function: 0x25
    math_result = R[currentInst.rs] | R[currentInst.rt] ;

    // ori
    // logical OR immediate
    // opcode:  0xd
    math_result = R[currentInst.rs] | currentInst.iImm;

    // sll
    // shift left logical
    //  opcode: 0   function: 0
    math_result = R[currentInst.rt] << currentInst.shamt;

    // sllv
    // shift left variable
    //  opcode: 0   function: 4
    math_result = R[currentInst.rt] << R[currentInst.rs];

    // sra
    // shift right arithmetic
    // opcode:  0   function: 3
    math_result = R[currentInst.rt] >> currentInst.shamt;

    // srav
    // shift right variable arithmetic
    //  opcode: 0   function: 7
    math_result = R[currentInst.rt] >> R[currentInst.rs];

    // srl
    // shift right logical
    //  opcode: 0   function: 2
    math_result = R[currentInst.rt] >> currentInst.shamt;

    // srlv
    // shift right logical variable
    //  opcode: 0   function: 6
    math_result = R[currentInst.rt] >> R[currentInst.rs];

    // sub
    // subtract with overflow
    //  opcode: 0   function: 0x22
    math_result = R[currentInst.rs]-R[currentInst.rt];

    //  subu
    //  subtract without overflow
    //  opcode: 0   function: 0x23
    math_result = R[currentInst.rs]-R[currentInst.rt];

    // xor
    // logical xor
    //  opcode: 0   function: 0x26
    math_result = (R[currentInst.rs] != R[currentInst.rt]);

    // xori
    // logical xor immediate
    //  opcode: 0xe
    math_result = (R[currentInst.rs] != currentInst.iImm);

    // lui
    // load upper immediate
    //  opcode: 0xf
    R[currentInst.rt] = currentInst.iImm ;
    R[currentInst.rt] = R[currentInst.rt] << 16;

    // slt
    // set less than
    //  opcode: 0   function: 0x2a
    R[currentInst.rd] = (R[currentInst.rs] < R[currentInst.rt]) ? 1 : 0 ;

    //  sltu
    //  set less than unsigned
    //  opcode: 0   function: 0x2b
    R[currentInst.rd] = (R[currentInst.rs] < R[currentInst.rt]) ? 1 : 0 ;

    // slti
    // set less than immediate
    //  opcode: 0xa
    R[currentInst.rd] = (R[currentInst.rs] < currentInst.iImm) ? 1 : 0 ;

    // sltiu
    // set less than unsigned immediate
    //  opcode: 0xb
    R[currentInst.rd] = (R[currentInst.rs] < R[currentInst.rt]) ? 1 : 0 ;

    // beq
    // branch on equal
    //  opcode: 4
    if( R[currentInst.rs] == R[currentInst.rt])
        $pc += 4;
        $pc += (currentInst.iImm << 2);
    else
        $pc += 4;

    //  bgez
    //  branch on greater than equal zero
    //  opcode: 1
    if( R[currentInst.rs] >= 0)
        $pc += 4;
        $pc += (currentInst.iImm << 2);
    else
        $pc += 4;

    //  bgezal
    //  branch on greater than equal zero and link
    //  if rs greater than or equal to 0. Save address of next instruction in register 31
    //  opcode: 1
    if( R[currentInst.rs] >= 0)
        $pc += 4;
        $pc += (currentInst.iImm << 2);
        R[31] = $pc+4;
    else
        $pc += 4;

    //  bgtz
    //  Branch on greater than zero
    //  opcode: 7
    if( R[currentInst.rs] > 0)
        $pc += 4;
        $pc += (currentInst.iImm << 2);
    else
        $pc += 4;

    //  blez
    //  Branch on less than equal zero
    //  opcode: 6
    if( R[currentInst.rs] <= 0)
        $pc += 4;
        $pc += (currentInst.iImm << 2);
    else
        $pc += 4;

    //  bltzal
    //  branch on less than and link
    //  opcode: 1
    if( R[currentInst.rs] < 0)
        $pc += 4;
        $pc += (currentInst.iImm << 2);
        R[31] = $pc+4;
    else
        $pc += 4;

    //  bltz
    //  branch on less than zero
    //  opcode: 1
    if( R[currentInst.rs] < 0)
        $pc += 4;
        $pc += (currentInst.iImm << 2);
    else
        $pc += 4;

    //  bne
    //  branch on not equal
    //  opcode: 5
    if( R[currentInst.rs] != R[currentInst.rt])
        $pc += 4;
        $pc += (currentInst.iImm << 2);
    else
        $pc += 4;

    //  j
    //  jump
    //  opcode: 2
    $pc = ($pc & 0xf0000000) | (currentInst.jImm << 2);

    //  jal
    //  jump and link
    //  opcode: 3
    $pc = ($pc & 0xf0000000) | (currentInst.jImm << 2);
    R[31] = $pc+4;

    //  jalr
    //  jump and link register
    //  opcode: 0   function: 9
    $pc = ($pc & 0xf0000000) | (currentInst.jImm << 2);
    R[currentInst.rd] = $pc+4;

    //  jr
    //  jump register
    //  opcode: 0   function:   8
    $pc = R[currentInst.rs];

    //  teq
    //  Trap if equal
    //  opcode: 0   function:   0x34


    //  teqi
    //  Trap if equal immediate
    //  opcode: 1

    //  teq
    //  Trap if not equal
    //  opcode: 0   function:   0x36

    //  teqi
    //  Trap if not equal immediate
    //  opcode: 1

    //  tge
    //  trap if greater equal
    //  opcode: 1   function:   0x30

    //  tgeu
    //  unsigned trap if greater equal
    //  opcode: 1   function:   0x31

    //  tgei
    //  trap if greater equal immediate
    //  opcode: 1

    // tgeiu
    //  unsigned trap if greater equal immediate
    //  opcode: 1

    //  tlt
    //  trap if less than
    //  opcode: 0   function:   0x32

    //  tltu
    //  unsigned trap if less than
    //  opcode: 0   function:   0x33

    // tlti
    // trap if less than immediate
    //  opcode: 1

    //  tltiu
    //  unsigned trap if less than immediate
    //  opcode:1

    //  lb
    //  load byte
    //  opcode: 0x20
    R[currentInst.rt] = sign_ext((memory[ R[currentInst.rs] + currentInst.iImm]) & 0x000000ff);
    $pc = $pc+4;

    //  lbu
    //  load unsigned byte
    //  opcode: 0x24
    R[currentInst.rt] = sign_ext((memory[ R[currentInst.rs] + currentInst.iImm]) & 0x000000ff);


    //  lh
    //  load halfword
    //  opcode: 0x21
    R[currentInst.rt] = sign_ext((memory[ R[currentInst.rs] + currentInst.iImm]) & 0x0000ffff);


    //  lhu
    //  load unsigned halfword
    //  opcode: 0x25
    R[currentInst.rt] = sign_ext((memory[ R[currentInst.rs] + currentInst.iImm]) & 0x0000ffff);


    //  lw
    //  load word
    //  opcode: 0x23
    R[currentInst.rt] = (memory[ R[currentInst.rs] + currentInst.iImm]) ;


    //  lwl
    //  load word left
    //  opcode: 0x22


    //  lwr
    //  load word right
    //  opcode: 0x26

    //  ll
    //  load linked
    //  opcode: 0x30

    // sb
    // store byte
    // opcode: 0x28
    uint32_t low_byte = (0xff & R[currentInst.rt]);
    uint32_t mem_index = R[currentInst.rs] + currentInst.iImm ;
    // whole instruction:: memory[R[currentInst.rs] + currentInst.iImm] = (0xff & R[currentInst.rt]);

    // sh
    // store halfword
    // opcode: 0x29
    uint32_t low_half = (0xffff & R[currentInst.rt]);
    uint32_t mem_index = R[currentInst.rs] + currentInst.iImm ;
     // whole instruction:: memory[R[currentInst.rs] + currentInst.iImm] = (0xffff & R[currentInst.rt]);

    // sw
    // store word
    // opcode: 0x2b
    // whole instruction: memory[R[currentInst.rs] + currentInst.iImm] = R[currentInst.rt];


    // swl
    // store word left
    // opcode: 0x2a

    // swr
    // store word right
    // opcode: 0x2e

    // sc
    // store conditional
    // opcode 0x38




}


