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
    if (mask) {
        return count_bits((mask & -mask) - 1);
    }
    else {
        return -1;
    }
}

int count_bits(U64 mask) {
    int count = 0;
    while (mask) {
        count += mask & 1;
        mask >>= 1;
    }
    return count;
}

U64 board_to_U64(vector<vector<int>> vis) {
    // This function is used for testing
    // Allows conversion from visual 2D vector to bitset:
    U64 ret = 0ULL;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            if (vis[r][c]) {
                ret += 1ULL << (r * 8 + c);
            }
        }
    }
    return ret;
}

vector<int> char_board_to_piece_board(vector<char> char_board) {    
    vector<int> piece_board = {};
    for (int i = 0; i < (int)char_board.size(); ++i) {

        if (char_board[i] == 'p') piece_board.push_back(Move::WHITE_PAWN);
        else if (char_board[i] == 'n') piece_board.push_back(Move::WHITE_KNIGHT);
        else if (char_board[i] == 'b') piece_board.push_back(Move::WHITE_BISHOP);
        else if (char_board[i] == 'r') piece_board.push_back(Move::WHITE_ROOK);
        else if (char_board[i] == 'q') piece_board.push_back(Move::WHITE_QUEEN);
        else if (char_board[i] == 'k') piece_board.push_back(Move::WHITE_KING);
        else if (char_board[i] == 'P') piece_board.push_back(Move::BLACK_PAWN);
        else if (char_board[i] == 'N') piece_board.push_back(Move::BLACK_KNIGHT);
        else if (char_board[i] == 'B') piece_board.push_back(Move::BLACK_BISHOP);
        else if (char_board[i] == 'R') piece_board.push_back(Move::BLACK_ROOK);
        else if (char_board[i] == 'Q') piece_board.push_back(Move::BLACK_QUEEN);
        else if (char_board[i] == 'K') piece_board.push_back(Move::BLACK_KING);
        else piece_board.push_back(0);
    }

    return piece_board;
}