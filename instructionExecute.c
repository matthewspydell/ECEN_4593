/* @author matthewspydell
 *
 */

void instructionExecute() {
  EX_MEM.rdShadow = ID_EX.rd; // move rd to next pipeline register
  EX_MEM.opcodeShadow = ID_EX.opcode; // move opcode to next pipeline register
  EX_MEM.memReadShadow = ID_EX.memRead; // move memRead indicator to next pipeline register

  if (ID_EX.opcode == 0) {  ///////////// R-format

    /////////// hazard protection and forwarding ////////////
    if (ID_EX.rs == EX_MEM.rd || ID_EX.rs == MEM_WB.rd || ID_EX.rt == EX_MEM.rd || ID_EX.rt == MEM_WB.rd) {
      if (ID_EX.rs == EX_MEM.rd) {
        R[ID_EX.rs] = EX_MEM.aluOutput;
      } else if (ID_EX.rs == MEM_WB.rd) {
        R[ID_EX.rs] = MEM_WB.aluOutput;
      } else if (ID_EX.rt == EX_MEM.rd) {
        R[ID_EX.rt] = EX_MEM.aluOutput;
      } else {
        R[ID_EX.rt] = MEM_WB.aluOutput;
      }
    }

    ///////// R-format execution ////////////
    switch (ID_EX.funct) {
      case 0x20:  // add, R[rd] = R[rs] + R[rt] (signed)
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)R[ID_EX.rt]);
        break;
      case 0x21:  // add, R[rd] = R[rs] + R[rt] (unsigned)
        EX_MEM.aluOutputShadow = R[ID_EX.rs] + R[ID_EX.rt];
        break;
      case 0x24:  // and, R[rd] = R[rs] & R[rt]
        EX_MEM.aluOutputShadow = R[ID_EX.rs] & R[ID_EX.rt];
        break;
      case 0x08:  // jump reg, $pc = R[rs]
        $pc = R[ID_EX.rs >> 2]; // change from byte aligned to word aligned
        break;
      case 0x27:  // nor, R[rd] = ~(R[rs] | R[rt])
        EX_MEM.aluOutputShadow = ~(R[ID_EX.rs] | R[ID_EX.rt]);
        break;
      case 0x25:  // or,  R[rd] = R[rs] | R[rt]
        EX_MEM.aluOutputShadow = R[ID_EX.rs] | R[ID_EX.rt];
        break;
      case 0x2a:  // set less than, R[rd] = (R[rs] < R[rt]) ? 1:0 (signed)
        EX_MEM.aluOutputShadow = (uint32_t) (((int)R[ID_EX.rs] < (int)R[ID_EX.rt]) ? 1:0);
        break;
      case 0x2b:  // set less than, R[rd] = (R[rs] < R[rt]) ? 1:0 (unsigned)
        EX_MEM.aluOutputShadow = (uint32_t) ((R[ID_EX.rs] < R[ID_EX.rt]) ? 1:0);
        break;
      case 0x00:  // shift left logical, R[rd] = R[rt] << shamt
        EX_MEM.aluOutputShadow = R[ID_EX.rt] << ID_EX.shamt;
        break;
      case 0x02:  // shift right logical, R[rd] = R[rt] >> shamt
        EX_MEM.aluOutputShadow = R[ID_EX.rt] >> ID_EX.shamt;
        break;
      case 0x22:  // subtract, R[rd] = R[rs] - R[rt] (signed)
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] - (int)R[ID_EX.rt]);
        break;
      case 0x23:  // subtract, R[rd] = R[rs] - R[rt] (unsigned)
        EX_MEM.aluOutputShadow = R[ID_EX.rs] - R[ID_EX.rt];
        break;
    }
  } else if (ID_EX.opcodeShadow == 0x2 || ID_EX.opcodeShadow == 0x3) {
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
  } else {
      /////////// hazard protection and forwarding ////////////
      if (ID_EX.rs == EX_MEM.rd || ID_EX.rs == MEM_WB.rd) {
        if (ID_EX.rs == EX_MEM.rd) {
          R[ID_EX.rs] = EX_MEM.aluOutput;
        } else {
          R[ID_EX.rs] = MEM_WB.aluOutput;
        }
      }

      ///////// I-format execution ////////////
      switch (ID_EX.opcode) {
      case 0x08:  // add immediate, R[rt] = R[rs] + imm (signed)
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
      case 0x09:  // add immediate, R[rt] = R[rs] + imm (unsigned)
        EX_MEM.aluOutputShadow = R[ID_EX.rs] + ID_EX.imm);
        break;
      case 0x0C:  // and immediate, R[rt] = R[rs] & imm (zero extended)
        EX_MEM.aluOutputShadow = R[ID_EX.rs] & ID_EX.imm;
        break;
      case 0x24:  // load byte, R[rt] = MEM8(R[rs] + imm) (unsigned)
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
      case 0x25:  // load halfword, R[rt] = MEM16(R[rs] + imm) (unsigned)
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
      case 0x23:  // load word, R[rt] = MEM16(R[rs] + imm)
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
      case 0x0D:  // or immediate, R[rt] = R[rs] | imm (zero extended)
        EX_MEM.aluOutputShadow = R[ID_EX.rs] | ID_EX.imm;
        break;
      case 0x0A:  // set less than, R[rt] = (R[rs] < imm) ? 1:0 (signed)
        EX_MEM.aluOutputShadow = (uint32_t) (((int)R[ID_EX.rs] < (int)imm) ? 1:0);
        break;
      case 0x0B:  // set less than, R[rt] = (R[rs] < imm) ? 1:0 (unsigned)
        EX_MEM.aluOutputShadow = (R[ID_EX.rs] < ID_EX.imm) ? 1:0;
        break;
      case 0x28:  // store byte, MEM8(R[rs] + imm) = R[rt]
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
      case 0x29:  // store halfword, MEM16(R[rs] + imm) = R[rt]
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
      case 0x2B:  // store word, MEM32(R[rs] + imm) = R[rt]
        EX_MEM.aluOutputShadow = (uint32_t) ((int)R[ID_EX.rs] + (int)ID_EX.imm);
        break;
    }
  }
}
