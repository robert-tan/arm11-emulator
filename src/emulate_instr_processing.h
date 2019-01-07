#ifndef EMULATE_INSTR_PROCESSING
#define EMULATE_INSTR_PROCESSING

#include "emulate.h"
#include "emulate_branch.h"
#include "emulate_datap.h"
#include "emulate_sdt.h"
#include "emulate_mult.h"
#include "emulate_state_utils.h"
#include "emulate_utils.h"

void execute_pipeline(State state, Pipeline* pipeline);

#endif
