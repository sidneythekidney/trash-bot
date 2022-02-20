#include "initialize.h"
#include "utils.hpp"
#include <cmath>
#include <iostream>

using namespace std;

U64 Initialize::set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
    U64 occ = 0ULL;
    for (int i = 0; i < bits_in_mask; ++i) {
        int sq = get_ls1b(attack_mask);
        attack_mask = pop_bit(attack_mask, sq);
        if (index & (1 << i)) {
            occ |= (1ULL << sq);
        }
    }
    return occ;
}

// Only used for initialization
U64 Initialize::slow_bishop_attacks(int sq, U64 blockers) {
    U64 attacks = 0ULL;
    int r, c, nr, nc;
    r = sq / 8;
    c = sq % 8;
    for (nr = r-1, nc = c-1; nr >= 0 && nc >= 0; --nr, --nc) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r-1, nc = c+1; nr >= 0 && nc <= 7; --nr, ++nc) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r+1, nc = c-1; nr <= 7 && nc >= 0; ++nr, --nc) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r+1, nc = c+1; nr <= 7 && nc <= 7; ++nr, ++nc) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    return attacks;
}

U64 Initialize::slow_rook_attacks(int sq, U64 blockers) {
    U64 attacks = 0ULL;
    int r, c, nr, nc;
    r = sq / 8;
    c = sq % 8;
    for (nr = r+1, nc = c; nr <= 7; ++nr) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r-1, nc = c; nr >= 0; --nr) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nc = c+1, nr = r; nc <= 7; ++nc) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nc = c-1, nr = r; nc >= 0; --nc) {
        attacks |= (1ULL << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    return attacks;
}

// Initialize sliding piece masks
void Initialize::init_slider_masks() {
    // Initialize bishop mask
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0ULL;
        int nr, nc, r, c;
        r = i / 8;
        c = i % 8;
        for(nr = r+1, nc = c+1; nr <= 6 && nc <= 6; ++nr, ++nc){
            mask |= (1ULL << (nr * 8 + nc));
        }
        for(nr = r-1,  nc = c+1; nr >= 1 && nc <= 6; --nr, ++nc) {
            mask |= (1ULL << (nr * 8 + nc));
        }
        for(nr = r+1,  nc = c-1; nr <= 6 && nc >= 1; ++nr, --nc) {
            mask |= (1ULL << (nr * 8 + nc));
        }
        for(nr = r-1,  nc = c-1; nr >= 1 && nc >= 1; --nr, --nc) {
            mask |= (1ULL << (nr * 8 + nc));
        }
        bishop_masks[i] = mask;
    }
    // Initialize rook masks:
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0ULL;
        int r, c, nr, nc;
        r = i / 8;
        c = i % 8;
        for(nr = r+1, nc = c; nr <= 6; ++nr) mask |= (1ULL << (nr * 8 + nc));
        for(nr = r-1, nc = c; nr >= 1; --nr) mask |= (1ULL << (nr * 8 + nc));
        for(nc = c+1, nr = r; nc <= 6; ++nc) mask |= (1ULL << (nr * 8 + nc));
        for(nc = c-1, nr = r; nc >= 1; --nc) mask |= (1ULL << (nr * 8 + nc));
        rook_masks[i] = mask;
    }
}

void Initialize::init_slider_attack_tables() {
    init_slider_masks();
    // Initialize bishop attacks
    for (int i = 0; i < 64; ++i) {
        // Initialize bishop mask:
        U64 attack_mask = bishop_masks[i];
        // Initialize occupancy indices
        U64 occ_ind = (1ULL << bishop_relevant_bits[i]);
        for (size_t j = 0; j < occ_ind; ++j) {
            // Calculate the blocker mask
            U64 occupancy = set_occupancy(j, bishop_relevant_bits[i], attack_mask);
            // Calculate the magic index based on this blocker mask
            U64 magic_ind = occupancy * BishopMagic[i] >> (64 - bishop_relevant_bits[i]);
            // Initalize bishop attacks:
            bishop_attacks[i][magic_ind] = slow_bishop_attacks(i, occupancy);
        }
    }
    // Initialize rook attacks
    for (int i = 0; i < 64; ++i) {
        // Initialize rook mask:
        U64 attack_mask = rook_masks[i];
        // Initialize occupancy indices
        U64 occ_ind = (1ULL << rook_relevant_bits[i]);
        for (size_t j = 0; j < occ_ind; ++j) {
            // Calculate the blocker mask
            U64 occupancy = set_occupancy(j, rook_relevant_bits[i], attack_mask);
            // Calculate the magic index based on this blocker mask
            U64 magic_ind = occupancy * RookMagic[i] >> (64 - rook_relevant_bits[i]);
            // Initalize rook attacks:
            rook_attacks[i][magic_ind] = slow_rook_attacks(i, occupancy);
        }
    }
}

