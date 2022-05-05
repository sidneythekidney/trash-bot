#ifndef MOVE_PICK_REC_H
#define MOVE_PICK_REC_H

#include "move.h"
#include "move_gen_rec.h"
#include "defs.h"
#include "utils.hpp"

#include <unordered_map>
#include <vector>

class MovePickRec {
    public:
        MovePickRec(Generate* gen, MoveGenRec* move_gen);
        // Find the best move at a certain depth
        Move get_best_move(int depth);
        double get_best_move_helper(int depth, int starting_depth, double alpha, double beta);

        // Determine if side is in checkmate and evaluate accordingly
        double checkmate_eval();

        // Evaluate the current position in terms of material
        double material_eval();

        // Evaluate position based on the position of the pieces
        double position_eval();

        // EValuate general pawn structure
        double pawn_structure_eval();

        // Evaluate doubled pawns
        double doubled_pawns_eval();
        
        // Search and evaluate for pawn chains
        double pawn_chain_eval();

        // Find and evaluate isolated pawns
        double isolated_pawns_eval();

        // Find and evaluate passed pawns
        double passed_pawns_eval();

        // Evaluate king safety
        double king_safety_eval();

    private:
        Generate* gen;
        MoveGenRec* move_gen;
        color starting_player;
        Move best_move = Move(0);
        bool endgame = false;
        unordered_map<int, double> eval_pos;
        unordered_map<U64, int> eval_pawn_structs;

        // Will contain random numbers used for zobrist hashing
        int zob_rand_nums[64][12];
        // piece_square table used for position evalation:
        /*
            12 pieces, starting with white pawn = 1, not 0
            2 players
            2 games (middlegame, endgame)
            64 squares
        */
        int piece_square[13][2][64];

        // Masks for determining passed pawns
        vector<U64> w_passed_pawn_mask;
        vector<U64> b_passed_pawn_mask;

        vector<U64> row_masks;
        vector<U64> col_masks;

        // Masks for helping to determine king safety
        vector<U64> w_king_safety_masks;
        vector<U64> b_king_safety_masks;

        // Evaluate the current position
        double eval_current_pos();

        int zob_hash(const vector<int> &board);

        // Initialize piece square tables
        void init_piece_squares();

        void init_row_masks();

        void init_col_masks();
        
        void init_passed_pawn_masks();

        void init_king_safety_masks();

        void set_piece_arr(int piece, int game, vector<int> squares);

        double alphaBetaMax(double alpha, double beta, int depth, int start_depth);
        double alphaBetaMin(double alpha, double beta, int depth, int start_depth);

        U64 leaf_nodes_explored = 0ULL;
        U64 last_leaf_nodes_explored = 0ULL;
};

#endif