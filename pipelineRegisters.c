/* @author matthewspydell
 * This file creates all the various structures that will represent the
 * pipeline registers IF/ID, ID/EX, EX/MEM, and MEM/WB
 */

/* instruction-fetch | instruction-decode register */
struct IF_ID {
    uint32_t instructionShadow;
    uint32_t instruction;
};

/* instruction-decode | execute register */
struct ID_EX {
    uint32_t opcodeShadow;
    uint32_t rsShadow;
    uint32_t rtShadow;
    uint32_t rdShadow;
    uint32_t shamtShadow;
    uint32_t functShadow;
    uint32_t immShadow;
    uint32_t opcode;
    uint32_t rs;
    uint32_t rt;
    uint32_t rd;
    uint32_t shamt;
    uint32_t funct;
    uint32_t imm;
};

/* execute | memory register */
struct EX_MEM {
    uint32_t rdShadow;
    uint32_t aluOutputShadow;
    uint32_t addressShadow;
    uint32_t rd;
    uint32_t aluOutput;
    uint32_t address;
};

/* memory | write-back register */
struct MEM_WB {
    uint32_t rdShadow;
    uint32_t aluOutputShadow;
    uint32_t addressOutputShadow;
    uint32_t rd;
    uint32_t aluOutput;
    uint32_t addressOut;
};
