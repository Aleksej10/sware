#ifndef POS_H
#define POS_H

#include <stdint.h>

#define MAX_SIZE 32

/* bit masks */
#define first_1     0b0000000010101010010101011010101001010101101010100101010110101010
#define first_2     0b0101010110101010000000000000000000000000000000000000000000000000
#define n_first_2   0b0000000000000000010101011010101001010101101010100101010110101010

#define legal_mask  0b0101010110101010010101011010101001010101101010100101010110101010
#define start       0b0000000000000000000000000000000000000000000000000101010110101010
#define goal        0b0101010110101010000000000000000000000000000000000000000000000000

/* arithmetics */
#define UP_RIGHT(P)     (P<<7)
#define UP_LEFT(P)      (P<<9)
#define UP_RIGHT_2(P)   (P<<14)
#define UP_LEFT_2(P)    (P<<18)

#define DOWN_LEFT(P)    (P>>7)
#define DOWN_RIGH(P)    (P>>9)
#define DOWN_LEFT_2(P)  (P>>14)
#define DOWN_RIGH_2(P)  (P>>18)
#define INVERT(P)       (~P & legal_mask)

#define IS_NULL_MOVE(M) (M.from_square == 3)

typedef struct {
  uint64_t from_square;
  uint64_t to_square;
} Move;

#define Move_new(X, Y) (Move){ X, Y }
#define Move_null (Move){ 3, 3 }
#define Pos_new start

void Pos_print(const uint64_t);
uint64_t Pos_play_move(uint64_t, const Move);
uint32_t Pos_hash(uint64_t);

void Move_print(const Move);

typedef struct {
  Move data[MAX_SIZE];
  unsigned size;
} Array;

#define Array_new (Array){ .size = 0 }

Array Array_merge(Array*, const unsigned);
void Array_add(Array*, const Move);
void Array_print(const Array*);

Array Pos_gen_moves(const uint64_t);
Array Pos_silent_moves(const uint64_t);
Array Pos_captures(const uint64_t, const uint64_t, const uint64_t);

#endif
