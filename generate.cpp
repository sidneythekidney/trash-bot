#include "generate.h"

U64 Generate::getBishopAttacks(U64 blockers, uint8_t sq) {
    blockers &= bishop_masks[sq];
    U64 ind = (blockers * BishopMagic[sq]) >> (64 - bishop_relevant_bits[sq]);
    return bishop_attacks[sq][ind];
}

U64 Generate::getRookAttacks(U64 blockers, uint8_t sq) {
    blockers &= rook_masks[sq];
    U64 ind = (blockers * RookMagic[sq]) >> (64 - rook_relevant_bits[sq]);
    return rook_attacks[sq][ind];
}
