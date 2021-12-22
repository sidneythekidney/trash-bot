// Generate unobstructed rook and bishop moves
#include <iostream>
#include "defs.h"
#include "utils.hpp"

using namespace std;

int8_t mailbox[120] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
    -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
    -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
    -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
    -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
    -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
    -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
    -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
}; // 120 bytes;

int8_t mailbox64[64] = {
    21, 22, 23, 24, 25, 26, 27, 28,
    31, 32, 33, 34, 35, 36, 37, 38,
    41, 42, 43, 44, 45, 46, 47, 48,
    51, 52, 53, 54, 55, 56, 57, 58,
    61, 62, 63, 64, 65, 66, 67, 68,
    71, 72, 73, 74, 75, 76, 77, 78,
    81, 82, 83, 84, 85, 86, 87, 88,
    91, 92, 93, 94, 95, 96, 97, 98
}; // 64 bytes;

void hex_to_binary(U64 hex_num) {
    for (int i = 0; i < 64; ++i) {
        if (i % 8 == 0) {
            cout << "\n";
        }
        cout << (hex_num & 1);
        hex_num = hex_num >> 1;
    }
    cout << "\n";
}

int main() {
    // Bishop Moves
    int bishop_offset[4] = {-11, -9, 9, 11};
    // cout << "{";
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0;
        // Check each direction
        for (int j = 0; j < 4; ++j) {
            for (int k = i;;) {
                k = mailbox[mailbox64[k] + bishop_offset[j]];
                if (k == -1) {
                    break;
                }
                mask |= (1ULL << k);
                // hex_to_binary(mask);
            }
        }
        // hex_to_binary(mask);
        // cout << "0x";
        // cout << hex << mask;
        // cout << ",\n";
    }
    // cout << "};\n";
    // Rook moves
    int rook_offset[4] = {-10,-1,1,10};
    // cout << "{";
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0;
        // Check each direction
        for (int j = 0; j < 4; ++j) {
            for (int k = i;;) {
                k = mailbox[mailbox64[k] + rook_offset[j]];
                if (k == -1) {
                    break;
                }
                mask |= (1ULL << k);
            }
        }
        // hex_to_binary(mask);
        // cout << "0x";
        // cout << hex << mask;
        // cout << ",\n "; 
    }
    // cout << "};\n";
}