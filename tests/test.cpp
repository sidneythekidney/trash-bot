// This file is for testing snippets of code
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <cmath>
#include <vector>
// #include "../utils.hpp"
#include "../defs.h"

using namespace std;

// int countBits(U64 mask) {
//     return (int)log2(mask) + 1;
// }

// void hex_to_b(U64 hex_num) {
//     for (int i = 0; i < 64; ++i) {
//         if (i % 8 == 0) {
//             cout << "\n";
//         }
//         cout << (hex_num & 1);
//         hex_num = hex_num >> 1;
//     }
//     cout << "\n";
// }

// count bits (Brian Kernighan's way)
int count_bits(U64 bitboard) {
  // bit count
  int count = 0;
  
  // pop bits untill bitboard is empty
  while (bitboard)
  {
      // increment count
      count++;
      
      // consecutively reset least significant 1st bit
      bitboard &= bitboard - 1;
  }
  
  // return bit count
  return count;
}

// get index of LS1B in bitboard
int get_ls1b_index(U64 bitboard) {
    // make sure bitboard is not empty
    if (bitboard != 0)
        // convert trailing zeros before LS1B to ones and count them
        return count_bits((bitboard & -bitboard) - 1);
    
    // otherwise
    else
        // return illegal index
        return -1;
}

int get_ls1b(U64 mask) {
    int pos = 0;
    // cout << "mask: " << mask << "\n";

    while (mask != 0ULL) {
        if (mask & 1) {
            return pos;
        }
        mask >>= 1;
        ++pos;
    }
    return -1;
}

int main(){
    // vector<vector<int>> var = {
    //     {0,1,1,0,1,0,0,0},
    //     {0,1,1,0,1,0,0,0},
    //     {1,1,1,0,0,1,1,0},
    //     {1,1,1,0,0,1,1,0},
    //     {1,1,1,0,0,1,1,0},
    //     {1,1,1,0,0,1,1,0},
    //     {1,1,1,0,0,1,1,0},
    //     {1,1,1,0,0,1,1,0},
    // };

    // U64 converted = board_to_U64(var);
    // print_binary(converted);
    // vector<vector<int>> correct_no_blockers = {
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,1,0,0},
    //     {0,0,1,0,0,0,1,0},
    //     {0,0,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,1,0},
    //     {0,0,0,1,0,1,0,0},
    //     {0,0,0,0,0,0,0,0},
    // }; 

    // U64 mask = 0x00000000000000010ULL;

    // cout << "get_ls1b(mask): " << get_ls1b(mask) << "\n";
    // cout << "get_ls1b_index(mask): " << get_ls1b_index(mask) << "\n";
    auto start = clock();
    int delay_in_seconds = 5;
    cout << start << "\n";
    while (clock() < (start + delay_in_seconds * CLOCKS_PER_SEC)) {
        
    }
    cout << "Finished\n";

}