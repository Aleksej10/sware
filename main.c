#include "pos.h"
#include "node.h"
#include "stdio.h"

#include <stdlib.h>
#include <string.h>

/* 2^32 */
#define HASH_SIZE 4294967296

Node* stack_end = NULL;
unsigned char* HASH;

int main(){
  HASH = malloc(HASH_SIZE * sizeof(unsigned char));
  if(HASH == NULL){ printf("need 4GB of RAM\n"); fflush(stdout); exit(1); }

  printf("initializing hash... "); fflush(stdout);
  memset(HASH, 255, HASH_SIZE * sizeof(unsigned char));
  printf("ok\n"); fflush(stdout);

  Node* n = Stack_init();

  while(true){
    if(n->duplicate){
      n = n->next;
      continue;
    }

    Array moves = Pos_gen_moves(n->pawns);

    for(unsigned i=0; i<moves.size; i++)
      Stack_add(n, moves.data[i]);

    n = n->next;
  }

  return 0;
};
