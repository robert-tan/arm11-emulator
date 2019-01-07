//
// Created by Blair S. on 11/06/18.
//

#include "Instruction.h"
#include "constants.h"

// initialize the value
int num_line=0;
int position2add=0;
int memory_0=0;
int memory_1=0;
int memory_2=0;
int memory_3=0;
int memory_4=0;
int memory_5=0;

/* check immediate value or not*/
int immediate(char* oprand) {
  return (oprand[0]=='#') || (oprand[0] == '=');
}

char* remove_first(char* oprand) {
  char *p = malloc(sizeof(*p) * strlen(oprand));
  int i;
  for(i = 0; i<strlen(oprand); i++) {
    p[i] = oprand[i + 1];
  }
  return p;
}

char* remove_last(char* oprand) {
  char *p = malloc(sizeof(*p) * strlen(oprand));
  int i;
  for(i = 0; i<strlen(oprand) - 1; i++) {
    p[i] = oprand[i];
  }

  return p;
}

int get_index(char* r) {
  return atoi(remove_first(r));
}

//give the binary form of given int
char* int2bin(unsigned n, char *buf) {
#define BITS (sizeof(n) * CHAR_BIT)
  static char static_buf[BITS + 1];
  int i;
  if (buf == NULL)
    buf = static_buf;
  for (i = BITS - 1; i >= 0; --i) {
    buf[i] = (n & 1) ? '1' : '0';
    n >>= 1;
  }
  buf[BITS] = '\0';
  return buf;
}

int check_trailing_zero(char* binary) {
  int len = strlen(binary);
  if (binary[len - 1] == '0'
      && binary[len - 2] == '0') { // the last two
    return 1;
  }
  return 0;
}

int16_t get_constant(char* r, int* rotate_index) {
  if (!rotate_index) {
    rotate_index = malloc(sizeof(int));
  }
  const char* temp = remove_first(r);
  if (temp[1] == 'x') { // binary number 0xff
    int res = (int) strtol(temp, NULL, 0);
    if (res > BYTE_MASK) { // to deal large number
      char* buf = malloc((MSB + 1) * sizeof(char));
      buf = int2bin(res, buf);
      while ((res > BYTE_MASK || check_trailing_zero(buf))) {
        res = res >> SHIFT_PER_TIME;
        buf = int2bin(res, buf);
        (*rotate_index) = (*rotate_index) + SHIFT_PER_TIME;
      }
      *rotate_index = (MSB + 1) - (*rotate_index);
      *rotate_index = (*rotate_index) / SHIFT_PER_TIME;
    }
    return res;
  } else { // decimal number #1 or #945646
    u_int32_t cur =  atoi(temp);
    if (cur > BYTE_MASK) {
      char* buf = malloc((MSB + 1) * sizeof(char));
      buf = int2bin(cur, buf);
      while ((cur > BYTE_MASK || check_trailing_zero(buf))) {
        cur = cur >> SHIFT_PER_TIME;
        buf = int2bin(cur, buf);
        (*rotate_index) = (*rotate_index) + SHIFT_PER_TIME;
      }
      *rotate_index = (MSB + 1) - (*rotate_index);
      *rotate_index = (*rotate_index) / SHIFT_PER_TIME;
    }
    return cur;
  }
}

/* Implementation of function for assembling Data Processing instructions */
int32_t mov(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = MOV_DEF;
  result += get_index(tokens[1]) * RD;
  char* tid_ar = tokens[2]; // the third argument of the commend line
  if (immediate(tid_ar)) {
    result += 3 * IM_P; // according to spec
    int rotate = 0;
    result += get_constant(tid_ar, &rotate);
    result += TID_ARG * rotate;
  } else {
    result += IM_P;
    result += get_index(tid_ar);
  }
  return result;
}

int32_t add(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = ADD_DEF;
  result += get_index(tokens[1]) * RD;
  char* third_arg = tokens[2]; // the third argument of the commend line
  char* fouth_arg = tokens[3]; // the third argument of the commend line
  result += get_index(third_arg) * RN;
  if (immediate(fouth_arg)) {
    result += 2 * IM_P;
    result += get_constant(fouth_arg, 0);
  } else {
    if ((third_arg[strlen(third_arg) - 1]!=']') || (fouth_arg==NULL)) {
      result += get_index(fouth_arg);
    }
  }
  if (tokens[4]) { // check whether it need shifting
    result += get_index(tokens[5]) * TID_ARG; // get shift number
  }
  return result;
}

int32_t sub(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = SUB_DEF;
  result += get_index(tokens[1]) * RD;
  char* third_arg = tokens[2]; // the third argument of the commend line
  char* fouth_arg = tokens[3]; // the third argument of the commend line
  result += get_index(third_arg) * RN;
  if (immediate(fouth_arg)) {
    result += 2 * IM_P;
    result += get_constant(fouth_arg, 0);
  } else {
    result += get_index(fouth_arg);
    char* fifth_arg = tokens[4]; // shift position
    if (fifth_arg) {
      result += get_index(tokens[5]) * TID_ARG; // get the shift value
      int shift_type = 0;
      if (!strcmp(fifth_arg, "lsl")) {
        shift_type = 1;
      } else if (!strcmp(fifth_arg, "lsr")) {
        shift_type = 3;
      } else if (!strcmp(fifth_arg, "asr")) {
        shift_type = 5;
      } else if (!strcmp(fifth_arg, "ror")) {
        shift_type = 7;
      }
      result += shift_type * SHI_P;
    }
  }

  return result;

}

