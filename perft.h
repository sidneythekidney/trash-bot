#ifndef PERFT_H
#define PERFT_H

#include "move_gen.h"

class Perft {
    public:
        Perft (MoveGen* move_gen) : move_gen(move_gen) {};
        U64 calculate_num_moves(); // Calculate the number of moves up to move_gen.depth()

    private:
        MoveGen* move_gen;
};

#endif