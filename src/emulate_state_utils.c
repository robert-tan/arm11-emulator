#include "emulate_state_utils.h"

u_int32_t get_reg_value(State state, u_int8_t reg) {
  return state.registers[reg];
}

u_int32_t get_memory_word(State state, u_int32_t location) {
  if (location == GPIO_MEM_1 || location == GPIO_MEM_2 || location == GPIO_MEM_3) {
    return location;
  }
  if (!(location >= 0 && location < MEM_SIZE)) {
    printf("Error: Out of bounds memory access at address 0x%08x\n", location);
    return 0;
  }
  return ((u_int32_t) state.memory[location + 3] << 3 * BYTE_BITS) +
         ((u_int32_t) state.memory[location + 2] << 2 * BYTE_BITS) +
         ((u_int32_t) state.memory[location + 1] << BYTE_BITS) +
         (u_int32_t) state.memory[location];
}

static u_int32_t get_memory_word_reversed(State state, u_int32_t location) {
  assert (location >= 0 && location < MEM_SIZE);
  return ((u_int32_t) state.memory[location] << 3 * BYTE_BITS) +
         ((u_int32_t) state.memory[location + 1] << 2 * BYTE_BITS) +
         ((u_int32_t) state.memory[location + 2] << BYTE_BITS) +
         (u_int32_t) state.memory[location + 3];
}

void print_registers(State state) {
  printf("Registers:\n");
  for (int i = 0; i < 13; i++) {
    printf("$%-3i: %10i (0x%08x)\n", i, state.registers[i], state.registers[i]);
  }
  printf("PC  : %10i (0x%08x)\n", state.registers[REG_PC], state.registers[REG_PC]);
  printf("CPSR: %10i (0x%08x)\n", state.registers[REG_CPSR], state.registers[REG_CPSR]);
}

void print_memory(State state) {
  printf("Non-zero memory:\n");
  for (int i = 0; i < MEM_SIZE; i += 4) {
    u_int32_t word = get_memory_word_reversed(state, (u_int32_t) i);
    if (word != 0) {
      printf("0x%08x: 0x%08x\n", i, word);
    }
  }
}

void set_CPSR(State state, u_int32_t flag, u_int8_t val) {
  *& state.registers[REG_CPSR] |= flag;
  if (!val) {
    *& state.registers[REG_CPSR] ^= flag;
  }
}

void set_CPSR_ZN(State state, u_int32_t result) {
  set_CPSR(state, CPSR_N, (u_int8_t) get_bits(result, BIT_SIZE, MSB));
  set_CPSR(state, CPSR_Z, (u_int8_t) (result == 0));
}

u_int32_t check_cond(State state, DecodedInstruction* instr) {
  u_int32_t CPSR = state.registers[REG_CPSR];
  switch (instr->cond) {
    case EQ: return (u_int32_t) (CPSR & CPSR_Z);
    case NE: return (u_int32_t) !(CPSR & CPSR_Z);
    case GE: return (u_int32_t) ((!(CPSR & CPSR_V) && !(CPSR & CPSR_N)) || ((CPSR & CPSR_V) && (CPSR & CPSR_N)));
    case LT: return (u_int32_t) !((!(CPSR & CPSR_V) && !(CPSR & CPSR_N)) || ((CPSR & CPSR_V) && (CPSR & CPSR_N)));
    case GT: return (u_int32_t) (!(CPSR & CPSR_Z) && ((!(CPSR & CPSR_V) &&
          !(CPSR & CPSR_N)) || ((CPSR & CPSR_V) && (CPSR & CPSR_N))));
    case LE: return (u_int32_t) ((CPSR & CPSR_Z) || (!((!(CPSR & CPSR_V) &&
          !(CPSR & CPSR_N)) || ((CPSR & CPSR_V) && (CPSR & CPSR_N)))));
    case AL: return TRUE;
    default: return (u_int8_t) -1;
  }
}

u_int32_t calculate_register_offset(State state, DecodedInstruction* instr) {
  u_int32_t offset;
  if (instr->instr_type == TYPE_DATAP) {
    offset = instr->data_processing.operand2;
  } else {
    offset = instr->sdt.offset;
  }
  int32_t rm_val = get_reg_value(state, (u_int8_t) (offset & OFFSET_RM_M));
  u_int8_t shift_type = (u_int8_t) get_bits(offset, BIT_SIZE, OFFSET_SHIFT_LSB);
  u_int8_t shift_op = (u_int8_t) (offset & OFFSET_TYPE_M) >> OFFSET_TYPE_L;
  u_int32_t shift_amount;
  if (!(shift_type)) {
    shift_amount = (offset & OFFSET_CONST_M) >> OFFSET_CONST_L;

  } else {
    u_int8_t rs = (u_int8_t) ((offset & OFFSET_RS_M) >> OFFSET_RS_L);
    shift_amount = get_bits(state.registers[rs], BYTE_BITS, 0);
  }
  switch (shift_op) {
    case SHIFT_LSL:
      return (u_int32_t) (rm_val << shift_amount);
    case SHIFT_LSR:
      return ((u_int32_t) rm_val) >> shift_amount;
    case SHIFT_ASR:
      return (u_int32_t) (rm_val >> shift_amount);
    case SHIFT_ROR:
      return rotate((u_int32_t) rm_val, shift_amount);
    default:
      break;
  }
  return (u_int32_t) -1;
}

u_int8_t get_carry_bit(State state, DecodedInstruction* instr) {
  assert (instr->instr_type == TYPE_DATAP && instr->data_processing.set_cond);
  int32_t rm_val = get_reg_value(state, (u_int8_t) (instr->data_processing.operand2 & OFFSET_RM_M));
  u_int8_t shift_amount = (u_int8_t) (instr->data_processing.operand2 & OFFSET_CONST_M) >> OFFSET_CONST_L;
  if (shift_amount == 0) {
    return 0;
  }
  u_int8_t shift_op = (u_int8_t) (instr->data_processing.operand2 & OFFSET_TYPE_M) >> OFFSET_TYPE_L;
  if (shift_op == SHIFT_LSL) {
    return (u_int8_t) get_bits((u_int32_t) rm_val, BIT_SIZE, (u_int8_t) MSB - shift_amount);
  } else {
    return (u_int8_t) get_bits((u_int32_t) rm_val, BIT_SIZE, shift_amount - (u_int8_t) 1);
  }
}

void initialize(State state) {
  memset(state.memory, 0, MEM_SIZE);
  memset(state.registers, 0, REG_BYTES * REG_NUM);
}
