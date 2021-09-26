#include "initialize.h"
#include <cmath>
#include <iostream>

using namespace std;

void print_binary(U64 num) {
    for (int i = 0; i < 64; ++i) {
        if (i % 8 == 0) {
            cout << "\n";
        }
        cout << (num & 1) << ' ';
        num >>= 1;
    }
    cout << "\n";
}

void pop_bit(U64 &bitboard, uint8_t sq) {
    if (bitboard & (1 << sq)) {
        bitboard ^= (1 << sq);
    }
}

uint8_t get_ls1b(U64 bitboard) {
    uint8_t pos = 0;
    while(!bitboard & 1) {
        bitboard >>= 1;
        ++pos;
    }
    return pos;
}

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask) {
    U64 occ = 0ULL;
    for (int i = 0; i < bits_in_mask; ++i) {
        uint8_t sq = get_ls1b(attack_mask);
        pop_bit(attack_mask, sq);
        // Is the square actually occupied in this variation?
        if (index & (1 << i)) {
            occ |= (1ULL << sq);
        }
    }
    return occ;
}

// Only used for initialization
U64 slow_bishop_attacks(uint8_t sq, U64 blockers) {
    U64 attacks = 0ULL;
    int r, c, nr, nc;
    r = sq / 8;
    c = sq % 8;
    for (nr = r-1, nc = c-1; nr >= 0 && nc >= 0; --nr, --nc) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r-1, nc = c+1; nr >= 0 && nc <= 7; --nr, ++nc) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r+1, nc = c-1; nr <= 7 && nc >= 0; ++nr, --nc) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r+1, nc = c+1; nr <= 0 && nc <= 7; ++nr, ++nc) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    return attacks;
}

U64 slow_rook_attacks(uint8_t sq, U64 blockers) {
    U64 attacks = 0ULL;
    int r, c, nr, nc;
    r = sq / 8;
    c = sq % 8;
    for (nr = r+1, nc = c; nr <= 7; ++nr) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nr = r-1, nc = c; nr >= 0; --nr) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nc = c+1, nr = r; nc <= 7; ++nc) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    for (nc = r-1, nr = r; nc >= 0; --nc) {
        attacks |= (1 << (nr * 8 + nc));
        if (blockers & (1ULL << (nr * 8 + nc))) {
            break;
        }
    }
    return attacks;
}

int countBits(U64 mask) {
    // Count the number of bits in the mask
    int num_bits = 0;
    for (int i = 0; i < 64; ++i) {
        if ((1ULL << i) & mask) {
            ++num_bits;
        }
    }
    return num_bits;
}

