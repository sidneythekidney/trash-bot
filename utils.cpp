#include <iostream>
#include <vector>
#include "utils.hpp"

using namespace std;

void print_binary(U64 bitboard) {
    // Print the binary version of num (used for debugging)
    for (int i = 0; i < 64; ++i) {
        if (i % 8 == 0) {
            cout << "\n";
        }
        cout << (bitboard & 1) << ' ';
        bitboard >>= 1;
    }
    cout << "\n";
}

U64 pop_bit(U64 bitboard, int sq) {
    U64 bb = bitboard;
    // Pop the bit at the given square if it exists
    if (bb & (1ULL << sq)) {
        bb ^= (1ULL << sq);
    }
    else {
        bb = 0ULL;
    }
    return bb;
}

int get_ls1b(U64 mask) {
    int pos = 0;
    while (mask) {
        if (mask & 1) {
            return pos;
        }
        mask >>= 1;
        ++pos;
    }
    return -1;
}

int count_bits(U64 mask) {
    int count = 0;
    while (mask) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}
