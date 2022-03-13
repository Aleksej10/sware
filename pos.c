#include "pos.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

const uint64_t squares[32] = {
  0b0000000000000000000000000000000000000000000000000000000000000010,
  0b0000000000000000000000000000000000000000000000000000000000001000,
  0b0000000000000000000000000000000000000000000000000000000000100000,
  0b0000000000000000000000000000000000000000000000000000000010000000,
  0b0000000000000000000000000000000000000000000000000000000100000000,
  0b0000000000000000000000000000000000000000000000000000010000000000,
  0b0000000000000000000000000000000000000000000000000001000000000000,
  0b0000000000000000000000000000000000000000000000000100000000000000,
  0b0000000000000000000000000000000000000000000000100000000000000000,
  0b0000000000000000000000000000000000000000000010000000000000000000,
  0b0000000000000000000000000000000000000000001000000000000000000000,
  0b0000000000000000000000000000000000000000100000000000000000000000,
  0b0000000000000000000000000000000000000001000000000000000000000000,
  0b0000000000000000000000000000000000000100000000000000000000000000,
  0b0000000000000000000000000000000000010000000000000000000000000000,
  0b0000000000000000000000000000000001000000000000000000000000000000,
  0b0000000000000000000000000000001000000000000000000000000000000000,
  0b0000000000000000000000000000100000000000000000000000000000000000,
  0b0000000000000000000000000010000000000000000000000000000000000000,
  0b0000000000000000000000001000000000000000000000000000000000000000,
  0b0000000000000000000000010000000000000000000000000000000000000000,
  0b0000000000000000000001000000000000000000000000000000000000000000,
  0b0000000000000000000100000000000000000000000000000000000000000000,
  0b0000000000000000010000000000000000000000000000000000000000000000,
  0b0000000000000010000000000000000000000000000000000000000000000000,
  0b0000000000001000000000000000000000000000000000000000000000000000,
  0b0000000000100000000000000000000000000000000000000000000000000000,
  0b0000000010000000000000000000000000000000000000000000000000000000,
  0b0000000100000000000000000000000000000000000000000000000000000000,
  0b0000010000000000000000000000000000000000000000000000000000000000,
  0b0001000000000000000000000000000000000000000000000000000000000000,
  0b0100000000000000000000000000000000000000000000000000000000000000
};

const char squares_map[32][3] = { 
  "g1", "e1", "c1", "a1", 
  "h2", "f2", "d2", "b2", 
  "g3", "e3", "c3", "a3", 
  "h4", "f4", "d4", "b4",
  "g5", "e5", "c5", "a5",
  "h6", "f6", "d6", "b6", 
  "g7", "e7", "c7", "a7", 
  "h8", "f8", "d8", "b8" 
};

unsigned 
pawn_to_index(uint64_t pawn){
  uint64_t lg = legal_mask;
  unsigned i = 0;

  while(pawn > 0) {
    if(pawn == 1) return i;
    if((lg & 1) == 1) i++;
    lg >>= 1;
    pawn >>= 1;
  }

  return i;
};

void 
Pos_print(const uint64_t self){
  for(unsigned i=0; i<8; i++){
    printf("+---+---+---+---+---+---+---+---+\n");
    for(unsigned k=0; k<8; k++){
      printf("| ");

      const uint64_t mask = (uint64_t)1 << (63-(8*i + k));

      if ((mask & self) > 0)  
        printf("\033[1;44;94m#\033[0m ");
      else                    
        printf("  ");
      
    }
    printf("|\n");
  }
  printf("+---+---+---+---+---+---+---+---+\n");
  fflush(stdout);
};

uint64_t
Pos_play_move(uint64_t self, Move m){
  self  |=  m.to_square;
  self  &= ~m.from_square;
  return self;
};

uint32_t
Pos_hash(uint64_t self){
  uint64_t lg = legal_mask;
  unsigned mask = 1;
  uint32_t key = 0;

  while(lg > 0){
    if((lg & 1) == 1){
      if((self & 1) == 1) key |= mask;
      mask <<= 1;
    }

    lg >>= 1;
    self >>= 1;
  }

  return key;
};

void
Move_print(const Move self){
  printf("%s-%s",
    squares_map[pawn_to_index(self.from_square)],
    squares_map[pawn_to_index(self.to_square)]
  );
};

void
Array_add(Array* self, const Move m){
  self->data[self->size++] = m;
};

void 
Array_print(const Array* self){
  printf("[");

  for(unsigned i=0; i<self->size-1; i++){
    Move_print(self->data[i]);
    printf(", ");
  }

  Move_print(self->data[self->size-1]);
  printf("]\n");
}

Array
Array_merge(Array* arrays, const unsigned n){
  Array new_array = Array_new;

  for(unsigned i=0; i<n; i++){
    if(arrays[i].size == 0) continue;

    memcpy(&new_array.data[new_array.size], arrays[i].data, arrays[i].size * sizeof(Move));
    new_array.size += arrays[i].size;
  }

  return new_array;
};

Array
Pos_silent_moves(uint64_t pawns){
  Array moves = Array_new;

  const uint64_t silent_1 = first_1 & pawns & DOWN_LEFT(INVERT(pawns));
  const uint64_t silent_2 = first_1 & pawns & DOWN_RIGH(INVERT(pawns));

  /* TODO; optimize to look at most 8 squares */
  uint64_t s;
  for(unsigned i=0; i<32; i++){
    s = squares[i];

    if((silent_1 & s) != 0)
      Array_add(&moves, Move_new(s, UP_RIGHT(s)));

    if((silent_2 & s) != 0)
      Array_add(&moves, Move_new(s, UP_LEFT(s)));
  }

  return moves;
};

Array
Pos_captures(const uint64_t pawns, const uint64_t capture, const uint64_t from){
  if((capture & first_2) != 0) return Array_new;

  const uint64_t c1 = capture & pawns & DOWN_LEFT(pawns) & DOWN_LEFT_2(INVERT(pawns));
  const uint64_t c2 = capture & pawns & DOWN_RIGH(pawns) & DOWN_RIGH_2(INVERT(pawns));

  unsigned i = 0;
  Array cs[2];

  if(c1 != 0){
    const uint64_t c = UP_RIGHT_2(capture);
    Array future_c1 = Pos_captures(Pos_play_move(pawns, Move_new(capture, c)), c, from);
    Array_add(&future_c1, Move_new(from, c));

    cs[i++] = future_c1;
  }

  if(c2 != 0){
    const uint64_t c = UP_LEFT_2(capture);
    Array future_c2 = Pos_captures(Pos_play_move(pawns, Move_new(capture, c)), c, from);
    Array_add(&future_c2, Move_new(from, c));

    cs[i++] = future_c2;
  }

  return Array_merge(cs, i);
};

Array
Pos_gen_moves(const uint64_t pawns){
  const uint64_t c1 = pawns & DOWN_LEFT(pawns) & DOWN_LEFT_2(INVERT(pawns));
  const uint64_t c2 = pawns & DOWN_RIGH(pawns) & DOWN_RIGH_2(INVERT(pawns));

  const uint64_t captures = n_first_2 & (c1 | c2);

  Array cs[9];
  unsigned captures_n = 0;
  uint64_t s;
  /* TODO; optimize to look at most 8 squares */
  for(unsigned i=0; i<32; i++){
    s = squares[i];

    if((captures & s) != 0){
      cs[captures_n++] = Pos_captures(pawns, s, s);
    }
  }
  cs[captures_n++] = Pos_silent_moves(pawns);

  return Array_merge(cs, captures_n);
}