int32_t rsb(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = RSB_DEF;
  result += get_index(tokens[1]) * RD;
  char* third_arg = tokens[2]; // the third argument of the commend line
  char* fouth_arg = tokens[3]; // the third argument of the commend line
  result += get_index(third_arg) * RN;
  if (immediate(tokens[3])) {
    result += 2 * IM_P; // according to spec
    result += get_constant(fouth_arg, 0);
  } else {
    result += get_index(fouth_arg);
  }
  return result;
}

int32_t and(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = AND_DEF;
  result += get_index(tokens[1]) * RD;
  char* third_arg = tokens[2];
  char* fouth_arg = tokens[3];
  result += get_index(third_arg) * RN;
  if (immediate(fouth_arg)) {
    result += 2 * IM_P;
    result += get_constant(fouth_arg, 0);
  } else {
    result += get_index(fouth_arg);
  }
  return result;
}

int32_t eor(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = EOR_DEF;
  result += get_index(tokens[1]) * RD;
  char* third_arg = tokens[2];
  char* fouth_arg = tokens[3];
  result += get_index(third_arg) * RN;
  if (immediate(fouth_arg)) {
    result += 2 * IM_P;
    result += get_constant(fouth_arg, 0);
  } else {
    result += get_index(fouth_arg);
  }
  return result;
}

int32_t orr(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = ORR_DEF;
  result += get_index(tokens[1]) * RD;
  char* third_arg = tokens[2];
  char* fouth_arg = tokens[3];
  result += get_index(third_arg) * RN;
  result += IM_P;
  if (immediate(fouth_arg)) {
    result += 2 * IM_P;
    result += get_constant(fouth_arg, 0);
  } else {
    result += get_index(fouth_arg);
  }
  return result;
}

int32_t tst(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = 0xe0100000;
  result += get_index(tokens[1]) * RN;
  char* third_arg = tokens[2];
  if (immediate(third_arg)) {
    result += IM_P * 3;
    result += get_constant(third_arg, 0);
  } else {
    result += IM_P;
    result += get_index(third_arg);
  }
  return result;
}

int32_t teq(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = TEQ_DEF;
  result += get_index(tokens[1]) * RN;
  char* third_arg = tokens[2];
  if (immediate(third_arg)) {
    result += IM_P * 3;
    result += get_constant(third_arg, 0);
  } else {
    result += IM_P;
    result += get_index(third_arg);
  }
  return result;
}

int32_t cmp(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = CMP_DEF;
  result += get_index(tokens[1]) * RN;
  char* third_arg = tokens[2];
  if (immediate(third_arg)) {
    result += IM_P * 3;
    result += get_constant(third_arg, 0);
  } else {
    result += IM_P;
    result += get_index(third_arg);
  }
  return result;
}

int32_t mul(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = MUL_DEF;
  result += get_index(tokens[1]) * RN;
  result += get_index(tokens[2]);
  result += get_index(tokens[3]) * TID_ARG;
  return result;
}

int32_t mla(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = MLA_DEF;
  result += get_index(tokens[1]) * RN;
  result += get_index(tokens[2]);
  result += get_index(tokens[3]) * TID_ARG;
  result += get_index(tokens[4]) * RD;

  return result;
}

void store_memory(int32_t i) {
  if (!memory_0) {
    memory_0 = i;
  } else {
    if (!memory_1) {
      memory_1 = i;
    } else {
      if (!memory_2) {
        memory_2 = i;
      } else {
        if (!memory_3) {
          memory_3 = i;
        } else {
          if (!memory_4)  {
            memory_4 = i;
          } else {
            if (!memory_5) {
              memory_5 = i;
            }
          }
        }
      }
    }
  }
}


