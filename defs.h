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

// Define blocker masks used when calcuclating castles:
U64 white_ks_blockers = 7ULL << 60;
U64 white_qs_blockers = 7ULL << 58;
U64 black_ks_blockers = 7ULL << 4;
U64 black_qs_blockers = 7ULL << 2;

#endif
