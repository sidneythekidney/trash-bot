
#ifndef MOVE_GEN_REC_H
#define MOVE_GEN_REC_H

#include "initialize.h"
#include "generate.h"
#include "defs.h"
#include "move.h"

#include <vector>
#include <stack>

class MoveGenRec {
    public:
        MoveGenRec(
            Initialize* init,
            Generate* gen,
            color active_player
        );
        // Secondary constructor used for testing:
        MoveGenRec(
            Initialize* init,
            Generate* gen,
            color active_player,
            int* initial_board,
            U64 en_passant,
            unsigned int castles
        );

        void make_move(Move move);
        void undo_move(Move move);
        vector<Move> get_legal_moves();
        void print_piece_board();
        void print_color_piece_board(int friendly_side);
        void print_all_boards();
        bool checked();

        // getters
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
        U64 get_white_pieces();
        U64 get_black_pieces();
        color get_active_player();

        // setters
        void set_active_player(color new_active_player);

        int* get_piece_board();

    private:
        // Private member variables
        Initialize* init;
        Generate* gen;
        color active_player;
        int* piece_board;
        U64 en_passant;
        int castles;
        vector<U64> piece_masks;
        stack<Move> move_history;
        vector<Move> move_list;

        void calculate_moves(); // calculates pseudolegal moves
        void move_piece(Move move, int undo);
        bool in_check(Move move);
        void add_move(
            unsigned int from,
            unsigned int to,
            unsigned int moved_piece,
            unsigned int captured_piece,
            unsigned int legal_castles,
            unsigned int flags
        );

        // Private helper functions
        void get_gen_pawn_moves(color active_player, U64 friend_bl, U64 enemy_bl);
        void get_gen_knight_moves(color active_player, U64 friend_bl);
        void get_gen_bishop_moves(color active_player, U64 friend_bl, U64 enemy_bl);
        void get_gen_rook_moves(color active_player, U64 friend_bl, U64 enemy_bl);
        void get_gen_queen_moves(color active_player, U64 friend_bl, U64 enemy_bl);
        void get_gen_king_moves(color active_player, U64 friend_bl, U64 enemy_bl);

        // private constants
        int NUM_PIECE_TYPES = 12;
        U64 white_ks_blockers = 3ULL << 61;
        U64 white_qs_blockers = 7ULL << 57;
        U64 black_ks_blockers = 3ULL << 5;
        U64 black_qs_blockers = 7ULL << 1;
        int black_castles = 0b1100;
        int white_castles = 0b0011;

};

#endif