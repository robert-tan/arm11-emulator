#ifndef EMULATE_UTILS_H
#define EMULATE_UTILS_H

#include <stdlib.h>
#include <assert.h>
#include "constants.h"

u_int32_t get_bits(u_int32_t instr, u_int8_t length, u_int8_t lsb);
u_int32_t rotate(u_int32_t val, u_int32_t rot);
u_int8_t get_condition(u_int32_t instr);
u_int8_t split_to_byte(u_int32_t data, u_int8_t byte_num);
u_int8_t get_instr_type(u_int32_t instr);
u_int32_t calculate_immediate_val(u_int32_t offset);

#endif
