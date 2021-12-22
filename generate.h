#ifndef GENERATE_H
#define GENERATE_H

#include "defs.h"
#include "initialize.h"

// extern U64 pawn_attacks[2][64];
// extern U64 knight_attacks[64];
// extern U64 king_attacks[64];
// extern U64 bishop_masks[64];
// extern U64 rook_masks[64]; 
// extern U64 bishop_attacks[64][512];
// extern U64 rook_attacks[64][4096];

// extern U64 blockers[2];

class Generate {
    private:
        Initialize init;
    public:
        Generate(Initialize init) : init(init) { };

        U64 getBishopMask(U64 blockers, uint8_t sq);
        U64 getRookMask(U64 blockers, uint8_t sq);
        U64 getKnightMask(U64 blockers, uint8_t sq);
        U64 getKingMask(U64 blockers, uint8_t sq);
};

#endif