int32_t ldr(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = LDR_DEF; //Set cond, U and L bits
  result += RD * get_index(tokens[1]); //Rd
  char* third_arg = tokens[2];
  char* fouth_arg = tokens[3];
  char* sixth_arg = tokens[5];
  if (third_arg[0] == '=') {
    int con = (int) strtol(remove_first(third_arg),NULL,0);
    if (con <= BYTE_MASK) {  // eg. offset = "=0x02"
      result = mov(tokens, st, no_line);
    } else { // eg. offset = "0x11456114"
      store_memory(con);
      result += 15 * RN;
      result += IM_P;
      int gap=position2add-no_line - 1;
      result+= 4 * gap; //offset
      position2add++;
    }
  } else { // eg. [...]
    if ((third_arg[strlen(third_arg) - 1]!=']') || (fouth_arg==NULL)) {
      result += IM_P; //Set P bit
    }
    result += RN * get_index(remove_first(third_arg)); //Rn
    if (fouth_arg!=NULL) { //Offset or not
      if (fouth_arg[0]=='#') { //immediate or not
        if (fouth_arg[1] == '-') { // Clear U bit if negative
          result -= TYPE * 8;
          result += get_constant(remove_first(fouth_arg), 0);
        } else {
          result += get_constant(fouth_arg, 0);
        }
      } else {
        result += 2 * IM_P; //Set I bit
        if (fouth_arg[0] == '-') { // Clear U bit if negative
          result -= TYPE * 8;
        }
        if (sixth_arg!=NULL) { //Shift or not
          result += TID_ARG;
          result += SHI_P * get_index(fouth_arg);
          result += get_constant(sixth_arg, 0);
        } else {
          result += get_index(fouth_arg);
        }
      }
    }
  }

  return result;
}

int32_t str(char** tokens, sym_table_t* st, int no_line) { // differ from ldr only L bit set
  int32_t result = STR_DEF; //Set cond and U bits
  result += RD * get_index(tokens[1]); //Rd
  char* third_arg = tokens[2];
  char* fouth_arg = tokens[3];
  result += RN * get_index(remove_first(third_arg)); //Rn
  if ((third_arg[strlen(third_arg) - 1]!=']') || (fouth_arg==NULL)) {
    result += IM_P; //Set P bit
  }
  if (fouth_arg!=NULL) { //Offset or not
    if (fouth_arg[0]=='#') { //immediate or not
      if (fouth_arg[1] == '-') { // Clear U bit if negative
        result -= TYPE * 8;
        result += get_constant(remove_first(fouth_arg), 0);
      } else {
        result += get_constant(fouth_arg, 0);
      }
    } else {
      result += IM_P * 2; //Set I bit
      result += get_index(fouth_arg);
      if (fouth_arg[0] == '-') { // Clear U bit if negative
        result -= TYPE * 8;
      }
    }
  }
  return result;
}

// helper function to deal with label
int32_t deal_label(int32_t ini, char* label, sym_table_t* st, int no_line) {
  node_t* sym = symbol_find_by_name(st, label);
  if (sym) { // symbol has added to the table
    int offset = BRC_OFFSET_M - (no_line -(int) sym->symbol.address);
    ini += offset;
  } else {
    symbol_add(st, label, no_line);
  }
  return ini;
}

int32_t beq(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = BEQ_DEF;
  result = deal_label(result, tokens[1], st, no_line);

  return result;
}

int32_t bne(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = BNE_DEF;
  result = deal_label(result, tokens[1], st, no_line);
  return result;
}

int32_t bge(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = BGE_DEF;
  result = deal_label(result, tokens[1], st, no_line);
  return result;
}

int32_t blt(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = BLT_DEF;
  result = deal_label(result, tokens[1], st, no_line);
  return result;
}

int32_t bgt(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = BGE_DEF;
  result = deal_label(result, tokens[1], st, no_line);
  return result;
}

int32_t ble(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = BLE_DEF;
  result = deal_label(result, tokens[1], st, no_line);
  return result;
}

int32_t b(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = B_DEF;
  result = deal_label(result, tokens[1], st, no_line);
  return result;
}

int32_t lsl(char** tokens, sym_table_t* st, int no_line) {
  int32_t result = LSL_DEF;
  result += get_index(tokens[1]) * (RD + 1);
  result += get_constant(tokens[2], 0) * (SHI_P * 8);
  return result;
}

int32_t andeq(char** tokens, sym_table_t* st, int no_line) {
  return 0;
}

sym_table_t* create_ins_table() {
  sym_table_t* ins_table = symbol_init();
  symbol_add(ins_table, "add", &add);
  symbol_add(ins_table, "sub", &sub);
  symbol_add(ins_table, "rsb", &rsb);
  symbol_add(ins_table, "and", &and);
  symbol_add(ins_table, "eor", &eor);
  symbol_add(ins_table, "orr", &orr);
  symbol_add(ins_table, "mov", &mov);
  symbol_add(ins_table, "tst", &tst);
  symbol_add(ins_table, "teq", &teq);
  symbol_add(ins_table, "cmp", &cmp);
  symbol_add(ins_table, "mul", &mul);
  symbol_add(ins_table, "mla", &mla);
  symbol_add(ins_table, "ldr", &ldr);
  symbol_add(ins_table, "str", &str);
  symbol_add(ins_table, "beq", &beq);
  symbol_add(ins_table, "bgt", &bgt);
  symbol_add(ins_table, "bne", &bne);
  symbol_add(ins_table, "bge", &bge);
  symbol_add(ins_table, "blt", &blt);
  symbol_add(ins_table, "ble", &ble);
  symbol_add(ins_table, "b", &b);
  symbol_add(ins_table, "lsl", &lsl);
  symbol_add(ins_table, "andeq", &andeq);
  return ins_table;
}
