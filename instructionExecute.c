/*
 * @author matthewspydell
 */

#include "instructionExecute.h"

void instructionExecute() {
    EX_MEM.rdShadow = ID_EX.rd; // move rd to next pipeline register
    ID_EX.rdValue = R[ID_EX.rd]; // pull value from rd
    EX_MEM.rdValueShadow = ID_EX.rdValue; // move rdValue to next pipeline register
    ID_EX.rsValue = R[ID_EX.rs]; // pull value from rs
    ID_EX.rtValue = R[ID_EX.rt]; // pull value from rt
    EX_MEM.opcodeShadow = ID_EX.opcode; // move opcode to next pipeline register
    EX_MEM.memReadShadow = ID_EX.memRead; // move memRead indicator to next pipeline register

    if (printing) {
        printf("\nExecute Stage\n");
    }

    if ((ID_EX.opcode == 0) || (ID_EX.opcode == 0x1F)) { ///////////// R-format
        if (printing) {
            printf("rs = %d, R[rs] = %d\n", ID_EX.rs, R[ID_EX.rs]);
            printf("rt = %d, R[rt] = %d\n", ID_EX.rt, R[ID_EX.rt]);
            printf("rd = %d, R[rd] = %d\n", ID_EX.rd, R[ID_EX.rd]);
            printf("shamt = %d\n", ID_EX.shamt);
            printf("funct = 0x%x\n", ID_EX.funct);
        }

        /////////// hazard protection and forwarding ////////////
        if ((ID_EX.rs == EX_MEM.rd) || (ID_EX.rs == MEM_WB.rd) || (ID_EX.rt == EX_MEM.rd) || (ID_EX.rt == MEM_WB.rd)) {
            if ((ID_EX.rs == EX_MEM.rd) && (EX_MEM.moveControl != 0)) {
                ID_EX.rsValue = EX_MEM.aluOutput;
                if (printing) {
                    printf("Forwarded EX_MEM.aluOutput = %d to ID_EX.rs\n", EX_MEM.aluOutput);
                }
            }
            if ((ID_EX.rs == MEM_WB.rd) && (MEM_WB.moveControl != 0)) {
                ID_EX.rsValue = MEM_WB.aluOutput;
                if (printing) {
                    printf("Forwarded MEM_WB.aluOutput = %d to ID_EX.rs\n", MEM_WB.aluOutput);
                }
            }
            if ((ID_EX.rt == EX_MEM.rd) && (EX_MEM.moveControl != 0)) {
                ID_EX.rtValue = EX_MEM.aluOutput;
                if (printing) {
                    printf("Forwarded EX_MEM.aluOutput = %d to ID_EX.rt\n", EX_MEM.aluOutput);
                }
            }
            if ((ID_EX.rt == MEM_WB.rd) && (MEM_WB.moveControl != 0)) {
                ID_EX.rtValue = MEM_WB.aluOutput;
                if (printing) {
                    printf("Forwarded MEM_WB.aluOutput = %d to ID_EX.rt\n", MEM_WB.aluOutput);
                }
            }

        }

        if (ID_EX.opcode == 0x1F) { ///////// Special execution ////////////
            // (seb) sign-extend byte in rt and move to rd
            EX_MEM.aluOutputShadow = (signed)ID_EX.rtValue;
            EX_MEM.moveControlShadow = 1;
            if (printing) {
                printf("Sign-Extended Byte Instruction\n");
            }
        } else {
            ///////// R-format execution ////////////
            switch (ID_EX.funct) {
                case 0x20: // add, R[rd] = R[rs] + R[rt] (signed)
                    EX_MEM.aluOutputShadow = ID_EX.rsValue + ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Add Instruction\n");
                    }
                    break;
                case 0x21: // add, R[rd] = R[rs] + R[rt] (unsigned)
                    EX_MEM.aluOutputShadow = ID_EX.rsValue + ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Add Unsigned Instruction\n");
                    }
                    break;
                case 0x24: // and, R[rd] = R[rs] & R[rt]
                    EX_MEM.aluOutputShadow = ID_EX.rsValue & ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("And Instruction\n");
                    }
                    break;
                case 0x26: // xor, R[rd] = R[rs] ^ R[rt]
                    EX_MEM.aluOutputShadow = ID_EX.rsValue ^ ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Xor Instruction\n");
                    }
                    break;
                case 0x27: // nor, R[rd] = ~(R[rs] | R[rt])
                    EX_MEM.aluOutputShadow = ~(ID_EX.rsValue | ID_EX.rtValue);
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Nor Instruction\n");
                    }
                    break;
                case 0x25: // or,  R[rd] = R[rs] | R[rt]
                    EX_MEM.aluOutputShadow = ID_EX.rsValue | ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Or Instruction\n");
                    }
                    break;
                case 0x2a: // set less than, R[rd] = (R[rs] < R[rt]) ? 1:0 (signed)
                    EX_MEM.aluOutputShadow = (ID_EX.rsValue < ID_EX.rtValue) ? 1 : 0;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Set Less Than Instruction\n");
                    }
                    break;
                case 0x2b: // set less than, R[rd] = (R[rs] < R[rt]) ? 1:0 (unsigned)
                    EX_MEM.aluOutputShadow = ((unsigned) ID_EX.rsValue < (unsigned) ID_EX.rtValue) ? 1 : 0;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Set Less Than Unsigned Instruction\n");
                    }
                    break;
                case 0x00: // shift left logical, R[rd] = R[rt] << shamt
                    EX_MEM.aluOutputShadow = ID_EX.rtValue << ID_EX.shamt;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Shift Left Logical Instruction\n");
                    }
                    break;
                case 0x02: // shift right logical, R[rd] = R[rt] >> shamt
                    EX_MEM.aluOutputShadow = ID_EX.rtValue >> ID_EX.shamt;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Shift Right Logical Instruction\n");
                    }
                    break;
                case 0x22: // subtract, R[rd] = R[rs] - R[rt] (signed)
                    EX_MEM.aluOutputShadow = ID_EX.rsValue - ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Subtract Instruction\n");
                    }
                    break;
                case 0x23: // subtract, R[rd] = R[rs] - R[rt] (unsigned)
                    EX_MEM.aluOutputShadow = ID_EX.rsValue - ID_EX.rtValue;
                    EX_MEM.moveControlShadow = 1;
                    if (printing) {
                        printf("Subtract Unsigned Instruction\n");
                    }
                    break;
                case 0x0A: // move rs to rd if rt is zero, 
                    EX_MEM.aluOutputShadow = ID_EX.rsValue;
                    EX_MEM.moveControlShadow = (ID_EX.rtValue == 0) ? 1 : 0;
                    if (printing) {
                        printf("Move on Zero\n");
                    }
                    break;
                case 0x0B: // move rs to rd if rt is not zero, 
                    EX_MEM.aluOutputShadow = ID_EX.rsValue;
                    EX_MEM.moveControlShadow = (ID_EX.rtValue != 0) ? 1 : 0;
                    if (printing) {
                        printf("Move on not Zero\n");
                    }
            }
        }

    }/*    Jump instruction is performed in decode stage
        else if (ID_EX.opcodeShadow == 0x2 || ID_EX.opcodeShadow == 0x3) {
          ///////// J-format execution ////////////
          switch (ID_EX.opcode) {
            case 0x02:  // PC = JumpAddress
              $pc = ID_EX.imm; // jump to address pointed to by immediate value
              break;
            case 0x03:  // R[31] = PC + 8; PC = JumpAddress
              R[31] = ($pc + 1) << 2; // byte align before adding to register
              $pc = ID_EX.imm; // jump to address pointed to by immediate value
              break;
          }
        }*/ else {
        if (printing) {
            printf("rs = %d, R[rs] = %d\n", ID_EX.rs, ID_EX.rsValue);
            printf("rd = %d, R[rd] = %d\n", ID_EX.rd, ID_EX.rdValue);
            printf("imm = %d\n", ID_EX.imm);
        }

        /////////// hazard protection and forwarding ////////////
        if ((ID_EX.rs == EX_MEM.rd) || (ID_EX.rs == MEM_WB.rd)) {
            if ((ID_EX.rs == EX_MEM.rd) && (EX_MEM.moveControl != 0)) {
                ID_EX.rsValue = EX_MEM.aluOutput;
                if (printing) {
                    printf("Forwarded EX_MEM.aluOutput = %d to ID_EX.rs\n", EX_MEM.aluOutput);
                }
            }
            if ((ID_EX.rs == MEM_WB.rd) && (MEM_WB.moveControl != 0)) {
                ID_EX.rsValue = MEM_WB.aluOutput;
                if (printing) {
                    printf("Forwarded MEM_WB.aluOutput = %d to ID_EX.rs\n", MEM_WB.aluOutput);
                }
            }

        }

        ///////// I-format execution ////////////
        switch (ID_EX.opcode) {
            case 0x08: // add immediate, R[rt] = R[rs] + imm (signed)
                EX_MEM.aluOutputShadow = ID_EX.rsValue + ID_EX.imm;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Add Immediate Instruction\n");
                }
                break;
            case 0x09: // add immediate, R[rt] = R[rs] + imm (unsigned)
                EX_MEM.aluOutputShadow = ID_EX.rsValue + ID_EX.imm;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Add Immediate Unsigned Instruction\n");
                }
                break;
            case 0x0C: // and immediate, R[rt] = R[rs] & imm (zero extended)
                EX_MEM.aluOutputShadow = ID_EX.rsValue & ID_EX.imm;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("And Immediate Instruction\n");
                }
                break;
            case 0x20: // load byte, R[rt] = MEM8(R[rs] + imm)
                EX_MEM.aluOutputShadow = (ID_EX.rsValue + ID_EX.imm) >> 2;
                EX_MEM.offsetShadow = (ID_EX.rsValue + ID_EX.imm) % 4;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Load Byte Instruction\n");
                }
                break;
            case 0x21: // load halfword, R[rt] = MEM16(R[rs] + imm)
                EX_MEM.aluOutputShadow = (ID_EX.rsValue + ID_EX.imm) >> 2;
                EX_MEM.offsetShadow = (ID_EX.rsValue + ID_EX.imm) % 2;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Load Halfword Instruction\n");
                }
                break;
            case 0x23: // load word, R[rt] = MEM16(R[rs] + imm)
                EX_MEM.aluOutputShadow = (ID_EX.rsValue + ID_EX.imm) >> 2;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Load Word Instruction\n");
                }
                break;
            case 0x0D: // or immediate, R[rt] = R[rs] | imm (zero extended)
                EX_MEM.aluOutputShadow = ID_EX.rsValue | ID_EX.imm;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Or Immediate Instruction\n");
                }
                break;
            case 0x0A: // set less than, R[rt] = (R[rs] < imm) ? 1:0 (signed)
                EX_MEM.aluOutputShadow = (ID_EX.rsValue < ID_EX.imm) ? 1 : 0;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Set Less Than Instruction\n");
                }
                break;
            case 0x0B: // set less than, R[rt] = (R[rs] < imm) ? 1:0 (unsigned)
                EX_MEM.aluOutputShadow = ((unsigned) ID_EX.rsValue < (unsigned) ID_EX.imm) ? 1 : 0;
                EX_MEM.moveControlShadow = 1;
                if (printing) {
                    printf("Set Less Than Unsigned Instruction\n");
                }
                break;
            case 0x28: // store byte, MEM8(R[rs] + imm) = R[rt]
                EX_MEM.aluOutputShadow = (ID_EX.rsValue + ID_EX.imm) >> 2;
                EX_MEM.offsetShadow = (ID_EX.rsValue + ID_EX.imm) % 4;
                EX_MEM.moveControlShadow = 0;
                if (printing) {
                    printf("Store Byte Instruction\n");
                }
                break;
            case 0x29: // store halfword, MEM16(R[rs] + imm) = R[rt]
                EX_MEM.aluOutputShadow = (ID_EX.rsValue + ID_EX.imm) >> 2;
                EX_MEM.offsetShadow = (ID_EX.rsValue + ID_EX.imm) % 2;
                EX_MEM.moveControlShadow = 0;
                if (printing) {
                    printf("Store Halfword Instruction\n");
                }
                break;
            case 0x2B: // store word, MEM32(R[rs] + imm) = R[rt]
                EX_MEM.aluOutputShadow = (ID_EX.rsValue + ID_EX.imm) >> 2;
                EX_MEM.moveControlShadow = 0;
                if (printing) {
                    printf("Store Word Instruction\n");
                }
                break;
        }
    }
    if (printing) {
        printf("ALU Output = %d\n", EX_MEM.aluOutputShadow);
    }
}
