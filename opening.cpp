#include "opening.h"
#include "move_gen_rec.h"
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

OpeningTreeNode::OpeningTreeNode() : next_moves({}) {}

OpeningTree::OpeningTree(MoveGenRec* move_gen) : move_gen(move_gen) {
    // Initialize opening tree:
    opening_init();
}

MoveGenRec* OpeningTree::get_curr_pos() {
    return move_gen;
}

bool OpeningTree::move_to_next_node(Move move) {
    // Check to make sure this move is available:
    if (root->next_move_ptrs.find(move.get_move()) != root->next_move_ptrs.end()) {
        root = root->next_move_ptrs[move.get_move()];
        return true;
    }
    return false;
}

int OpeningTree::get_random_move() {
    // Get random move from the curr node:
    if (root->next_moves.size() == 0) {
        cout << "Tried to get a random node when none existed.\n";
        return 0;
    }
    int random_ind = rand() % root->next_moves.size();
    // Make the next move in the opening:
    return root->next_moves[random_ind];
}

void OpeningTree::opening_init() {
    cout << "Initializing opening!\n";
    // Read in the opening moves
    ifstream infile("data/opening_moves.txt");
    // Initialize opening tree
    root = new OpeningTreeNode();
    OpeningTreeNode* curr_node = root;

    // Read in moves file line by line:
    string line;
    while (getline(infile, line)) {
        // Parse the input file and add moves to next_moves:
        curr_node = root;
        stringstream ss(line);
        int move_num;
        while (ss >> move_num) {
            // Create new move based on this move:
            Move move = Move(move_num);
            curr_node->next_moves.push_back(move.get_move());
            // Create new node to travel to if not already seen:
            if (curr_node->next_move_ptrs.find(move.get_move()) == curr_node->next_move_ptrs.end()) {
                OpeningTreeNode* new_node = new OpeningTreeNode();
                curr_node->next_move_ptrs[move.get_move()] = new_node;
                curr_node = new_node;
            }
            else {
                // Otherwise travel to existing node:
                curr_node = curr_node->next_move_ptrs[move.get_move()];
            }
        }
    }
    cout << "Finished initializing opening\n";
}
