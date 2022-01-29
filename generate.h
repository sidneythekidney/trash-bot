#ifndef GENERATE_H
#define GENERATE_H

#include "defs.h"
#include "initialize.h"
#include <vector>

using namespace std;

class Generate {
    private:
        Initialize init;

    public:
        Generate(Initialize init) : init(init) {}
        
        U64 get_bishop_mask(U64 blockers, int sq);
        U64 get_rook_mask(U64 blockers, int sq);
        U64 get_knight_mask(U64 blockers, int sq);
        U64 get_king_mask(U64 blockers, int sq);
        U64 get_pawn_mask(int color, int sq);
};

#endif
