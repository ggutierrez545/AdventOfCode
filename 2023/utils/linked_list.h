#ifndef ADVENTOFCODE2023_LINKED_LIST_H
#define ADVENTOFCODE2023_LINKED_LIST_H

typedef struct {
  char* value;
  void* next;
} CharNode;

void add_charnode(CharNode* n, CharNode* init_node);

void free_all(CharNode* init_node);

#endif //ADVENTOFCODE2023_LINKED_LIST_H
