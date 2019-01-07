//
// Created by Blair S. on 2018/5/27.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>


#ifndef ARM11_32_SYMTAB_H
#define ARM11_32_SYMTAB_H

typedef struct sym_table sym_table_t;

// address can be any type
typedef struct symbol {
    char* name;
    void* address;
} symbol_t;

typedef struct node {
    symbol_t symbol;
    struct node* next;
} node_t;

struct sym_table {
  node_t* head;
};

// create a table with size
sym_table_t* symbol_init ();

// add the symbol with name and addr to the SymbolTable symTab
int symbol_add (sym_table_t* symTab, char* name, void* addr);

// give the pointer of the node with given name in the symTab
node_t* symbol_find_by_name (sym_table_t* symTab, char* name);

#endif //ARM11_32_SYMTAB_H

