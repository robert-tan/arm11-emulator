#include <assert.h>
#include "SymTab.h"
#include "Instruction.h"
#include "assemble.h"

char** tokenizer(char instr[], char* imet) {
  char **result = NULL;
  char *p = strtok(instr, imet);
  int n_spaces = 0;
  while(p) {
    result = realloc(result, sizeof(char*) * ++n_spaces);
    if (result == NULL)
      exit(-1);
    result[n_spaces - 1] = p;
    p = strtok(NULL, imet);
  }
  result = realloc(result, sizeof(char*) * (n_spaces + 1));
  result[n_spaces] = 0;
  return result;
}


/* sourse file loader */
char* load_sourcefile(char* path) {
  char * buffer = 0;
  long length;
  FILE * fp = fopen (path, "r");
  if (fp)
  {
    fseek (fp, 0, SEEK_END);
    length = ftell (fp);
    fseek (fp, 0, SEEK_SET);
    buffer = malloc (length);
    if (buffer) {
      fread (buffer, 1, length, fp);
    }
    fclose (fp);
  }
  return buffer;
}

// give the number of string in the array of string
int count_length(char** lines, int* no_label){
  int i = 0;
  char* line = lines[i];
  while (line != NULL && strcmp(line, "1") && strcmp(line, "A")) {
    if (line[strlen(line) - 1] == ':') {
      *no_label = *no_label + 1;
    }
    i++;
    line = lines[i];
  }
  return i;
}

// write i into the file fp
void binary_writer(FILE* fp, int32_t i) {
  u_int8_t a = (u_int8_t) i;
  u_int8_t b = (u_int8_t) (i >> 8);
  u_int8_t c = (u_int8_t) (i >> 16);
  u_int8_t d = (u_int8_t) (i >> 24);
  fwrite(&a, sizeof(u_int8_t), 1, fp);
  fwrite(&b, sizeof(u_int8_t), 1, fp);
  fwrite(&c, sizeof(u_int8_t), 1, fp);
  fwrite(&d, sizeof(u_int8_t), 1, fp);
}

void print_memory(FILE* fp) {
  if (memory_0) {
    binary_writer(fp, memory_0);
  } else {
    return;
  }
  if (memory_1) {
    binary_writer(fp, memory_1);
  } else {
    return;
  }
  if (memory_2) {
    binary_writer(fp, memory_2);
  } else {
    return;
  }
  if (memory_3) {
    binary_writer(fp, memory_3);
  } else {
    return;
  }
  if (memory_4) {
    binary_writer(fp, memory_4);
  } else {
    return;
  }
  if (memory_5) {
    binary_writer(fp, memory_5);
  } else {
    return;
  }
}


/* Implementation of the assembly process.*/
int main(int argc, char **argv) {
  // initialize
  assert(argc == 3);
  //create symbol table for label and address
  sym_table_t* label_address = symbol_init();
  // create symbol table for instruction function
  sym_table_t* ins_table = create_ins_table();
  char* file = load_sourcefile(argv[1]);   // load a file into a string
  int32_t res = 0; // the result
  FILE *binary_file = fopen(argv[2], "wb");   // create the output file
  int no_label = 0; // number of register in the file
  int cur_line = 0; // the position of the current line in the input file

  // break the input file line by line
  char** lines = tokenizer(file, "\r\n");
  num_line = count_length(lines, &no_label);
  position2add= num_line - no_label;

  // translate the string into binary line by line
  for (int i = 0; i < num_line; i++) {
    char** token_word = tokenizer(lines[i], " ,;");
    char* op = token_word[0];
    if (op[strlen(op) - 1] != ':') { // commend line
      cur_line++;
      // find the corresponding function of instruction
      func ins = (func) symbol_find_by_name(ins_table, op)->symbol.address;
      res = ins(token_word, label_address, cur_line);
      binary_writer(binary_file, res);
    } else { //label, eg. wait:
      node_t* sym = symbol_find_by_name(label_address, op);
      if (!sym) { // have not add to the table
        symbol_add(label_address, remove_last(op), cur_line);
      }
    }
    free(token_word);
  }

  print_memory(binary_file);

  //clean memory
  free(label_address);
  free(ins_table);
  free(file);
  free(lines);
  fclose(binary_file);
  return EXIT_SUCCESS;
}
