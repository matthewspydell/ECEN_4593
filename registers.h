// Register File Mappings

// 32 Registers, each 32 bits wide
uint32_t R[31] = {0};

#define $zero R[0]

// Values for Func Results
#define $v0 R[2]
#define $v1 R[3]

// Arguments
#define $a0 R[4]
#define $a1 R[5]
#define $a2 R[6]
#define $a3 R[7]

// Temporaries
#define $t0 R[8]
#define $t1 R[9]
#define $t2 R[10]
#define $t3 R[11]
#define $t4 R[12]
#define $t5 R[13]
#define $t6 R[14]
#define $t7 R[15]
#define $t8 R[24]
#define $t9 R[25]

// Saved Temporaries
#define $s0 R[16]
#define $s1 R[17]
#define $s2 R[18]
#define $s3 R[19]
#define $s4 R[20]
#define $s5 R[21]
#define $s6 R[22]
#define $s7 R[23]

// Global Pointer
#define $gp R[28]

// Stack Pointer
#define $sp R[29]

// Frame Pointer
#define $fp R[30]

// Return Address
#define $ra R[31]
