#include "emulate_sdt.h"

void decode_sdt(u_int32_t instr, DecodedInstruction* decoded) {
  decoded->instr_type = TYPE_SDT;
  decoded->cond = get_condition(instr);
  decoded->sdt.offset = get_bits(instr, SMALL_OFFSET_SIZE, SDT_OFFSET_LSB);
  decoded->sdt.load_store_bit = (u_int8_t) get_bits(instr, BIT_SIZE, SDT_L_LSB);
  decoded->sdt.up_bit = (u_int8_t) get_bits(instr, BIT_SIZE, SDT_U_LSB);
  decoded->sdt.pre_indexing = (u_int8_t) get_bits(instr, BIT_SIZE, SDT_P_LSB);
  decoded->sdt.rd = (u_int8_t) get_bits(instr, REG_PTR_SIZE, SDT_RD_LSB);
  decoded->sdt.rn = (u_int8_t) get_bits(instr, REG_PTR_SIZE, SDT_RN_LSB);
  decoded->sdt.i_bit = (u_int8_t) get_bits(instr, BIT_SIZE, ALL_I_LSB);
}

static u_int32_t calculate_sdt_offset(State state, DecodedInstruction* instr) {
  if (!instr->sdt.i_bit) {
    return (u_int32_t) instr->sdt.offset;
  } else {
    return calculate_register_offset(state, instr);
  }
}

static u_int32_t calculate_memory_address(State state, DecodedInstruction* instr, u_int32_t offset) {
  if (instr->sdt.up_bit) {
    return state.registers[instr->sdt.rn] + offset;
  } else{
    return state.registers[instr->sdt.rn] - offset;
  }
}

static void put_memory_word(State state, u_int32_t location, u_int32_t data) {
  *&state.memory[location] = split_to_byte(data, 0);
  *&state.memory[location + 1] = split_to_byte(data, 1);
  *&state.memory[location + 2] = split_to_byte(data, 2);
  *&state.memory[location + 3] = split_to_byte(data, 3);
}

static void load_store_data(State state, u_int8_t load_store_bit, u_int8_t rd, u_int32_t memory_loc) {
  if (load_store_bit) {
    *&state.registers[rd] = get_memory_word(state, memory_loc);
  } else {
    put_memory_word(state, memory_loc, state.registers[rd]);
  }
}

static void execute_gpio_memory(State state, DecodedInstruction* instr, u_int32_t address) {
  int start_position, end_position;
  if (address == GPIO_MEM_1) {
    start_position = 0;
    end_position = 9;
  } else if (address == GPIO_MEM_2) {
    start_position = 10;
    end_position = 19;
  } else if (address == GPIO_MEM_3) {
    start_position = 20;
    end_position = 29;
  }
  printf("One GPIO pin from %d to %d has been accessed\n", start_position, end_position);
}

void execute_sdt(State state, DecodedInstruction* instr) {
  u_int32_t address = calculate_memory_address(state, instr, calculate_sdt_offset(state, instr));
  if (address == GPIO_MEM_1 || address == GPIO_MEM_2 || address == GPIO_MEM_3) {
    execute_gpio_memory(state, instr, address);
    if (!instr->sdt.load_store_bit) {
        return;
    }
  }
  else if (address == GPIO_CLEAR) {
    printf("PIN OFF\n");
    return;
  } else if (address == GPIO_SET) {
    printf("PIN ON\n");
    return;
  }
  if (instr->sdt.pre_indexing) {
    load_store_data(state, instr->sdt.load_store_bit, instr->sdt.rd,
                    calculate_memory_address(state, instr, calculate_sdt_offset(state, instr)));
  } else {
    load_store_data(state, instr->sdt.load_store_bit, instr->sdt.rd, state.registers[instr->sdt.rn]);
    *&state.registers[instr->sdt.rn] = calculate_memory_address(state, instr, calculate_sdt_offset(state, instr));
  }
}
