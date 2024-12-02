#include <stdlib.h>
#include "linked_list.h"

void add_charnode(CharNode* n, CharNode* init_node) {
  CharNode* curr = init_node;
  if (!curr->value) {
    *init_node = *n;
    return;
  }
  while (curr) {
    if (!curr->next) {
      curr->next = n;
      break;
    }
    curr = curr->next;
  }
}

void free_all(CharNode* init_node) {
  CharNode* curr = init_node;
  while (curr) {
    CharNode* tmp = curr;
    curr = curr->next;
    free(tmp);
  }
}