// Initialize sliding piece masks
void init_slider_masks() {
    // Initialize bishop mask
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0ULL;
        int r, c, nr, nc;
        r = i / 8;
        c = i % 8;
        for(nr = r+1,  nc = c+1; nr <= 6 && nc <= 6; ++nr, ++nc) mask |= (1ULL << (nr * 8 + nc));
        for(nr = r-1,  nc = c+1; nr >= 1 && nc <= 6; --nr, ++nc) mask |= (1ULL << (nr * 8 + nc));
        for(nr = r+1,  nc = c-1; nr <= 6 && nc >= 1; ++nr, --nc) mask |= (1ULL << (nr * 8 + nc));
        for(nr = r-1,  nc = c-1; nr >= 1 && nc >= 1; --nr, --nc) mask |= (1ULL << (nr * 8 + nc));
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

void init_slider_attack_tables() {
    init_slider_masks();
    for (int i = 0; i < 64; ++i) {
        // Initialize bishop mask:
        U64 attack_mask = bishop_masks[i];
        // Initialize bit count
        size_t rel_bits = countBits(attack_mask);
        // Initialize occupancy indices
        U64 occ_ind = (1ULL << rel_bits);
        for (size_t j = 0; j < occ_ind; ++j) {
            U64 occupancy = set_occupancy(j, rel_bits, attack_mask);
            U64 magic_ind = (occupancy * BishopMagic[i]) >> (64 - bishop_relevant_bits[i]);
            // Initalize bishop attacks:
            bishop_attacks[i][magic_ind] = slow_bishop_attacks(i, occupancy);
        }
    }
    for (int i = 0; i < 64; ++i) {
        // Initialize rook mask:
        U64 attack_mask = rook_masks[i];
        // Initialize bit count
        size_t rel_bits = countBits(attack_mask);
        // Initialize occupancy indices
        U64 occ_ind = (1 << rel_bits);
        for (size_t j = 0; j < occ_ind; ++j) {
            U64 occupancy = set_occupancy(j, rel_bits, attack_mask);
            U64 magic_ind = (occupancy * BishopMagic[i]) >> (64 - rook_relevant_bits[i]);
            // Initalize rook attacks:
            rook_attacks[i][magic_ind] = slow_rook_attacks(i, occupancy);
        }
    }
}

void init_knight_attack_table() {
    uint8_t r, c;
    for (uint8_t i = 0; i < 64; ++i) {
        U64 board = 0ULL;
        r = i / 8;
        c = i % 8;
        if (r+1 < 8 && c+2 < 8) {
            board |= (1 << ((r+1)*8 + (c+2)));
        }
        if (r+2 < 8 && c+1 < 8) {
            board |= (1 << ((r+2)*8 + (c+1)));
        }
        if (r+1 < 8 && c-2 >= 0) {
            board |= (1 << ((r+1)*8 + (c-2)));
        }
        if (r+2 < 8 && c-1 >= 0) {
            board |= (1 << ((r+2)*8 + (c-2)));
        }

        if (r-1 >= 0 && c+2 < 8) {
            board |= (1 << ((r-1)*8 + (c+2)));
        }
        if (r-2 >= 0 && c+1 < 8) {
            board |= (1 << ((r-2)*8 + (c+1)));
        }
        if (r-1 >= 0 && c-2 >= 0) {
            board |= (1 << ((r-1)*8 + (c-2)));
        }
        if (r-2 >= 0 && c-1 >= 0) {
            board |= (1 << ((r-2)*8 + (c-2)));
        }

        knight_attacks[i] = board;
    }
}

void init_king_attack_table() {
    uint8_t r, c;
    for (int i = 0; i < 64; ++i) {
        U64 board = 0ULL;
        r = i / 8;
        c = i % 8;

        if (r+1 < 8 && c+1 < 8) {
            board |= (1 << ((r+1)*8 + c+1));
        }
        if (c+1 < 8) {
            board |= (1 << ((r)*8 + c+1));
        }
        if (r-1 >= 0 && c+1 < 8) {
            board |= (1 << ((r-1)*8 + c+1));
        }
        if (r-1 >= 0) {
            board |= (1 << ((r-1)*8 + c));
        }
        if (r-1 >= 0 && c-1 >= 0) {
            board |= (1 << ((r-1)*8 + c-1));
        }
        if (c-1 >= 0) {
            board |= (1 << ((r)*8 + c-1));
        }
        if (r+1 < 8 && c-1 >= 0) {
            board |= (1 << ((r+1)*8 + c-1));
        }
        if (r+1 < 8) {
            board |= (1 << ((r+1)*8 + c));
        }
        king_attacks[i] = board;
    }
}

void init_pawn_attacks() {
    // 0 - white pawn attacks
    // 1- black pawn attacks

    // Calculate white sliding attacks
    for (int i = 0; i < 64; ++i) {
        // Check left:
        U64 initial = 0ULL;
        if (i % 8 != 0 && i / 8 != 7) {
            initial |= (1 << (i+7)); 
        }
        if (i % 8 != 7 && i / 8 != 7) {
            initial |= (1 << (i+9)); 
        }
        pawn_attacks[0][i] = initial;
    }

    // Calculate blak sliding attacks
    for (int i = 0; i < 64; ++i) {
        // Check left:
        U64 initial = 0ULL;
        if (i % 8 != 0 && i / 8 != 0) {
            initial |= (1 << (i-7)); 
        }
        if (i % 8 != 7 && i / 8 != 0) {
            initial |= (1 << (i-9)); 
        }
        pawn_attacks[1][i] = initial;
    }
}

void init_all() {
    cout << "init slider attack tables\n";
    init_slider_attack_tables();
    cout << "init knight attack tables\n";
    init_knight_attack_table();
    cout << "init king attack tables\n";
    init_king_attack_table();
    cout << "init pawn attack tables\n";
    init_pawn_attacks();
}

int main() {
    init_all();
}
