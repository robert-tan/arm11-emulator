//
// Created by Blair S. on 11/06/18.
//

#include "SymTab.h"
#include "assemble.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>



#ifndef ARM11_32_INSTRUCTION_H
#define ARM11_32_INSTRUCTION_H

// functions for the instruction of the assembler
// char** tokens: the commend of each line
// sym_table_t* st: the symbol table for the label and address
// no_line: the current pipeline
typedef int32_t (*func)(char**, sym_table_t*, int);

int32_t add(char** tokens, sym_table_t* st, int no_line);

int32_t sub(char** tokens, sym_table_t* st, int no_line);

int32_t rsb(char** tokens, sym_table_t* st, int no_line);

int32_t and(char** tokens, sym_table_t* st, int no_line);

int32_t eor(char** tokens, sym_table_t* st, int no_line);

int32_t orr(char** tokens, sym_table_t* st, int no_line);

int32_t mov(char** tokens, sym_table_t* st, int no_line);

int32_t tst(char** tokens, sym_table_t* st, int no_line);

int32_t teq(char** tokens, sym_table_t* st, int no_line);

int32_t cmp(char** tokens, sym_table_t* st, int no_line);

int32_t mul(char** tokens, sym_table_t* st, int no_line);

int32_t mla(char** tokens, sym_table_t* st, int no_line);

int32_t ldr(char** tokens, sym_table_t* st, int no_line);

int32_t str(char** tokens, sym_table_t* st, int no_line);

int32_t beq(char** tokens, sym_table_t* st, int no_line);

int32_t bne(char** tokens, sym_table_t* st, int no_line);

int32_t bge(char** tokens, sym_table_t* st, int no_line);

int32_t blt(char** tokens, sym_table_t* st, int no_line);

int32_t bgt(char** tokens, sym_table_t* st, int no_line);

int32_t ble(char** tokens, sym_table_t* st, int no_line);

int32_t b(char** tokens, sym_table_t* st, int no_line);

int32_t lsl(char** tokens, sym_table_t* st, int no_line);

int32_t andeq(char** tokens, sym_table_t* st, int no_line);

// remove first char in a string
char* remove_first(char* oprand);

// remove last char in a string
char* remove_last(char* oprand);

// create a symbol table with the instruction name and the instruction function
sym_table_t* create_ins_table();

// check immediate value or not
int immediate(char* oprand);

// get index from the register, eg. r1 will get 1
int get_index(char* r);

//give the binary form of given int
char* int2bin(unsigned n, char *buf);

// return 1 if the last two digit of the number is 0
// otherwise return 0
int check_trailing_zero(char* binary);

// get the int constant
// eg. #5, #0xff, =46465489
int16_t get_constant(char* r, int* rotate_index);

// stores i into the memory space of the assembler
void store_memory(int32_t i) ;

// deal with label
// calculate the offset
int32_t deal_label(int32_t ini, char* label, sym_table_t* st, int no_line);


#endif //ARM11_32_INSTRUCTION_H
