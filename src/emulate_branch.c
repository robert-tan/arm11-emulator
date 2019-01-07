#include "emulate_branch.h"

static u_int32_t get_branch_offset(u_int32_t instr) {
  int32_t offset = (instr & BRC_OFFSET_M) << BRC_SHIFT_M;
  if ((BRC_SIGN_M & offset) > 0) {
    offset += BRC_FLIP_SIGN_M;
  }
  return (u_int32_t) offset;
}

void decode_branch(u_int32_t instr, DecodedInstruction* decoded) {
  decoded->instr_type = TYPE_BRNCH;
  decoded->cond = get_condition(instr);
  decoded->branch.offset = get_branch_offset(instr);
}

void execute_branch(State state, DecodedInstruction* instr) {
  *&state.registers[REG_PC] += instr->branch.offset;
}
