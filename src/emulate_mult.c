#include "emulate_mult.h"

void decode_mult(u_int32_t instr, DecodedInstruction* decoded) {
  decoded->instr_type = TYPE_MULT;
  decoded->cond = get_condition(instr);
  decoded->multiply.set_cond = (u_int8_t) get_bits(instr, BIT_SIZE, ALL_SET_COND_LSB);
  decoded->multiply.acc = (u_int8_t) get_bits(instr, BIT_SIZE, MUL_A_LSB);
  if (decoded->multiply.acc) {
    decoded->multiply.rn = (u_int8_t) get_bits(instr, REG_PTR_SIZE, MUL_RN_LSB);
  }
  decoded->multiply.rd = (u_int8_t) get_bits(instr, REG_PTR_SIZE, MUL_RD_LSB);
  decoded->multiply.rm = (u_int8_t) get_bits(instr, REG_PTR_SIZE, MUL_RM_LSB);
  decoded->multiply.rs = (u_int8_t) get_bits(instr, REG_PTR_SIZE, MUL_RS_LSB);
}

void execute_mult(State state, DecodedInstruction* instr) {
  u_int64_t product = state.registers[instr->multiply.rm] * state.registers[instr->multiply.rs];
  if (instr->multiply.acc) {
    product += state.registers[instr->multiply.rn];
  }
  *&state.registers[instr->multiply.rd] = (u_int32_t) product;
  if (instr->multiply.set_cond) {
    set_CPSR_ZN(state, (u_int32_t) product);
  }
}
