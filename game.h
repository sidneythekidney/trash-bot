// This class is used to play the game:
#ifndef GAME_H
#define GAME_H

#include "move_gen_rec.h"
#include "move.h"
#include "initialize.h"
#include "generate.h"
#include "move_pick_rec.h"
#include "opening.h"


class Game {
    private:
        Initialize* init;
        Generate* gen;
        MovePickRec* move_pick;
        MoveGenRec* game_move_gen;
        OpeningTree* opening;
        int user_side;

        int get_user_side();

        int get_user_promotion();

        void play_user_move();

        void play_cpu_move();

        int cpu_time;

        bool in_opening;

    public:
        Game(Initialize* init, Generate* gen, MoveGenRec* move_gen, MovePickRec* move_pick, OpeningTree* opening);

        void play_game();
};

#endif