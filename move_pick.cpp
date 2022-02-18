#include "move_pick.h"


using namespace std;

MovePick::MovePick(MoveGen* move_gen) : move_gen(move_gen) {
    // Generate a 64x12 table of random numbers used for Zobrist hashing:
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            // Initialize random value:
            zob_rand_nums[i][j] = rand();
        }
    }
}

int MovePick::zob_hash(const vector<int> &board) {
    // Compute the zobrist hash for the given board:
    // Algorithm described here: https://en.wikipedia.org/wiki/Zobrist_hashing
    int z_hash = 0;
    for (int i = 0; i < 64; ++i) {
        if (board[i]) {
            z_hash ^= zob_rand_nums[i][board[i]-1];
        }
    }
    return z_hash;
}

double MovePick::eval_current_pos() {
    // Check if this position has been evaluated before:
    int hash = zob_hash(move_gen->get_piece_board());
    if (eval_pos.find(hash) != eval_pos.end()) {
        return eval_pos[hash];
    }

    // If the position has not been evaluated, evaluate the position:
    double eval = 0;

    // Remember the evaluation for this position:
    eval_pos[hash] = eval;
    return eval;
}

Move MovePick::find_best_move() { // Find best move up to the given depth
    // Iterate through all possible moves, similar to perft implementation
    move_gen->calculate_moves();
    // Set initial best move value to null move - Guaranteed to change
    Move best_move = Move(0);
    // Keep track of the actual move to be played
    Move played_move = Move(0);
    // Set best score to be lowest possible value
    double best_score = -numeric_limits<double>::infinity();
    while (move_gen->make_move()) {
        // Update the played move if we're at a depth of 2
        if (move_gen->get_curr_depth() == 2) {
            // NOTE: When at depth = 2, move_gen.curr_move is the move played from depth = 1
            played_move = move_gen->get_curr_move();
        }
        // Evaluate current move and update best move if we exceed current depth:
        double score = eval_current_pos();
        // If we're at a leaf node, update the best score:
        if (move_gen->get_curr_depth() > move_gen->get_depth()) {
            if (score > best_score) {
                best_score = score;
                best_move = played_move;
            }
        }
    }
    return best_move;
}