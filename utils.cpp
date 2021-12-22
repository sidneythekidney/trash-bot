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

void pop_bit(U64 bitboard, uint8_t sq) {
    // Pop the bit at the given square if it exists
    if (bitboard & (1ULL << sq)) {
        bitboard ^= (1ULL << sq);
    }
}

uint8_t get_ls1b(U64 bitboard) {
    // Get the 
    uint8_t pos = 0;
    while(!bitboard & 1) {
        bitboard >>= 1;
        ++pos;
    }
    return pos;
}
