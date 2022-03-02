/* 

    Implements the evaluation heuristic for evaluating a given chess position
    and returns the best move based on this heuristic up to a certain depth.

*/

#ifndef MOVE_PICK_H
#define MOVE_PICK_H

#include <limits>
#include <stdlib.h>
#include <unordered_map>
#include <time.h>
#include <stack>
#include <iostream>

#include "move_gen.h"
#include "utils.hpp"
#include "move.h"
#include "initialize.h"
#include "generate.h"

class MovePick {
    private:
        Initialize* init;
        Generate* gen;
        MoveGen* move_gen; // Used to keep track of the current state
        MoveGen* iter_move_gen; // Used to calculate branches from the current state

        bool endgame = false; // Keep track of whether we're in the end game

        // Keep track of evaluated positions:
        unordered_map<int, double> eval_pos;
        unordered_map<U64, int> eval_pawn_structs;

        // Random numbers used for Zobrist hashing:
        int zob_rand_nums[64][12];

        // piece_square table used for position evalation:
        /*
            12 pieces, starting with white pawn = 1, not 0
            2 players
            2 games (middlegame, endgame)
            64 squares
        */
        int piece_square[13][2][64];
        void set_piece_arr(int piece, int game, vector<int> squares);

        U64 row_masks[8];
        U64 col_masks[8];

        // Keep track of the current path scores
        stack<double> path_scores;

        // Helper functions:
        double eval_current_pos(); // Evaluates the board at the current position

        int zob_hash(const vector<int> &board);

        double material_eval(); // Evaluate the current position in terms of material

        double position_eval(); // Evaluate position based on the position of the pieces

        int pawn_structure_eval();

        void init_piece_squares(); // Initialize the piece squares tables

        void init_row_masks();

        void init_col_masks();

    public:
        MovePick(Initialize* init, Generate* gen, MoveGen* move_gen);

        Move find_best_move(int max_runtime); // Finds the best move up to the desired depth

        Move find_best_move_given_time(int time); // Finds the best move given a max time

        void print_path_scores();
};

#endif