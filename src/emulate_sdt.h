#ifndef EMULATE_SDT_H
#define EMULATE_SDT_H

#include "emulate.h"
#include "emulate_utils.h"
#include "emulate_state_utils.h"

void decode_sdt(u_int32_t instr, DecodedInstruction* decoded);
void execute_sdt(State state, DecodedInstruction* instr);

#endif 
