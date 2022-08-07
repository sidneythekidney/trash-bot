// Driver for chess engine

#include <iostream>
#include "move_gen_rec.h"
#include "move.h"
#include "initialize.h"
#include "generate.h"
#include "move_pick_rec.h"
#include "game.h"
#include "opening.h"

using namespace std;

int main() {
    // Create the game object and play the game:
    cout << "Starting up the game\n";
    Initialize* init = new Initialize();
    init->init_all();
    Generate* gen = new Generate(*init);

    MoveGenRec* move_gen = new MoveGenRec(init, gen, color::WHITE);
    // vector<int> piece_board = char_board_to_piece_board(char_board);
    // MoveGenRec* move_gen = new MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
    
    MovePickRec* move_pick = new MovePickRec(gen, move_gen);

    OpeningTree* opening = new OpeningTree(move_gen);

    Game* new_game = new Game(init, gen, move_gen, move_pick, opening);
    
    new_game->play_game();

    delete init;
    delete gen;
    delete move_gen;
    delete move_pick;
    delete new_game;
}