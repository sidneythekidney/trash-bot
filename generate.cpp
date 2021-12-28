#include "defs.h"
#include "generate.h"
#include "utils.hpp"

// Function to get the bishop mask given blockers
U64 Generate::getBishopMask(U64 blockers, int sq) {
    blockers &= init.bishop_masks[sq];
    U64 ind = (blockers * init.BishopMagic[sq]) >> (64 - init.bishop_relevant_bits[sq]);
    return init.bishop_attacks[sq][ind];
}

// Function to get the rook mask given blockers
U64 Generate::getRookMask(U64 blockers, int sq) {
    blockers &= init.rook_masks[sq];
    U64 ind = (blockers * init.RookMagic[sq]) >> (64 - init.rook_relevant_bits[sq]);
    return init.rook_attacks[sq][ind];
}

// Function to get the knight mask given blockers
U64 Generate::getKnightMask(U64 blockers, int sq) {
    blockers &= init.knight_attacks[sq];
    return blockers ^ init.knight_attacks[sq];
}

// Function to get the king mask given blockers
U64 Generate::getKingMask(U64 blockers, int sq) {
    blockers &= init.king_attacks[sq];
    return blockers ^ init.king_attacks[sq];
}
