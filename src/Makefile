CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o Instruction.o SymTab.o

emulate: emulate.o file_utils.o emulate_utils.o emulate_state_utils.o emulate_instr_processing.o emulate_branch.o emulate_sdt.o emulate_datap.o emulate_mult.o

emulate.o: emulate.h file_utils.h emulate_utils.h emulate_state_utils.h emulate_instr_processing.h emulate_branch.h emulate_sdt.h emulate_datap.h emulate_mult.h

emulate_instr_processing.o: emulate_instr_processing.h emulate.h emulate_utils.h emulate_state_utils.h emulate_branch.h emulate_sdt.h emulate_datap.h emulate_mult.h

emulate_branch.o: emulate_branch.h emulate.h emulate_utils.h

emulate_sdt.o: emulate_sdt.h emulate.h emulate_utils.h emulate_state_utils.h

emulate_datap.o: emulate_datap.h emulate.h emulate_utils.h emulate_state_utils.h

emulate_mult.o: emulate_mult.h emulate_datap.h emulate.h emulate_utils.h emulate_state_utils.h

emulate_state_utils.o: emulate_state_utils.h emulate.h emulate_utils.h

emulate_utils.o: emulate_utils.h

file_utils.o: file_utils.h

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