void Initialize::init_knight_attack_table() {
    int r, c;
    for (int i = 0; i < 64; ++i) {
        U64 board = 0ULL;
        r = i / 8;
        c = i % 8;
        if (r+1 < 8 && c+2 < 8) {
            board |= (1ULL << ((r+1)*8 + (c+2)));
        }
        if (r+2 < 8 && c+1 < 8) {
            board |= (1ULL << ((r+2)*8 + (c+1)));
        }
        if (r+1 < 8 && c-2 >= 0) {
            board |= (1ULL << ((r+1)*8 + (c-2)));
        }
        if (r+2 < 8 && c-1 >= 0) {
            board |= (1ULL << ((r+2)*8 + (c-1)));
        }

        if (r-1 >= 0 && c+2 < 8) {
            board |= (1ULL << ((r-1)*8 + (c+2)));
        }
        if (r-2 >= 0 && c+1 < 8) {
            board |= (1ULL << ((r-2)*8 + (c+1)));
        }
        if (r-1 >= 0 && c-2 >= 0) {
            board |= (1ULL << ((r-1)*8 + (c-2)));
        }
        if (r-2 >= 0 && c-1 >= 0) {
            board |= (1ULL << ((r-2)*8 + (c-1)));
        }

        knight_attacks[i] = board;
    }
}

void Initialize::init_king_attack_table() {
    int r, c;
    for (int i = 0; i < 64; ++i) {
        U64 board = 0ULL;
        r = i / 8;
        c = i % 8;

        if (r+1 < 8 && c+1 < 8) {
            board |= (1ULL << ((r+1)*8 + c+1));
        }
        if (c+1 < 8) {
            board |= (1ULL << ((r)*8 + c+1));
        }
        if (r-1 >= 0 && c+1 < 8) {
            board |= (1ULL << ((r-1)*8 + c+1));
        }
        if (r-1 >= 0) {
            board |= (1ULL << ((r-1)*8 + c));
        }
        if (r-1 >= 0 && c-1 >= 0) {
            board |= (1ULL << ((r-1)*8 + c-1));
        }
        if (c-1 >= 0) {
            board |= (1ULL << ((r)*8 + c-1));
        }
        if (r+1 < 8 && c-1 >= 0) {
            board |= (1ULL << ((r+1)*8 + c-1));
        }
        if (r+1 < 8) {
            board |= (1ULL << ((r+1)*8 + c));
        }
        king_attacks[i] = board;
    }
}

void Initialize::init_pawn_attacks() {
    // 0 - white pawn attacks
    // 1- black pawn attacks

    // Calculate white sliding attacks
    for (int i = 0; i < 64; ++i) {
        // Check left:
        U64 initial = 0ULL;
        if (i % 8 != 0 && i / 8 != 0) {
            initial |= (1ULL << (i-9)); 
        }
        // Check right
        if (i % 8 != 7 && i / 8 != 0) {
            initial |= (1ULL << (i-7)); 
        }
        pawn_attacks[0][i] = initial;
    }

    // Calculate black sliding attacks
    for (int i = 0; i < 64; ++i) {
        // Check left:
        U64 initial = 0ULL;
        if (i % 8 != 0 && i / 8 != 7) {
            initial |= (1ULL << (i+7)); 
        }
        // Check right
        if (i % 8 != 7 && i / 8 != 7) {
            initial |= (1ULL << (i+9)); 
        }
        pawn_attacks[1][i] = initial;
    }
}

void Initialize::init_all() {
    cout << "init slider attack tables\n";
    init_slider_attack_tables();
    cout << "init knight attack tables\n";
    init_knight_attack_table();
    cout << "init king attack tables\n";
    init_king_attack_table();
    cout << "init pawn attack tables\n";
    init_pawn_attacks();
}

// Initialize::Initialize() {
//     init_all();
// }