// This class is used to play the game:
#ifndef GAME_H
#define GAME_H

#include "move_gen.h"
#include "move.h"
#include "initialize.h"
#include "generate.h"
#include "move_pick.h"


class Game {
    private:
        Initialize* init;
        Generate* gen;
        MoveGen* cpu_move_gen;
        MovePick* move_pick;
        MoveGen* game_move_gen;

        int get_user_side_and_time();

        void play_user_move();

        void play_cpu_move();

        int cpu_time;

    public:
        Game(Initialize* init, Generate* gen, MoveGen* move_gen, MovePick* move_pick);

        void play_game();
};

#endif