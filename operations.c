Make common case fast:      for(int j=1000 ; j!= 0 ; j-_ )

lui:        for 32-bit constants        // copies 16 bit constant to left 16 bits of rt , clears right 16 bits of rt

array indexing:     4-->words   2-->half words  1--> bytes  add to base values  *// consider using pointers



// Arithmetic

add:    R[rd]= R[rs]+R[rt];             // R
addi:   R[rt]= R[rs]+SignExtImm;        // I
addiu:  R[rt]= R[rs]+SignExtImm;        // I
addu:   R[rd]= R[rs]+R[rt];             // R
and:    R[rd]= R[rs]&R[rt];             // R
andi:   R[rt]=R[rs]& ZeroExtImm;        // I

beq:    if( R[rs]==R[rt])
            PC=PC+4BranchAddr;          // I

bne:    if( R[rs]!=R[rt])
            PC=PC+4BranchAddr;          // I

j:      PC=JumpAddr;                    // J

jal:    R[31]=PC+8;
        PC=JumpAddr;                    // J

jr:     PC=R[rs];                       // R

lbu:    R[rt]= M[ R[rs]+ offset];       // I byte addressing --> incrementing by 1
lhu:    R[rt]=M[ R[rs]+ offset];        // I half word addressing --> increment by 2
lui:    R[rt]= imm* (2<<16);            // I ! Convert to C

sb:     M[R[rs]+SignExtImm](7:0)= R[rt](7:0);       //I byte addressing --> increment by 1
sh:     M[R[rs]+SignExtImm](15:0)= R[rt](15:0);     //I half word addressing -

ll:     R[rt]=M[R[rs]+SignExtImm];      // I
lw:     R[rt]=M[R[rs]+SignExtImm];      // I

nor:    R[rd]= ~( R[rs]| R[rt]);        // R
or:     R[rd]=  R[rs]| R[rt];           // R
ori:    R[rt]= R[rs] | ZeroExtImm;      // I

slt:    R[rd]=( R[rs]<R[rt] ) ? 1:0 ;          // R
slti:   R[rt]=( R[rs]< SignExtImm ) ? 1:0 ;    // I
sltiu:  R[rt]=( R[rs]< SignExtImm ) ? 1:0 ;    // I
sltu:   R[rd]=( R[rs]<R[rt] ) ? 1:0 ;          // R

sll:    R[rd]=R[rt]<<shamt;         //R
srl:    R[rd]=R[rt]>>shamt;         //R


sw:     M[R[rs]+SignExtImm]=R[rt];  //I
sub:    R[rd]=R[rs]-R[rt];          //R
subu:   R[rd]=R[rs]-R[rt];          //R
