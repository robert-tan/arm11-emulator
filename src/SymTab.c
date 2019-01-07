//
// Created by Blair S. on 11/06/18.
//

#include "SymTab.h"

/* implementation of symbol table */
sym_table_t* symbol_init() {
  sym_table_t* tab = malloc(sizeof(sym_table_t));
  tab->head = malloc(sizeof(node_t));
  return tab;
}

node_t* symbol_add_helper(node_t* cur, node_t* temp) {
  if (cur == NULL) {
    return temp;
  } else {
    if (cur == temp) {
      return NULL;
    } else {
      cur->next = symbol_add_helper(cur->next, temp);
      return cur;
    }
  }
}

int symbol_add(sym_table_t* symTab, char *name, void* addr) {
  node_t* temp = malloc(sizeof(node_t));
  node_t* com = symTab->head;
  temp->symbol.name = name;
  temp->symbol.address = addr;
  temp->next = NULL;
  if (com->symbol.name == NULL) {
    symTab->head = malloc(sizeof(node_t));
    symTab->head = temp;
    return 1;
  } else {
    symTab->head = symbol_add_helper(symTab->head, temp);
    if (symTab->head == NULL) {
      return 0;
    } else {
      return 1;
    }
  }
}

node_t* symbol_find_by_name (sym_table_t* symTab, char* name) {
  node_t* cur = symTab->head;
  while (cur != NULL) {
    if (cur->symbol.name != NULL) {
      if (!strcmp(cur->symbol.name, name)) {
        return cur;
      }
    }
    cur = cur->next;
  }
  return cur;
}
