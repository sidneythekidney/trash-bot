#include <stdint.h>

#ifndef DEFS_H
#define DEFS_H

typedef unsigned long long U64;

enum color {
    WHITE, 
    BLACK
};

enum pieces {
    KING,
    QUEEN,
    ROOK,
    BISHOP,
    KNIGHT,
    PAWN
};

enum castle {
    NONE = 0,
    KING_SIDE = 1,
    QUEEN_SIDE = 2
};

#endif
