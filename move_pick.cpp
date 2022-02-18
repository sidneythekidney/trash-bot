#include "move_pick.h"
#include <limits>

using namespace std;

MovePick::MovePick(MoveGen* move_gen) : move_gen(move_gen) {}

double MovePick::eval_current_pos() {
    double eval = 0;
    return eval;
}

Move MovePick::find_best_move() { // Find best move up to the given depth
    // Iterate through all possible moves, similar to perft implementation
    move_gen->calculate_moves();
    // Set initial best move value to null move - Guaranteed to change
    Move best_move = Move(0);
    // Set best score to be lowest possible value
    double best_score = -numeric_limits<double>::infinity();
    while (move_gen->make_move()) {
        // Evaluate current move and update best move if we exceed current depth:
        double score = eval_current_pos();
        best_move = max(best_score, score);
    }
    return best_move;
}