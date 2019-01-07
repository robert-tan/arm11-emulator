#ifndef EMULATE_BRANCH_H
#define EMULATE_BRANCH_H

#include "emulate_utils.h"
#include "emulate.h"

void decode_branch(u_int32_t instr, DecodedInstruction* decoded);
void execute_branch(State state, DecodedInstruction* instr);

#endif
