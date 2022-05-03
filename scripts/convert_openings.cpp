#include <fstream>
#include <sstream>
#include <string>
#include "../initialize.h"
#include "../generate.h"
#include "../move_gen_rec.h"
#include "../move.h"

using namespace std;

int main() {
    ifstream infile("../data/openings.txt");
    ofstream outfile("../data/openings_coordinates.txt");
    ofstream move_outfile("../data/opening_moves.txt");
    string line;
    while (getline(infile, line)) {
        // Obtain move sequence for each opening:
        int last_colon = 0;
        for (int i = 0; i < (int)line.length(); ++i) {
            if (line[i] == ':') {
                last_colon = i;
            }
        }
        string algebraic = line.substr(last_colon + 2, line.length() - last_colon - 2);
        vector<string> alg_vec = {};
        string curr_move = "";
        for (int i = 0; i < (int)algebraic.length(); ++i) {
            if (algebraic[i] == ' ') {
                // Add the word to the algebraic vector:
                alg_vec.push_back(curr_move);
                curr_move = "";
            }
            else {
                curr_move += algebraic[i];
            }
        }
        // Add the final move to the vector:
        if (curr_move.length() != 0) {
            alg_vec.push_back(curr_move);
        }

        // Create move generator to convert to coordinate notation:
        Initialize* init = new Initialize();
        init->init_all();
        Generate* gen = new Generate(*init);
        MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE);

        move_gen.calculate_moves();

        for (int i = 0; i < (int)alg_vec.size(); ++i) {
            // Handle castles:
            if (alg_vec[i] == "O-O") {
                // Handle kingside castle
                // Obtain the correct move:
                Move move = Move(0);
                if (i % 2) {
                    // Obtain black kingside castle
                    move = move_gen.get_move_from_pos(4, 6);
                }
                else {
                    // Obtain white kingside castle
                    move = move_gen.get_move_from_pos(60, 62);
                }
                // Handle error
                if (move.get_move() == 0) {
                    move_gen.print_cur_moves();
                    cout << "An error occured (kingside castle)!\n";
                    exit(1);
                }
                move_gen.play_move(move);
                move_gen.calculate_moves();
                continue;
            }
            if (alg_vec[i] == "O-O-O") {
                // Handle queenside castle
                Move move = Move(0);
                if (i % 2) {
                    // Obtain black queenside castle
                    move = move_gen.get_move_from_pos(4, 2);
                }
                else {
                    // Obtain white queenside castle
                    move = move_gen.get_move_from_pos(60, 58);
                }
                if (move.get_move() == 0) {
                    move_gen.print_cur_moves();
                    cout << "An error occured (queenside castle)!\n";
                    exit(1);
                }
                move_gen.play_move(move);
                move_gen.calculate_moves();
                continue;
            }
            // Get moved piece
            int piece_type = Move::WHITE_PAWN;
            if (alg_vec[i][0] == 'N') {
                piece_type = Move::WHITE_KNIGHT;
            }
            if (alg_vec[i][0] == 'B') {
                piece_type = Move::WHITE_BISHOP;
            }
            if (alg_vec[i][0] == 'R') {
                piece_type = Move::WHITE_ROOK;
            }
            if (alg_vec[i][0] == 'Q') {
                piece_type = Move::WHITE_QUEEN;
            }
            if (alg_vec[i][0] == 'K') {
                piece_type = Move::WHITE_KING;
            }
            // Update the moved piece if it is black to play:
            piece_type += (i % 2) ? 6 : 0;

            // Get the square from the back of the string:
            int square_pos = alg_vec[i].length() - 1;
            while (!isdigit(alg_vec[i][square_pos])) {
                --square_pos;
            }

            string square = alg_vec[i].substr(square_pos-1, 2);

            // Obtain column of moved piece if specified:
            int col = -1;
            int col_pos = square_pos - 2;
            while (col_pos > 0 && islower(alg_vec[i][col_pos-1])) {
                col_pos -= 1;
            }
            if (col_pos >= 0 && alg_vec[i][col_pos] >= 'a' && alg_vec[i][col_pos] < 'h') {
                col = alg_vec[i][col_pos] - 'a';
            }

            Move move = move_gen.get_from_square_from_move(piece_type, get_square_from_algebraic(square), col);
            if (move.get_move() == 0) {
                move_gen.print_cur_moves();
                cout << "An error occured\n";
                exit(1);
            }
            // Add the from and to the output file:
            outfile << get_algebraic_from_square(move.get_from(), get_square_from_algebraic(square)) << " ";
            move_outfile << move.get_move() << " ";
            // Play move:
            move_gen.play_move(move);
            move_gen.calculate_moves();
        }
        outfile << "\n";
        move_outfile << "\n";
    }
    infile.close();
    outfile.close();
}