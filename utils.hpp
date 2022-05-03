#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "defs.h"
#include "move.h"
#include <vector>
#include <time.h>

using namespace std;

void print_binary(U64 bitboard);

U64 pop_bit(U64 bitboard, int sq);

int get_ls1b(U64 mask);

int count_bits(U64 mask);

U64 board_to_U64(vector<vector<int>> vis);

vector<int> char_board_to_piece_board(vector<char> char_board);

void flip_chess_board(vector<int> &arr);

U64 flip_bitboard(U64 bitboard);

int get_square_from_algebraic(string alg);

string get_algebraic_from_square(int from, int to);

Move create_move(
    unsigned int from,
    unsigned int to,
    unsigned int moved_piece,
    unsigned int captured_piece,
    unsigned int legal_castles,
    unsigned int flags
);

#endif