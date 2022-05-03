#include <iostream>
#include "perft.h"

using namespace std;

Perft::Perft (MoveGenRec* move_gen_rec): move_gen_rec(move_gen_rec) {
    leaf_nodes_explored = 0ULL;
    last_leaf_nodes_explored = 0ULL;
}

U64 Perft::calculate_num_moves(int depth) {
    calculate_num_moves_helper(depth, move_gen_rec);
    return leaf_nodes_explored;
}

void Perft::calculate_num_moves_helper(int depth, MoveGenRec* move_gen) {
    // Handle base case
    if (depth == 0) {
        ++leaf_nodes_explored;
        return;
    }
    vector<Move> legal_moves = move_gen->get_legal_moves();
    for (auto move: legal_moves) {
        move_gen->make_move(move);
        calculate_num_moves_helper(depth-1, move_gen);
        move_gen->undo_move(move);
        move_gen->print_out = false;
    }
}
