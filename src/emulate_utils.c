#include "emulate_utils.h"

u_int32_t get_bits(u_int32_t instr, u_int8_t length, u_int8_t lsb) {
  return (((1 << length) - 1) & (instr >> lsb));
}

u_int32_t rotate(u_int32_t val, u_int32_t rot) {
  u_int8_t mask = INSTR_BITS - 1;
  rot &= mask;
  return val >> rot | (val << (INSTR_BITS - rot));
}

u_int8_t get_condition(u_int32_t instr) {
  return (u_int8_t) get_bits(instr, COND_SIZE, ALL_COND_LSB);
}

u_int8_t split_to_byte(u_int32_t data, u_int8_t byte_num) {
  return (u_int8_t) ((data & (BYTE_MASK << (byte_num * BYTE_BITS))) >> (byte_num * BYTE_BITS));
}

u_int8_t get_instr_type(u_int32_t instr) {
  if (instr == 0) {
    return HALT;
  } else {
    if ((instr & DET_BRNCH) == DET_BRNCH) {
      return TYPE_BRNCH;
    } else if (instr & DET_SDT) {
      return TYPE_SDT;
    } else if (((instr & DET_MULT) == DET_MULT) && !(instr & DET_DATAP) ) {
      return TYPE_MULT;
    } else {
      return TYPE_DATAP;
    }
  }
}

u_int32_t calculate_immediate_val(u_int32_t offset) {
  u_int8_t rot = (u_int8_t) (2 * ((offset & OFFSET_ROT_M) >> OFFSET_ROT_L));
  assert (rot >= 0 && rot < INSTR_BITS);
  return rotate(offset & OFFSET_IMM_M, rot);
}
