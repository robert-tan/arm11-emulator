//
// Created by Blair S. on 12/06/18.
//

#ifndef ARM11_32_ASSEMBLE_H
#define ARM11_32_ASSEMBLE_H

// the number of commends in the input file
extern int num_line;
// the position to add memory
extern int position2add;
// 6 integers to store memory of big number
extern int memory_0;
extern int memory_1;
extern int memory_2;
extern int memory_3;
extern int memory_4;
extern int memory_5;

// break the string into array of string by imet
char** tokenizer(char instr[], char* imet);

/* sourse file loader */
// give the string of all the content in the file
char* load_sourcefile(char* path);

// give the number of string in the array of string
int count_length(char** lines, int* no_register);

// write i into the file fp
void binary_writer(FILE* fp, int32_t i);

//print memory to the file
void print_memory(FILE* fp);

#endif //ARM11_32_ASSEMBLE_H
