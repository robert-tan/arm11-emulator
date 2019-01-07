#ifndef EMULATE_MULT_H
#define EMULATE_MULT_H

#include "emulate.h"
#include "emulate_utils.h"
#include "emulate_state_utils.h"

void decode_mult(u_int32_t instr, DecodedInstruction* decoded);
void execute_mult(State state, DecodedInstruction* instr);

#endif
