/*
 * @author matthewspydell
 */

#include "memoryAccess.h"

void memoryAccess() {
    if (printing) {
        printf("\nMemory Access Stage\n");
    }

    int temp;

    MEM_WB.opcodeShadow = EX_MEM.opcode; // move opcode to next pipeline register
    MEM_WB.memReadShadow = EX_MEM.memRead; // move memRead indicator to next pipeline register
    MEM_WB.moveControlShadow = EX_MEM.moveControl; // move moveControl to next pipeline register

    switch (EX_MEM.opcode) {
        case 0x24: // alu output from previous pipeline holds address location
            // load byte (unsigned)
            switch (EX_MEM.offset) {
                case 0:
                    EX_MEM.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0xFF000000;
                    break;
                case 1:
                    EX_MEM.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0x00FF0000;
                    break;
                case 2:
                    EX_MEM.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0x0000FF00;
                    break;
                case 3:
                    EX_MEM.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0x000000FF;
            }
            MEM_WB.rdShadow = EX_MEM.rd;
            break;
        case 0x25: // alu output from previous pipeline holds address location
            // load halfword (unsigned)
            switch (EX_MEM.offset) {
                case 0:
                    MEM_WB.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0x0000FFFF;
                    break;
                case 1:
                    MEM_WB.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0xFFFF0000;
            }
            MEM_WB.rdShadow = EX_MEM.rd;
            break;
        case 0x23: // alu output from previous pipeline holds address location
            // load word
            MEM_WB.aluOutputShadow = mainMemory[EX_MEM.aluOutput];
            MEM_WB.rdShadow = EX_MEM.rd;
            if (printing) {
                printf("Loading Word\nmainMemory[%d] = %d\nMEM_WB.rd = %d, value = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], EX_MEM.rd, MEM_WB.aluOutputShadow);
            }
            break;
        case 0x28: // store byte, this is the end of this type of instruction
            // store byte from register rd in memory
            switch (EX_MEM.offset) {
                case 0:
                    temp = mainMemory[EX_MEM.aluOutput] & 0x00FFFFFF;
                    mainMemory[EX_MEM.aluOutput] = temp | (EX_MEM.rdValue << 24);
                    break;
                case 1:
                    temp = mainMemory[EX_MEM.aluOutput] & 0xFF00FFFF;
                    mainMemory[EX_MEM.aluOutput] = temp | (EX_MEM.rdValue << 16);
                    break;
                case 2:
                    temp = mainMemory[EX_MEM.aluOutput] & 0xFFFF00FF;
                    mainMemory[EX_MEM.aluOutput] = temp | (EX_MEM.rdValue << 8);
                    break;
                case 3:
                    temp = mainMemory[EX_MEM.aluOutput] & 0xFFFFFF00;
                    mainMemory[EX_MEM.aluOutput] = temp | EX_MEM.rdValue;
            }
            break;
        case 0x29: // store halfword, this is the end of this type of instruction
            // store halfword from register rd in memory
            switch (EX_MEM.offset) {
                case 0:
                    temp = mainMemory[EX_MEM.aluOutput] & 0x0000FFFF;
                    mainMemory[EX_MEM.aluOutput] = temp | (EX_MEM.rdValue << 16);
                    break;
                case 1:
                    temp = mainMemory[EX_MEM.aluOutput] & 0xFFFF0000;
                    mainMemory[EX_MEM.aluOutput] = temp | EX_MEM.rdValue;
            }
            break;
        case 0x2B: // store word, this is the end of this type of instruction
            // store word from register rd in memory
            mainMemory[EX_MEM.aluOutput] = EX_MEM.rdValue;
            if (printing) {
                printf("Storing Word\nmainMemory[%d] = %d\nEX_MEM.rd = %d value = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], EX_MEM.rd, EX_MEM.rdValue);
            }
            break;
        default:
            // if no memory access move rd, aluOutput to next pipeline register
            MEM_WB.rdShadow = EX_MEM.rd;
            MEM_WB.rdValueShadow = EX_MEM.rdValue;
            MEM_WB.aluOutputShadow = EX_MEM.aluOutput;
    }
}
