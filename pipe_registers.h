 typedef struct INSTRUCTION{

   uint32_t opcode;
   int32_t rs;
   int32_t rt;
   int32_t rd;
   uint32_t shamt;
   uint32_t func;
   int16_t iImm;
   uint32_t jImm;
   bool Rform;
   bool Iform;
   bool Jform;

} instruction ;


/// Pipeline Registers
struct if_id
{
  unsigned int id_pc, next_pc;
  instruction id_inst;

} IF_ID_shadow, IF_ID;

struct id_ex
{
    unsigned int ex_pc, next_pc;
    int32_t ex_signext;
    instruction ex_inst;
    unsigned int dest_rd, dest_rt, dest_reg;
    unsigned int RegisterRs,RegisterRd,RegisterRt;
    bool memRead;

}ID_EX_shadow,ID_EX;

struct ex_mem
{
    int32_t alu_result;
    instruction mem_inst;
    unsigned int mem_pc, next_pc;
    unsigned int dest_rd, dest_rt;
    unsigned int RegisterRs,RegisterRd,RegisterRt;
    bool RegWrite;
    int32_t dest_reg;
    bool memRead;

} EX_MEM_shadow, EX_MEM;

struct mem_wb
{
    int32_t read_data, wb_alu_result, dest_reg;
    instruction wb_inst;
    unsigned int wb_pc, next_pc;
    unsigned int dest_rd, dest_rt;
    unsigned int RegisterRs,RegisterRd,RegisterRt;
    bool RegWrite;
    bool memRead;

} MEM_WB_shadow, MEM_WB;


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


int32_t sign_ext(int16_t value)
{
   int32_t new_immediate = value;

    return new_immediate;
}
