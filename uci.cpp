// This file contains the code to interact with GUIs like Arena using the UCI
#include "uci.h"

using namespace std;

UCI::UCI() {
    // Initialize to be in the starting position
    init = new Initialize();
    init->init_all();
    gen = new Generate(*init);
    // init_starting_pos();
}

void UCI::init_starting_pos() {
    move_gen = new MoveGen(init, gen, 1, color::WHITE);
    move_pick = new MovePick(init, gen, move_gen);
}

void UCI::print_info() {
    cout << "id name trash bot\nid author Sidney Meade\n";
}

void UCI::set_options(stringstream &ss) {
    string key, val;
    ss >> key >> val;
    options[key] = val;
}

void UCI::uci_new_game() {
    init_starting_pos();
}

void UCI::set_position(stringstream &ss) {
    string type, moves;
    ss >> type;
    if (type == "startpos") {
        init_starting_pos();
        ss >> moves;
        if (moves == "moves") {
            // Make recent moves:
            string move;
            while (ss >> move) {
                // Convert move to engine-readable move and play that move:
                int from, to;
                from = get_square_from_algebraic(move.substr(0, 2));
                to = get_square_from_algebraic(move.substr(2, 4));
                // Calculate moves:
                move_gen->calculate_moves();
                Move to_play = move_gen->get_move_from_pos(from, to);
                // Play move that has the same from and to squares
                move_gen->play_move(to_play);
            }
        }
    }
    else if (type == "fen") {
        cout << "Updating position based on fen not currently supported\n";
        return;
    }
    else {
        cout << "Invalid type: " << type << "\n";
        return;
    }
}

void UCI::get_best_move(stringstream &ss) {
    // Update position as needed:
    string depth;
    ss >> depth;
    if (depth != "depth") {
        cout << "Timed search not currently supported!!\n";
        return;
    }
    string depth_str;
    ss >> depth_str;
    int depth_val = stoi(depth_str);
    // Set the depth on the move_gen object
    move_gen->set_max_depth(depth_val);
    move_pick->set_iter_move_gen(depth_val);
    // Find best move
    Move best_move = move_pick->find_best_move(3600); // limit search to 1 hour, game should be playable
    cout << "bestmove " << get_algebraic_from_square(best_move.get_from(), best_move.get_to()) << "\n";
}

int main() {
    string line;
    string cmd;
    UCI* uci = new UCI();
    uci->print_info();
    // while (getline(cin, line)) {
    //     // Accept UCI input:
    //     stringstream in(line);
    //     in >> cmd;
    //     if (cmd == "uci") {
    //         uci->print_info();
    //         cout << "uciok" << "\n";
    //     }
    //     else if (cmd == "isready") {
    //         cout << "readyok\n";
    //     }
    //     else if (cmd == "setoption") {
    //         // Set options as required:
    //         uci->set_options(in);
    //     }
    //     else if (cmd == "ucinewgame") {
    //         // Reset the board
    //         uci->uci_new_game();
    //     }
    //     else if (cmd == "position") {
    //         uci->set_position(in);
    //     }
    //     else if (cmd == "go") {
    //         uci->get_best_move(in);
    //     }
    //     else if (cmd == "stop") {
    //         cout << "Stop command not currently supported!!!\n";
    //     }
    //     else if (cmd == "quit") {
    //         // Quit the program
    //         exit(0);
    //     }
    //     else {
    //         cout << "Inavlid command: " << cmd << "\n";
    //     }
    // }
    // delete uci;
    while (getline(cin, line)) {
        cout << "enter input:\n";
        cout << "input: " << line << "\n";
    }
}
