#ifndef OPENING_H
#define OPENING_H

#include "initialize.h"
#include "generate.h"
#include "move.h"
#include "move_gen_rec.h"
#include <string>
#include <map>

using namespace std;

class OpeningTreeNode {
    public:
        OpeningTreeNode();
        vector<unsigned int> next_moves;
        map<unsigned int, OpeningTreeNode*> next_move_ptrs;
};
class OpeningTree {
    private:
        OpeningTreeNode* root;
        MoveGen* move_gen;
        void opening_init();
    public:
        OpeningTree(MoveGen* move_gen);
        MoveGen* get_curr_pos();
        bool move_to_next_node(Move move);
        int get_random_move();
};

#endif