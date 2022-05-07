// This file contains the code to interact with GUIs like Arena using the UCI
#include "uci.h"

using namespace std;

UCI::UCI() {
    // Initialize to be in the starting position
    init = new Initialize();
    init->init_all();
    gen = new Generate(*init);
    init_starting_pos();
}

void UCI::init_starting_pos() {
    move_gen = new MoveGenRec(init, gen, color::WHITE);
    move_pick = new MovePickRec(gen, move_gen);
}

void UCI::print_info() {
    cout << "id name trash bot\nid author Sidney Meade\n";
}

void UCI::set_options(stringstream &ss) {
    string key, val;
    ss >> key >> val;
    options[key] = val;
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
            Move to_play(0);
            while (ss >> move) {
                // Convert move to engine-readable move and play that move:
                int from, to;
                from = get_square_from_algebraic(move.substr(0, 2));
                to = get_square_from_algebraic(move.substr(2, 4));
                int promoted_piece = 0;
                if (move.length() == 5) {
                    if (move[5] == 'n') promoted_piece = 2;
                    else if (move[5] == 'b') promoted_piece = 3;
                    else if (move[5] == 'r') promoted_piece = 4;
                    else if (move[5] == 'q') promoted_piece = 5;
                    else {
                        cout << "invalid promoted piece\n";
                        exit(1);
                    }
                }
                // Get legal moves
                vector<Move> legal_moves = move_gen->get_legal_moves();
                for (auto legal_move : legal_moves) {
                    if (move.length() == 5) {
                        if ((int)legal_move.get_from() == from && (int)legal_move.get_to() == to && (int)legal_move.get_moved() % 6 == promoted_piece) {
                            to_play = legal_move;
                            break;
                        }
                    }
                    else {
                        if ((int)legal_move.get_from() == from && (int)legal_move.get_to() == to) {
                            to_play = legal_move;
                            break;
                        }
                    }
                }
                // Play move that has the same from and to squares and moved piece
                if ((int)to_play.get_move() == 0) {
                    cout << "Invalid move found in the sequence!\n";
                    exit(1);
                }
                move_gen->make_move(to_play);
            }
        }
        move_gen->print_piece_board();
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
    // Find best move
    Move best_move = move_pick->get_best_move(depth_val, false);
    cout << "bestmove " << get_algebraic_from_square(best_move.get_from(), best_move.get_to()) << "\n";
}

int main() {
    string line;
    string cmd;
    UCI* uci = new UCI();
    uci->print_info();
    while (getline(cin, line)) {
        // Accept UCI input:
        stringstream in(line);
        in >> cmd;
        if (cmd == "uci") {
            uci->print_info();
            cout << "uciok" << "\n";
        }
        else if (cmd == "isready") {
            cout << "readyok\n";
        }
        else if (cmd == "setoption") {
            // Set options as required:
            uci->set_options(in);
        }
        else if (cmd == "ucinewgame") {
            // Reset the board
            uci->init_starting_pos();
        }
        else if (cmd == "position") {
            uci->set_position(in);
        }
        else if (cmd == "go") {
            uci->get_best_move(in);
        }
        else if (cmd == "stop") {
            cout << "Stop command not currently supported!!!\n";
        }
        else if (cmd == "quit") {
            // Quit the program
            exit(0);
        }
    }
    delete uci;
}
