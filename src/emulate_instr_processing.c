#include "emulate_instr_processing.h"

static void decode_instruction(State state, u_int32_t location, DecodedInstruction* decoded) {
  u_int32_t instr = get_memory_word(state, location);
  u_int8_t type = get_instr_type(instr);
  switch (type) {
    case HALT: decoded->instr_type = HALT; break;
    case TYPE_BRNCH: decode_branch(instr, decoded); break;
    case TYPE_MULT: decode_mult(instr, decoded); break;
    case TYPE_SDT: decode_sdt(instr, decoded); break;
    case TYPE_DATAP: decode_datap(instr, decoded); break;
    default: break;
  }
}

static void execute_instruction(State state, DecodedInstruction* instr) {
  if (check_cond(state, instr)) {
    switch (instr->instr_type) {
      case TYPE_BRNCH: execute_branch(state, instr); break;
      case TYPE_MULT: execute_mult(state, instr); break;
      case TYPE_SDT: execute_sdt(state, instr); break;
      case TYPE_DATAP: execute_datap(state, instr); break;
      default: break;
    }
  }
}

void execute_pipeline(State state, Pipeline* pipeline) {
  u_int8_t clear_pipeline;
  pipeline->has_fetched = FALSE;
  pipeline->has_decoded = FALSE;
  pipeline->halt = FALSE;
  DecodedInstruction* decoded = malloc(sizeof(DecodedInstruction));
  while (!(pipeline->halt)) {
    clear_pipeline = FALSE;
    if (pipeline->has_decoded) {
      if (decoded->instr_type != HALT) {
        execute_instruction(state, decoded);
      } else {
        pipeline->halt = TRUE;
      }
      pipeline->has_decoded = FALSE;
      if (decoded->instr_type == TYPE_BRNCH && check_cond(state, decoded)) {
        clear_pipeline = TRUE;
      }
    }
    if (!pipeline->halt) {
      if (pipeline->has_fetched) {
        if (!clear_pipeline) {
          decode_instruction(state, pipeline->fetched, decoded);
          pipeline->has_decoded = TRUE;
        } else {
          pipeline->has_fetched = FALSE;
          continue;
        }
      }
      pipeline->fetched = *&state.registers[REG_PC];
      pipeline->has_fetched = TRUE;
      *&state.registers[REG_PC] += REG_BYTES;
    }
  }
  free(decoded);
}
