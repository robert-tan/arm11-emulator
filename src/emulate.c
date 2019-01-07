#include "emulate.h"
#include "file_utils.h"
#include "emulate_utils.h"
#include "emulate_state_utils.h"

int main(int argc, char **argv) {
  assert(argc == 2);

  State arm_machine;

  arm_machine.memory = malloc(MEM_SIZE);
  arm_machine.registers = calloc(REG_NUM, REG_BYTES);

  initialize(arm_machine);

  read_binary_file(argv[1], arm_machine.memory);

  Pipeline* pipeline = malloc(sizeof(Pipeline));
  execute_pipeline(arm_machine, pipeline);
  print_registers(arm_machine);
  print_memory(arm_machine);

  free(arm_machine.registers);
  free(arm_machine.memory);
  free(pipeline);

  return EXIT_SUCCESS;
}
