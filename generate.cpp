#include "generate.h"

// Function to get the bishop mask given blockers
U64 Generate::get_bishop_mask(U64 blockers, int sq) {
    blockers &= init.bishop_masks[sq];
    U64 ind = (blockers * init.BishopMagic[sq]) >> (64 - init.bishop_relevant_bits[sq]);
    return init.bishop_attacks[sq][ind];
}

// Function to get the rook mask given blockers
U64 Generate::get_rook_mask(U64 blockers, int sq) {
    blockers &= init.rook_masks[sq];
    U64 ind = (blockers * init.RookMagic[sq]) >> (64 - init.rook_relevant_bits[sq]);
    return init.rook_attacks[sq][ind];
}

// Function to get the knight mask given blockers
U64 Generate::get_knight_mask(U64 blockers, int sq) {
    blockers &= init.knight_attacks[sq];
    return blockers ^ init.knight_attacks[sq];
}

// Function to get the king mask given blockers
U64 Generate::get_king_mask(U64 blockers, int sq) {
    blockers &= init.king_attacks[sq];
    return blockers ^ init.king_attacks[sq];
}

U64 Generate::get_pawn_mask(int color, int sq) {
    return init.pawn_attacks[color][sq];
}
