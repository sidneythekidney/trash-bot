#include "game.h"
#include <iostream>
#include <time.h>

using namespace std;

Game::Game(Initialize* init, Generate* gen, MoveGen* move_gen, MovePick* move_pick) : 
            init(init), gen(gen), cpu_move_gen(move_gen), move_pick(move_pick), game_move_gen(move_gen) {
                cpu_time = 0;
            }

int Game::get_user_side_and_time() {
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
    // Prompt the user to select run time for the computer:
    bool time_chosen = false;
    while (!time_chosen) {
        int time;
        cout << "Human player, enter the amount of time you wish to give the cpu per move in whole seconds: \n";
        cin >> time;
        if (time < 3) {
            cout << "Come on, give it at least 3 seconds!\n";
            continue;
        }
        else {
            cpu_time = time;
            time_chosen = true;
        }
    }
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
            return 3;
        }
        else if (promote == "r") {
            cout << "Promoted to rook!\n";
            return 2;
        }
        else if (promote == "b") {
            cout << "Promoted to bishop!\n";
            return 1;
        }
        else if (promote == "n") {
            cout << "Promoted to knight!\n";
            return 0;
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
    game_move_gen->calculate_moves();
    while (!move_selected) {
        cout << "select move (ex. e2e4): ";
        cin >> input;
        // Obtain move:
        int from, to;
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
        move = game_move_gen->get_move_from_pos(from, to);
        if (move == 0) {
            game_move_gen->print_cur_moves();
            continue;
        }
        // Handle promotion
        if (move.get_promotion()) {
            // Get promotion offset and add to currently moved piece:
            cout << "move get moved: " << move.get_moved() << "\n";
            move = Move(move.get_move() + (get_user_promotion() << 12));
            cout << "move get moved: " << move.get_moved() << "\n";
        }
        // Otherwise, play the move now that it is valid:
        cout << "Attempting to play user move\n";
        game_move_gen->play_move(move);
        move_selected = true;
        // Remove the first move to reset depth:
        game_move_gen->set_start_move(game_move_gen->get_curr_move());
    }
}

void Game::play_cpu_move() {
    cout << "Thinking of cpu move...\n";
    // Select and play the best move for the given position using iterative deepening:
    cout << "curr move from: " << game_move_gen->get_curr_move().get_from() << " to: " << game_move_gen->get_curr_move().get_to() << "\n";
    Move move = move_pick->find_best_move_at_depth(6);
    // cout << "Piece board after finding the best move\n";
    // game_move_gen->print_piece_board();
    // cout << "move from:" << move.get_from() << "\n";
    // cout << "move to:" << move.get_to() << "\n";
    game_move_gen->play_move(move);
    // cout << "Piece board after making the best move\n";
    // game_move_gen->print_piece_board();
}

void Game::play_game() {
    int side_chosen = get_user_side_and_time();
    // Begin asking for and playing moves:
    bool game_over = false;
    string result;
    // Create a move_gen object to play the game:
    while (!game_over) {
        cout << "\ncurrent board\n";
        game_move_gen->print_piece_board();
        // Check for checkmate/stalemate:
        if (game_move_gen->get_checkmate_status() == checkmate::STALEMATE) {
            // Print stalemate message and return
            cout << "STALEMATE: the game resulted in a draw!\n";
            return;
        }
        else if (game_move_gen->get_checkmate_status() > checkmate::STALEMATE) {
            // Print checkmate message and return
            cout << "CHECKMATE: " << (game_move_gen->get_active_player() ? "white " : "black ") << "wins!\n";
            return;
        }
        if (game_move_gen->get_active_player() == side_chosen) {
            play_user_move();
        }
        else {
            // Get computer move:
            cout << "Attempting to get cpu move!\n";
            play_cpu_move();
        }
    }
    cout << "The match ended in a " << result << "\n";
}
