#ifndef EMULATE_H
#define EMULATE_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>
#include "constants.h"
#include "file_utils.h"

typedef struct {
  u_int8_t* memory;
  u_int32_t* registers;
} State;

typedef struct {
  u_int32_t operand2;
  u_int8_t op_code;
  u_int8_t rn;
  u_int8_t rd;
  u_int8_t set_cond;
  u_int8_t i_bit;
} DataProcessing;

typedef struct {
  int32_t offset;
} Branch;

typedef struct {
  u_int8_t set_cond;
  u_int8_t rn;
  u_int8_t rd;
  u_int8_t rs;
  u_int8_t rm;
  u_int8_t acc;
} Multiply;

typedef struct {
  u_int8_t rn;
  u_int8_t rd;
  u_int32_t offset;
  u_int8_t pre_indexing;
  u_int8_t up_bit;
  u_int8_t load_store_bit;
  u_int8_t i_bit;
} SDT;

typedef struct {
  u_int8_t instr_type;
  u_int8_t cond;

  DataProcessing data_processing;
  Branch branch;
  Multiply multiply;
  SDT sdt;

} DecodedInstruction;

typedef struct {
  u_int8_t halt;
  u_int8_t has_fetched;
  u_int8_t has_decoded;
  u_int32_t fetched;
} Pipeline;

void initialize(State state);
void execute_pipeline(State state, Pipeline* pipeline);
int main(int argc, char **argv);

#endif
