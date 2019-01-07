#include "emulate_datap.h"

void decode_datap(u_int32_t instr, DecodedInstruction* decoded) {
  decoded->instr_type = TYPE_DATAP;
  decoded->cond = get_condition(instr);
  decoded->data_processing.set_cond = (u_int8_t) get_bits(instr, BIT_SIZE, ALL_SET_COND_LSB);
  decoded->data_processing.op_code = (u_int8_t) get_bits(instr, OPCODE_SIZE, DTP_OPCODE_LSB);
  decoded->data_processing.operand2 = get_bits(instr, SMALL_OFFSET_SIZE, DTP_OPERAND2_LSB);
  decoded->data_processing.rn = (u_int8_t) get_bits(instr, REG_PTR_SIZE, DTP_RN_LSB);
  decoded->data_processing.rd = (u_int8_t) get_bits(instr, REG_PTR_SIZE, DTP_RD_LSB);
  decoded->data_processing.i_bit = (u_int8_t) get_bits(instr, BIT_SIZE, ALL_I_LSB);
}

static u_int32_t calculate_datap_operand2(State state, DecodedInstruction* instr) {
  if (instr->data_processing.i_bit) {
    return calculate_immediate_val(instr->data_processing.operand2);
  } else {
    return calculate_register_offset(state, instr);
  }
}

void execute_datap(State state, DecodedInstruction* instr) {
  u_int32_t operand2 = calculate_datap_operand2(state, instr);
  u_int64_t temp_result;
  switch (instr->data_processing.op_code) {
    case OP_AND:
      temp_result = state.registers[instr->data_processing.rn] & operand2;
      break;
    case OP_EOR:
      temp_result = state.registers[instr->data_processing.rn] ^ operand2;
      break;
    case OP_SUB:
      temp_result = (u_int64_t) state.registers[instr->data_processing.rn] - operand2;
      break;
    case OP_RSB:
      temp_result = (u_int64_t) operand2 - state.registers[instr->data_processing.rn];
      break;
    case OP_ADD:
      temp_result = (u_int64_t) state.registers[instr->data_processing.rn] + operand2;
      break;
    case OP_TST:
      temp_result = state.registers[instr->data_processing.rn] & operand2;
      break;
    case OP_TEQ:
      temp_result = state.registers[instr->data_processing.rn] ^ operand2;
      break;
    case OP_CMP:
      temp_result = (u_int64_t) state.registers[instr->data_processing.rn] - operand2;
      break;
    case OP_ORR:
      temp_result = state.registers[instr->data_processing.rn] | operand2;
      break;
    case OP_MOV:
      temp_result = operand2;
      break;
    default:
      return;
  }
  if (instr->data_processing.op_code != OP_CMP &&
      instr->data_processing.op_code != OP_TEQ && instr->data_processing.op_code != OP_TST) {
    *&state.registers[instr->data_processing.rd] = (u_int32_t) temp_result;
  }
  if (instr->data_processing.set_cond) {
    set_CPSR_ZN(state, (u_int32_t) temp_result);
    if (instr->data_processing.op_code == OP_AND || instr->data_processing.op_code == OP_EOR ||
        instr->data_processing.op_code == OP_ORR || instr->data_processing.op_code == OP_TEQ ||
        instr->data_processing.op_code == OP_TST || instr->data_processing.op_code == OP_MOV) {
      set_CPSR(state, CPSR_C, get_carry_bit(state, instr));
    } else if (instr->data_processing.op_code == OP_ADD) {
      set_CPSR(state, CPSR_C, (u_int8_t) (temp_result > MAX_INT));
    } else if (instr->data_processing.op_code == OP_SUB || instr->data_processing.op_code == OP_CMP) {
      set_CPSR(state, CPSR_C, (u_int8_t) (operand2 <= state.registers[instr->data_processing.rn]));
    } else if (instr->data_processing.op_code == OP_RSB) {
      set_CPSR(state, CPSR_C, (u_int8_t) (operand2 >= state.registers[instr->data_processing.rn]));
    }
  }
}
