#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "defs.h"
#include "move.h"
#include <vector>

using namespace std;

void print_binary(U64 bitboard);

U64 pop_bit(U64 bitboard, int sq);

int get_ls1b(U64 mask);

int count_bits(U64 mask);

U64 board_to_U64(vector<vector<int>> vis);

vector<int> char_board_to_piece_board(vector<char> char_board);

#endif