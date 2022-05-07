#include <iostream>
#include <vector>
#include <string>
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
        return count_bits((mask & -(long long)mask) - 1);
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

string algebraic_move(int from, int to) {
    string ret = "";
    vector<char> rows = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    ret += (rows[from % 8] + to_string((int)(from / 8)));
    ret += (rows[to % 8] + to_string((int)(to / 8)));
    return ret;
}

void flip_chess_board(vector<int> &arr) {
    // Vertically flip the piece board
    for (int i = 0; i < 32; ++i) {
        int r = i / 8;
        int c = i % 8;
        swap(arr[i], arr[(7-r) * 8 + c]);
    }
}

U64 flip_bitboard(U64 bitboard) {
    // Vertically flip the bitboard
    // https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating#Flip_and_Mirror
    const U64 k1 = U64(0x00FF00FF00FF00FF);
    const U64 k2 = U64(0x0000FFFF0000FFFF);
    bitboard = ((bitboard >>  8) & k1) | ((bitboard & k1) <<  8);
    bitboard = ((bitboard >> 16) & k2) | ((bitboard & k2) << 16);
    bitboard = ( bitboard >> 32)       | ( bitboard       << 32);
    return bitboard;
}


int get_square_from_algebraic(string alg) {
    if (alg.length() != 2) {
        cout << "Incorrect algebraic string length\n";
        return -1;
    }
    if (alg[0] < 'a' || alg[0] > 'h') {
        cout << "Incorrect letter!\n";
        return -1;
    }
    if (alg[1] < '1' || alg[1] > '8') {
        cout << "Incorrect number!\n";
        return -1;
    }
    int row, col;
    row = ('8' - alg[1]);
    col = (alg[0] - 'a');
    return row * 8 + col;
}

string get_algebraic_from_square(int from, int to) {
    string algebraic = "";
    vector<char> rows = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    algebraic += (rows[from % 8] + to_string(8-(int)(from / 8)));
    algebraic += (rows[to % 8] + to_string(8-(int)(to / 8)));
    return algebraic;
}

bool cmp_floats_eq(double a, double b, double eps) {
    if (abs(a-b) < eps) {
        return true;
    }
    return false;
}

Move create_move(
    unsigned int from,
    unsigned int to,
    unsigned int moved_piece,
    unsigned int captured_piece,
    unsigned int legal_castles,
    unsigned int flags
) {
    return Move(from | (to << 6) | (moved_piece << 12) | (captured_piece << 16) | \
                            (legal_castles << 20) | (flags << 24));
}

void print_piece_board(vector<int> piece_board) {
    for (int i = 0; i < 8; ++i) {
        cout << (8-i) << "   "; 
        for (int j = 0; j < 8; ++j) {
            if (piece_board[i*8+j] == Move::WHITE_PAWN) cout << "p ";
            else if (piece_board[i*8+j] == Move::WHITE_KNIGHT) cout << "n ";
            else if (piece_board[i*8+j] == Move::WHITE_BISHOP) cout << "b ";
            else if (piece_board[i*8+j] == Move::WHITE_ROOK) cout << "r ";
            else if (piece_board[i*8+j] == Move::WHITE_QUEEN) cout << "q ";
            else if (piece_board[i*8+j] == Move::WHITE_KING) cout << "k ";
            else if (piece_board[i*8+j] == Move::BLACK_PAWN) cout << "P ";
            else if (piece_board[i*8+j] == Move::BLACK_KNIGHT) cout << "N ";
            else if (piece_board[i*8+j] == Move::BLACK_BISHOP) cout << "B ";
            else if (piece_board[i*8+j] == Move::BLACK_ROOK) cout << "R ";
            else if (piece_board[i*8+j] == Move::BLACK_QUEEN) cout << "Q ";
            else if (piece_board[i*8+j] == Move::BLACK_KING) cout << "K ";
            else cout << ". ";
        }
        cout << "\n";
    }
    cout << "\n    a b c d e f g h";
    cout << "\n\n\n";
}

