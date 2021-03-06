/*
 * @author matthewspydell
 */

#include "memoryAccess.h"
#include "memAccess.h"

void memoryAccess() {
    if (printing) {
        printf("\nMemory Access Stage\n");
    }

    int temp;

    MEM_WB.opcodeShadow = EX_MEM.opcode; // move opcode to next pipeline register
    MEM_WB.memReadShadow = EX_MEM.memRead; // move memRead indicator to next pipeline register
    MEM_WB.moveControlShadow = EX_MEM.moveControl; // move moveControl to next pipeline register

    switch (EX_MEM.opcode) {
        case 0x20: // alu output from previous pipeline holds address location
            // load byte
            if (printing) {
                printf("Load Byte\n");
            }
            switch (EX_MEM.offset) {
                case 0:
                    MEM_WB.aluOutputShadow = (mainMemory[EX_MEM.aluOutput] & 0xFF000000) >> 24;
                    break;
                case 1:
                    MEM_WB.aluOutputShadow = (mainMemory[EX_MEM.aluOutput] & 0x00FF0000) >> 16;
                    break;
                case 2:
                    MEM_WB.aluOutputShadow = (mainMemory[EX_MEM.aluOutput] & 0x0000FF00) >> 8;
                    break;
                case 3:
                    MEM_WB.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0x000000FF;
            }
            MEM_WB.rdShadow = EX_MEM.rd;
            //memAccess(false, true, EX_MEM.aluOutput);
            if (printing) {
                printf("mainMemory[%d] = 0x%x = %d\nMEM_WB.rd = %d, value = 0x%x = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], mainMemory[EX_MEM.aluOutput], EX_MEM.rd, MEM_WB.aluOutputShadow, MEM_WB.aluOutputShadow);
            }
            break;
        case 0x21: // alu output from previous pipeline holds address location
            // load halfword (unsigned)
            if (printing) {
                printf("Loading Halfword\n");
            }
            switch (EX_MEM.offset) {
                case 0:
                    MEM_WB.aluOutputShadow = (mainMemory[EX_MEM.aluOutput] & 0xFFFF0000) >> 16;
                    break;
                case 1:
                    MEM_WB.aluOutputShadow = mainMemory[EX_MEM.aluOutput] & 0x0000FFFF;
            }
            MEM_WB.rdShadow = EX_MEM.rd;
            //memAccess(false, true, EX_MEM.aluOutput);
            if (printing) {
                printf("mainMemory[%d] = 0x%x =  %d\nMEM_WB.rd = %d, value = 0x%x = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], mainMemory[EX_MEM.aluOutput], EX_MEM.rd, MEM_WB.aluOutputShadow, MEM_WB.aluOutputShadow);
            }
            break;
        case 0x23: // alu output from previous pipeline holds address location
            // load word
            if (printing) {
                printf("Loading Word\n");
            }
            MEM_WB.aluOutputShadow = mainMemory[EX_MEM.aluOutput];
            MEM_WB.rdShadow = EX_MEM.rd;
            //memAccess(false, true, EX_MEM.aluOutput);
            if (printing) {
                printf("mainMemory[%d] = %d\nMEM_WB.rd = %d, value = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], EX_MEM.rd, MEM_WB.aluOutputShadow);
            }
            break;
        case 0x28: // store byte, this is the end of this type of instruction
            // store byte from register rd in memory
            if (printing) {
                printf("Storing Byte\n");
            }
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
            //memAccess(false, false, EX_MEM.aluOutput);
            if (printing) {
                printf("mainMemory[%d] = 0x%x = %d\nEX_MEM.rd = %d value = 0x%x = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], mainMemory[EX_MEM.aluOutput], EX_MEM.rd, EX_MEM.rdValue, EX_MEM.rdValue);
            }
            break;
        case 0x29: // store halfword, this is the end of this type of instruction
            // store halfword from register rd in memory
            if (printing) {
                printf("Storing Halford\n");
            }
            switch (EX_MEM.offset) {
                case 0:
                    temp = mainMemory[EX_MEM.aluOutput] & 0x0000FFFF;
                    mainMemory[EX_MEM.aluOutput] = temp | (EX_MEM.rdValue << 16);
                    break;
                case 1:
                    temp = mainMemory[EX_MEM.aluOutput] & 0xFFFF0000;
                    mainMemory[EX_MEM.aluOutput] = temp | EX_MEM.rdValue;
            }
            //memAccess(false, false, EX_MEM.aluOutput);
            if (printing) {
                printf("mainMemory[%d] = 0x%x = %d\nEX_MEM.rd = %d value = 0x%x = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], mainMemory[EX_MEM.aluOutput], EX_MEM.rd, EX_MEM.rdValue, EX_MEM.rdValue);
            }
            break;
        case 0x2B: // store word, this is the end of this type of instruction
            // store word from register rd in memory
            if (printing) {
                printf("Storing Word\n");
            }
            mainMemory[EX_MEM.aluOutput] = EX_MEM.rdValue;
            //memAccess(false, false, EX_MEM.aluOutput);
            if (printing) {
                printf("mainMemory[%d] = %d\nEX_MEM.rd = %d value = %d\n", EX_MEM.aluOutput, mainMemory[EX_MEM.aluOutput], EX_MEM.rd, EX_MEM.rdValue);
            }
            break;
        default:
            // if no memory access move rd, aluOutput to next pipeline register
            MEM_WB.rdShadow = EX_MEM.rd;
            MEM_WB.rdValueShadow = EX_MEM.rdValue;
            MEM_WB.aluOutputShadow = EX_MEM.aluOutput;
    }
}
