#include <iostream>
#include "perft.h"

using namespace std;

U64 Perft::calculate_num_moves() {

    move_gen->calculate_moves(); // Initialize start move

    // cout << "num_moves: " << move_gen->num_calculated_moves() << "\n";

    // move_gen->print_piece_board();

    while (move_gen->make_move()) {
        // cout << "depth: " << move_gen->get_curr_depth() << "\n";
    }

    cout << "num_move_combs: " << move_gen->get_num_move_combs() - move_gen->last_num_move_combs << "\n";

    // Print all pieces to make sure we move and unmove correctly
    // cout << "white pawns: \n"; 
    // print_binary(move_gen->get_white_pawns());
    // cout << "white knights: \n"; 
    // print_binary(move_gen->get_white_knights());
    // cout << "white bishops: \n"; 
    // print_binary(move_gen->get_white_bishops());
    // cout << "white rooks: \n"; 
    // print_binary(move_gen->get_white_rooks());
    // cout << "white queens: \n"; 
    // print_binary(move_gen->get_white_queens());
    // cout << "white king: \n"; 
    // print_binary(move_gen->get_white_king());

    // cout << "black pawns: \n";
    // print_binary(move_gen->get_black_pawns());
    // cout << "black knights: \n"; 
    // print_binary(move_gen->get_black_knights());
    // cout << "black bishops: \n"; 
    // print_binary(move_gen->get_black_bishops());
    // cout << "black rooks: \n"; 
    // print_binary(move_gen->get_black_rooks());
    // cout << "black queens: \n"; 
    // print_binary(move_gen->get_black_queens());
    // cout << "black king: \n"; 
    // print_binary(move_gen->get_black_king());

    // cout << "num_move_combs: " << move_gen->get_num_move_combs() - move_gen->last_num_move_combs << "\n";

    return move_gen->get_num_move_combs();
}