#ifndef MOVE_GEN_H
#define MOVE_GEN_H

#include <vector>
#include "move.h"
#include "initialize.h"
#include "generate.h"

using namespace std;

class MoveGen {
    public:
        MoveGen(int depth, 
                color active_player,
                Initialize* init,
                Generate* gen) : 
                depth(depth), 
                active_player(active_player),
                init(init),
                gen(gen) {};     
        
        void make_move();
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

    private:
        Initialize* init;
        Generate* gen;
        // Variables to keep track of the pieces
        U64 white_pawns;
        U64 white_knights;
        U64 white_bishops;
        U64 white_rooks;
        U64 white_queens;
        U64 white_king;
        U64 black_pawns;
        U64 black_knights;
        U64 black_bishops;
        U64 black_rooks;
        U64 black_queens;
        U64 black_king;

        U64 en_passant; // Keeps track of what squares can be en passanted

        vector<Move> move_vec;

        Move curr_move = Move(0xf << 20); // Keeps track of the last move made
        // Use 0xf since initially all castles are still valid (no castling has occured)

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
        int piece_board[64];

        // Calculate all moves for the give position
        void calculate_moves();

        void make_move();

        void get_gen_pawn_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_knight_moves(int side, const U64 &friend_bl);

        void get_gen_bishop_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_rook_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_queen_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);

        void get_gen_king_moves(int side, const U64 &friend_bl, const U64 &enemy_bl);
};

#endif