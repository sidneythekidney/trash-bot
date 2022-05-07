#include "game.h"
#include <iostream>
#include <time.h>

using namespace std;

Game::Game(Initialize* init, Generate* gen, MoveGenRec* move_gen, MovePickRec* move_pick, OpeningTree* opening) : 
            init(init), gen(gen), move_pick(move_pick), game_move_gen(move_gen), opening(opening) {
                cpu_time = 0;
                in_opening = true;
            }

int Game::get_user_side() {
    bool chose_side = false;
    int side_chosen;
    while (!chose_side) {
        cout << "Human player, choose your side: [w/b]\n";
        char side;
        cin >> side;
        if (side == 'w') {
            cout << "Selected to play with WHITE\n";
            side_chosen = color::WHITE;
            chose_side = true;
        }
        else if (side == 'b') {
            cout << "Selected to play with BLACK\n";
            side_chosen = color::BLACK;
            chose_side = true;
        }
        else {
            cout << "Invalid choice: please select either 'w' or 'b'\n";
        }
    }
    user_side = side_chosen;
    return side_chosen;
}

int Game::get_user_promotion() {
    while (true) {
        string promote;
        cout << "Enter piece to be promoted (q|r|b|n):\n";
        cin >> promote;
        if (promote == "q") {
            cout << "Promoted to queen!\n";
            // Since the original move considers KNIGHT as the first promoted piece, not PAWN
            return 4;
        }
        else if (promote == "r") {
            cout << "Promoted to rook!\n";
            return 3;
        }
        else if (promote == "b") {
            cout << "Promoted to bishop!\n";
            return 2;
        }
        else if (promote == "n") {
            cout << "Promoted to knight!\n";
            return 1;
        }
        cout << "Invalid promotion piece entered, enter either 'q', 'r', 'b', or 'n'\n";
    }
    cout << "Error occured in promote function\n";
    exit(1);
}

void Game::play_user_move() {
    // Get user move:
    bool move_selected = false;
    Move move = Move(0);
    string input;        
    vector<Move> legal_moves = game_move_gen->get_legal_moves();
    while (!move_selected) {
        cout << "select move (ex. e2e4): ";
        cin >> input;
        // Obtain move:
        unsigned int from, to;
        try {
            from = (7 - (input[1] - '1')) * 8 + (input[0] - 'a');
            to = (7 - (input[3] - '1')) * 8 + (input[2] - 'a');
            // int moved_piece = game_move_gen->get_piece_board()[from];
        }
        catch (exception& e) {
            cout << "Move input incorrectly!\n";
            continue;
        }
        cout << "from: " << from << "\n";
        cout << "to: " << to << "\n";
        // Validate move:
        for (auto single_move : legal_moves) {
            if (single_move.get_from() == from && single_move.get_to() == to) {
                move = single_move;
            }
        }
        if (move.get_move() == 0) {
            cout << "move input incorrectly, try again!\n";
            continue;
        }
        // Handle promotion
        if (move.get_promotion()) {
            // Get promotion offset and add to currently moved piece:
            unsigned int piece = get_user_promotion() + ((game_move_gen->get_active_player() == color::WHITE) ? Move::WHITE_PAWN : Move::BLACK_PAWN);
            for (auto single_move : legal_moves) {
                if (single_move.get_from() == from && single_move.get_to() == to && single_move.get_moved() == piece) {
                    move = single_move;
                }
            }
        }
        // Otherwise, play the move now that it is valid:
        cout << "Attempting to play user move\n";
        game_move_gen->make_move(move);
        move_selected = true;
    }
    // Update the opening based on what the user selects
    if (in_opening) {
        // Check if move follows opening and update:
        in_opening = opening->move_to_next_node(move);
        cout << "playing opening move: from: " << move.get_from() << " to: " << move.get_to() << "\n";
    }
}

void Game::play_cpu_move() {
    if (in_opening) {
        // Check if move follows opening and update:
        int opening_move = opening->get_random_move();
        in_opening = opening->move_to_next_node(Move(opening_move));
        if (in_opening) {
            // Play the move and return:
            game_move_gen->make_move(opening_move);
            return;
        }
    }
    cout << "Thinking of cpu move...\n";
    // Select and play the best move for the given position using iterative deepening:
    Move move = move_pick->get_best_move(6, false);
    game_move_gen->make_move(move);
}

void Game::play_game() {
    int side_chosen = get_user_side();
    // Begin asking for and playing moves:
    bool game_over = false;
    string result;
    // Create a move_gen object to play the game:
    while (!game_over) {
        cout << "\ncurrent board\n";
        game_move_gen->print_color_piece_board(user_side);
        
        if (game_move_gen->get_active_player() == side_chosen) {
            // Check for checkmate/stalemate by the engine
            vector<Move> legal_moves = game_move_gen->get_legal_moves();
            if (legal_moves.size() == 0) {
                if (game_move_gen->checked()) {
                    // Checkmate
                    cout << "CHECKMATE: " << (game_move_gen->get_active_player() ? "white " : "black ") << "wins!\n";
                    return;
                }
                else {
                    // Stalemate
                    cout << "STALEMATE: the game resulted in a draw!\n";
                    return;
                }
            }
            play_user_move();
        }
        else {
            // Check for checkmate/stalemate by the user
            vector<Move> legal_moves = game_move_gen->get_legal_moves();
            if (legal_moves.size() == 0) {
                if (game_move_gen->checked()) {
                    // Checkmate
                    cout << "CHECKMATE: " << (game_move_gen->get_active_player() ? "white " : "black ") << "wins!\n";
                    return;
                }
                else {
                    // Stalemate
                    cout << "STALEMATE: the game resulted in a draw!\n";
                    return;
                }
            }
            // Get computer move:
            cout << "Attempting to get cpu move!\n";
            play_cpu_move();
        }
    }
}
