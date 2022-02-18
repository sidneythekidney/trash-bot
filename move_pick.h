/* 

    Implements the evaluation heuristic for evaluating a given chess position
    and returns the best move based on this heuristic up to a certain depth.

*/

#ifndef MOVE_PICK_H
#define MOVE_PICK_H

#include "move_gen.h"
#include "utils.hpp"
#include "move.h"

class MovePick {
    private:
        MoveGen* move_gen;

    public:
        MovePick(MoveGen* move_gen);

        double eval_current_pos(); // Evaluates the board at the current position

        Move find_best_move(); // Finds the best move up to the desired depth
};

#endif