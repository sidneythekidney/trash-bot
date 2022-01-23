#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <vector>
#include "move.h"
#include "initialize.h"
#include "generate.h"

using namespace std;

class MoveGen {
    public:
        MoveGen(
            Initialize* init,
            Generate* gen,
            int depth, 
            color active_player
        );
        // Secondary constructor used for testing:
        MoveGen(
            Initialize* init,
            Generate* gen,
            int depth,
            color active_player,
            vector<int> piece_board,
            U64 en_passant,
            unsigned int castles
        );
    
        void make_move();
        void undo_move();

        // Obtain color masks:
        U64 get_white_pieces();
        U64 get_black_pieces();

        Move get_first_move();
        // getters:
        U64 get_white_pawns();
        U64 get_white_knights();
        U64 get_white_bishops();
        U64 get_white_rooks();
        U64 get_white_queens();
        U64 get_white_king();

        U64 get_black_pawns();
        U64 get_black_knights();
        U64 get_black_bishops();
        U64 get_black_rooks();
        U64 get_black_queens();
        U64 get_black_king();

        U64 get_en_passant();

        // Used for testing calculated moves:
        int num_calculated_moves();

        // Calculate all moves for the give position
        void calculate_moves();

        // Check if a move is a current move, used for testing
        bool check_if_move_calculated(int piece_type, int from, int to);
        // Print the moves in the cur_move vector:
        void print_cur_moves();

    private:
        Initialize* init;
        Generate* gen;
        // Vector to keep track of the pieces
        int NUM_PIECE_TYPES = 12; 

        // U64 white_pawns = 0ULL;
        // U64 white_knights = 0ULL;
        // U64 white_bishops = 0ULL;
        // U64 white_rooks = 0ULL;
        // U64 white_queens = 0ULL;
        // U64 white_king = 0ULL;
        // U64 black_pawns = 0ULL;
        // U64 black_knights = 0ULL;
        // U64 black_bishops = 0ULL;
        // U64 black_rooks = 0ULL;
        // U64 black_queens = 0ULL;
        // U64 black_king = 0ULL;

        // Holds a vector of all pieces by type:
        vector< U64 > p;

        U64 en_passant; // Keeps track of what squares can be en passanted

        vector<Move> move_vec;

        Move curr_move = Move(0xf << 20); // Keeps track of the last move made
        // Use 0xf since initially all castles are still valid (no castling has occured)

        // Define blocker masks used when calcuclating castles:
        U64 white_ks_blockers = 3ULL << 61;
        U64 white_qs_blockers = 7ULL << 57;
        U64 black_ks_blockers = 3ULL << 5;
        U64 black_qs_blockers = 7ULL << 1;

        void add_move(
            unsigned int from,
            unsigned int to,
            unsigned int moved_piece,
            unsigned int captured_piece,
            unsigned int legal_castles,
            unsigned int flags
        );

        int depth;
        color active_player;

        // Piece board holds all of positions of all of the pieces by type in one variable
        vector<int> piece_board;

        void get_gen_pawn_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_knight_moves(int side, const U64 &friend_bl);

        void get_gen_bishop_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_rook_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_queen_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_king_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);
};

#endif