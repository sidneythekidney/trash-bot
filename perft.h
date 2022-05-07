#ifndef PERFT_H
#define PERFT_H

#include "move_gen_rec.h"

class Perft {
    public:
        Perft (MoveGenRec* move_gen_rec);
        U64 calculate_num_moves(int depth);

    private:
        void calculate_num_moves_helper(int depth, MoveGenRec* move_gen);
        MoveGenRec* move_gen_rec;
        U64 leaf_nodes_explored;
        U64 last_leaf_nodes_explored;
};

#endif