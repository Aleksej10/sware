#ifndef NODE_H
#define NODE_H

#include "pos.h"
#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
  struct Node* next;
  struct Node* father;

  uint64_t pawns;
  Move m;
  unsigned dist;
  bool duplicate;
} Node;

void Stack_add(Node*, Move);
Node* Stack_init();
void Node_declare_winner(Node*);

#endif
