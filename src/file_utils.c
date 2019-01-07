#include "file_utils.h"

void read_binary_file(char* file, u_int8_t* array) {
  FILE *f = fopen(file, "r");
  fseek(f, 0, SEEK_END);
  long file_size = ftell(f);
  rewind(f);

  fread(array, 1, (size_t) file_size, f);
}
