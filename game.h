// This class is used to play the game:
#ifndef GAME_H
#define GAME_H

#include "move_gen.h"
#include "move.h"
#include "initialize.h"
#include "generate.h"
#include "move_pick.h"
#include "opening.h"


class Game {
    private:
        Initialize* init;
        Generate* gen;
        MoveGen* cpu_move_gen;
        MovePick* move_pick;
        MoveGen* game_move_gen;
        OpeningTree* opening;

        int get_user_side_and_time();

        int get_user_promotion();

        void play_user_move();

        void play_cpu_move();

        int cpu_time;

        bool in_opening;

    public:
        Game(Initialize* init, Generate* gen, MoveGen* move_gen, MovePick* move_pick, OpeningTree* opening);

        void play_game();
};

#endif