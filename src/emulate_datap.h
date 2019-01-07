#ifndef EMULATE_DATAP_H
#define EMULATE_DATAP_H

#include "emulate.h"
#include "emulate_utils.h"
#include "emulate_state_utils.h"

void decode_datap(u_int32_t instr, DecodedInstruction* decoded);
void execute_datap(State state, DecodedInstruction* instr);

#endif
