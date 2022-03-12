#include <iostream>
#include <sstream>
#include <map>

#include "move_gen.h"
#include "move.h"
#include "initialize.h"
#include "generate.h"
#include "move_pick.h"

string NULL_MOVE_STRING = "0000";

class UCI {
    private:
        Initialize* init;
        Generate* gen;
        MoveGen* move_gen;
        MovePick* move_pick;

        map<string, string> options;

    public:
        UCI();
        void init_starting_pos();
        void print_info();
        void set_options(stringstream &ss);
        void set_position(stringstream &ss);
        void get_best_move(stringstream &ss);
};