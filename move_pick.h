/* 

    Implements the evaluation heuristic for evaluating a given chess position
    and returns the best move based on this heuristic up to a certain depth.

*/

#ifndef MOVE_PICK_H
#define MOVE_PICK_H

#include <limits>
#include <stdlib.h>
#include <unordered_map>

#include "move_gen.h"
#include "utils.hpp"
#include "move.h"

class MovePick {
    private:
        MoveGen* move_gen;

        // Keep track of evaluated positions:
        unordered_map<int, double> eval_pos;

        // Random numbers used for Zobrist hashing:
        int zob_rand_nums[64][12];

        // Helper functions:
        double eval_current_pos(); // Evaluates the board at the current position

        int zob_hash(const vector<int> &board);

        int material_eval(); // Evaluate the current position in terms of material

    public:
        MovePick(MoveGen* move_gen);

        Move find_best_move(); // Finds the best move up to the desired depth
};

#endif