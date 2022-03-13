#include "node.h"

#include <stdlib.h>
#include <stdio.h>

extern Node* stack_end;
extern unsigned char* HASH;

Node*
Stack_init(){
  Node* n = malloc(sizeof(Node));
  if(n == NULL){ printf("can't allocate initial node\n"); fflush(stdout); exit(1); }

  stack_end = n;
  n->father = NULL;

  n->m = Move_null;
  n->pawns = Pos_new;
  n->dist = 0;
  n->duplicate = false;

  return n;
};

void
Stack_add(Node* father, Move m){
  Node* n = malloc(sizeof(Node));
  if(n == NULL){ printf("can't allocate new node\n"); fflush(stdout); exit(1); }

  n->father = father;

  stack_end->next = n;
  stack_end = n;

  n->m = m;

  n->pawns = Pos_play_move(father->pawns, m);

  if(n->pawns == goal) Node_declare_winner(n);

  n->dist = father->dist + 1;
  n->duplicate = false;
  
  uint32_t i = Pos_hash(n->pawns);

  if(HASH[i] <= n->dist)
    n->duplicate = true;
  else
    HASH[i] = n->dist;
};


void
Node_declare_winner(Node* self){
  Pos_print(self->pawns);

  if(self->father == NULL){
    fflush(stdout);
    exit(0);
  }
  else{
    Move_print(self->m); printf("\n"); 
    Node_declare_winner(self->father);
  } 
};
