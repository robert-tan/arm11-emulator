#ifndef EMULATE_STATE_UTILS_H
#define EMULATE_STATE_UTILS_H

#include "emulate_utils.h"
#include "emulate.h"

u_int32_t get_reg_value(State state, u_int8_t reg);
u_int32_t get_memory_word(State state, u_int32_t location);
void set_CPSR(State state, u_int32_t flag, u_int8_t val);
void set_CPSR_ZN(State state, u_int32_t result);
u_int32_t check_cond(State state, DecodedInstruction* instr);
void print_registers(State state);
void print_memory(State state);
u_int32_t calculate_register_offset(State state, DecodedInstruction* instr);
u_int8_t get_carry_bit(State state, DecodedInstruction* instr);
void initialize(State state);

#endif
