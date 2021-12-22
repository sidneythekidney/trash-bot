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

// uint8_t countBits(U64 mask) {
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
    vector<vector<int>> correct_no_blockers = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,1,0,1,0,0},
        {0,0,1,0,0,0,1,0},
        {0,0,0,0,0,0,0,0},
        {0,0,1,0,0,0,1,0},
        {0,0,0,1,0,1,0,0},
        {0,0,0,0,0,0,0,0},
    }; 

}