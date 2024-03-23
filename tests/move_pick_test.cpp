// Unit tests for the MovePickRec class

#include <iostream>
#include "../generate.h"
#include "../utils.hpp"
#include "../move_gen_rec.h"
#include "../move_pick_rec.h"

using namespace std;

class MovePickRecTest {
    public:
        MovePickRecTest(Initialize* init, Generate* gen) : init(init), gen(gen) {}
        
        void test_all() {
            // Run all test functions inside this function:
            test_init();
            test_init_move_pick();
            test_material_eval();
            test_position_eval();
            test_doubled_pawns_eval();
            test_pawn_chain_eval();
            test_isolated_pawns_eval();
            test_passed_pawns_eval();
            test_king_safety_eval();
            test_get_best_move1();
            test_get_best_move2();
            test_get_best_move_checkmate_depth_1();
            test_get_best_move_checkmate_depth_2();
            test_avoid_stalemate();
        }
   
   private:
        Initialize* init;
        Generate* gen;

        void print_success(string success_msg) {
            cout << "\033[32;40m" + success_msg + "\033[0m\n";
        }

        void print_error(string error_msg) {
            cout << "\033[31;40m" + error_msg + "\033[0m\n";
        }

        void test_init() {
            print_success("PASS: test_init passed successfully");
        }

        bool almost_equals(double a, double b, double eps) {
            return abs(a - b) < eps;
        }

        void test_init_move_pick() {
            // Create move_gen and move_pick objects:
            MoveGenRec* move_gen = new MoveGenRec(init, gen, color::WHITE);
            MovePickRec move_pick = MovePickRec(init, gen, move_gen);
            Move best_move = move_pick.get_best_move(3, true);

            if (best_move.get_to() == 0 || best_move.get_from() == 0) {
                print_error("FAIL: test_init_move_pick - best move not set");
            }

            print_success("PASS: test_init_move_pick passed successfully");
        }

        void test_material_eval() {
            // Test the board from a near end game position
            vector<char> char_board1 = {
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', 'P', 'Q', '0', 'K', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'k', '0', '0', '0', '0'
            };
            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.material_eval() != -900) {
                print_error("FAIL: Failed material evaluation test!");
                cout << "Received: " << move_pick1.material_eval() << "\n";
                cout << "Expected: -900\n";
                fail = true;
            }

            // Test the material evaluation from the starting position
            MoveGenRec* move_gen2 = new MoveGenRec(init, gen, color::WHITE);
            MovePickRec move_pick2 = MovePickRec(init, gen, move_gen2);

            if (move_pick2.material_eval() != 0) {
                print_error("FAIL: Failed material evaluation test!");
                cout << "Received: " << move_pick2.material_eval() << "\n";
                cout << "Expected: 0\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test material evaluation test!\n");
            }
        }

        void test_position_eval() {
            vector<char> char_board1 = {
                'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'p', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', '0', 'p', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'
            };
            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.position_eval() != 40) {
                print_error("FAIL: Failed position evaluation test!");
                cout << "Received: " << move_pick1.position_eval() << "\n";
                cout << "Expected: 40\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test position evaluation test!\n");
            }
        }

        void test_doubled_pawns_eval() {
            vector<char> char_board1 = {
                '0', '0', '0', '0', '0', 'K', '0', '0',
                'P', 'P', '0', '0', '0', '0', 'P', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', 'p', '0', '0', 'p', '0', '0', '0',
                '0', 'k', '0', '0', '0', '0', '0', '0'
            };
            // Black has 2 doubled pawn structures, white has 1

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.doubled_pawns_eval() != 50) {
                print_error("FAIL: incorrect doubled pawns eval!");
                cout << "Received doubled pawns eval: " << move_pick1.doubled_pawns_eval() << "\n";
                cout << "Expected doubled pawns eval: 50" << "\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test doubled pawns eval!\n");
            }
        }

        void test_pawn_chain_eval() {
            vector<char> char_board1 = {
                '0', '0', '0', '0', 'K', '0', '0', '0',
                'P', '0', 'P', 'P', 'P', 'P', 'P', 'P',
                '0', 'P', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', 'p', '0', '0', '0', '0',
                '0', 'p', '0', '0', 'p', '0', 'p', '0',
                '0', '0', 'p', '0', '0', 'p', '0', 'p',
                '0', '0', '0', 'k', '0', '0', '0', '0'
            };
            // white attacks = 14
            // white protects = 6
            // black attacks = 14
            // black defends = 2
            // Evaluation: 100 * (6/14) - 100 * (2/14) = 28.5714285714
            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (!almost_equals(move_pick1.pawn_chain_eval(), 28.5714285714,  0.001)) {
                print_error("FAIL: Failed pawn chain evaluation test!");
                cout << "Received: " << move_pick1.pawn_chain_eval() << "\n";
                cout << "Expected: 28.5714285714...\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test pawn chains evaluation test!\n");
            }
        }

        void test_isolated_pawns_eval() {
            vector<char> char_board1 = {
                '0', '0', '0', '0', '0', 'K', '0', '0',
                'P', 'P', '0', '0', '0', '0', 'P', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', 'p', '0', '0', 'p', '0', '0', 'p',
                '0', 'k', '0', '0', '0', '0', '0', '0'
            };
            // Black has 3 isolated pawns, white has 4

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.isolated_pawns_eval() != -25) {
                print_error("FAIL: incorrect isolated pawns eval!");
                cout << "Received isolated pawns eval: " << move_pick1.isolated_pawns_eval() << "\n";
                cout << "Expected isolated pawns eval: -25" << "\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test isolated pawns eval!\n");
            }
        }

        void test_passed_pawns_eval() {
            vector<char> char_board1 = {
                '0', '0', '0', '0', '0', 'K', '0', '0',
                'P', 'P', '0', '0', '0', '0', 'P', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', 'p', '0', '0', 'p', '0', '0', '0',
                '0', 'k', '0', '0', '0', '0', '0', '0'
            };
            // Black has 1 passed pawn, white has none

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.passed_pawns_eval() != -75) {
                print_error("FAIL: incorrect passed pawns eval!");
                cout << "Received passed pawns eval: " << move_pick1.passed_pawns_eval() << "\n";
                cout << "Expected passed pawns eval: -75" << "\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test passed pawns eval!\n");
            }
        }

        void test_king_safety_eval() {
            vector<char> char_board1 = {
                '0', '0', '0', '0', '0', 'K', '0', '0',
                'P', 'P', '0', '0', '0', '0', 'P', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', 'P', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', 'q', '0',
                '0', 'p', '0', '0', 'p', 'k', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            // Black has 1 king defender, white has 3

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.king_safety_eval() != 60) {
                print_error("FAIL: incorrect king safety eval!");
                cout << "Received king safety eval: " << move_pick1.king_safety_eval() << "\n";
                cout << "Expected king safety eval: 60" << "\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test king safety eval!\n");
            }
        }

        void test_get_best_move1() {
            vector<char> char_board1 = {
                'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                '0', 'P', 'P', 'P', '0', 'P', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'P',
                'P', '0', '0', '0', 'P', '0', 'n', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', 'b', '0', 'r'
            };

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            Move to_play = move_pick1.get_best_move(5, true);

            bool fail = false;
            if (!(to_play.get_from() == 30) || !(to_play.get_to() == 13)) {
                print_error("FAIL: find best move 1!");
                cout << "Received to: " << to_play.get_to() << "\n";
                cout << "Expected to: 13" << "\n";
                cout << "Received from: " << to_play.get_from() << "\n";
                cout << "Expected from: 30" << "\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: find best move 1!\n");
            }
        }

        void test_get_best_move2() {
            vector<char> char_board1 = {
                'R', 'N', 'B', 'Q', 'K', '0', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', '0', 'B', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', 'N', '0',
                '0', '0', '0', '0', '0', '0', '0', 'p',
                '0', 'p', 'p', 'p', '0', 'p', 'p', '0',
                'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'
            };

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::BLACK, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            Move to_play = move_pick1.get_best_move(5, true);

            bool fail = false;
            if (!(to_play.get_from() == 38) || !(to_play.get_to() == 53)) {
                print_error("FAIL: find best move 2!");
                cout << "Received to: " << to_play.get_to() << "\n";
                cout << "Expected to: 53" << "\n";
                cout << "Received from: " << to_play.get_from() << "\n";
                cout << "Expected from: 38" << "\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: find best move 2!\n");
            }
        }

        void test_get_best_move_checkmate_depth_1() {
            // Test the engine's ability to find mate in 1
            vector<char> char_board1 = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', '0', '0', 'P',
                'p', '0', '0', '0', 'P', '0', 'n', '0',
                '0', '0', '0', '0', 'p', '0', 'N', '0',
                '0', '0', '0', '0', '0', 'q', '0', 'p',
                '0', 'p', 'p', 'p', '0', 'p', 'p', '0',
                'r', 'n', 'b', '0', 'k', 'b', '0', 'r'
            };

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            Move to_play = move_pick1.get_best_move(5, true);

            bool fail = false;
            // We expect the white queen to mate the black king
            if (!(to_play.get_from() == 45) || !(to_play.get_to() == 13)) {
                print_error("FAIL: find best move checkmate from a depth of 1!");
                cout << "Received to: " << to_play.get_to() << "\n";
                cout << "Expected to: 13" << "\n";
                cout << "Received from: " << to_play.get_from() << "\n";
                cout << "Expected from: 45" << "\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: find best move checkmate from a depth of 1!\n");
            }
        }

        void test_get_best_move_checkmate_depth_2() {
            // Test the engine's ability to find mate in 1
            vector<char> char_board1 = {
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'K', '0',
                '0', 'r', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'r', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'k', '0', '0', '0'
            };

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            Move to_play1 = move_pick1.get_best_move(5, true);

            bool fail = false;
            // We expect the white queen to mate the black king
            if (!(to_play1.get_from() == 43) || !(to_play1.get_to() == 11)) {
                print_error("FAIL: find best move checkmate from a depth of 2!");
                cout << "Received to: " << to_play1.get_to() << "\n";
                cout << "Expected to: 11" << "\n";
                cout << "Received from: " << to_play1.get_from() << "\n";
                cout << "Expected from: 43" << "\n";
                fail = true;
            }

            vector<char> char_board2 = {
                '0', '0', '0', '0', '0', 'K', '0', '0',
                '0', '0', '0', 'r', '0', '0', '0', '0',
                '0', 'r', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'k', '0', '0', '0'
            };

            vector<int> piece_board2 = char_board_to_piece_board(char_board2);
            MoveGenRec* move_gen2 = new MoveGenRec(init, gen, color::WHITE, piece_board2, 0ULL, 0xf);
            MovePickRec move_pick2 = MovePickRec(init, gen, move_gen2);

            Move to_play2 = move_pick2.get_best_move(5, true);

            if (!(to_play2.get_from() == 17) || !(to_play2.get_to() == 1)) {
                print_error("FAIL: find best move checkmate from a depth of 2!");
                cout << "Received to: " << to_play2.get_to() << "\n";
                cout << "Expected to: 1" << "\n";
                cout << "Received from: " << to_play2.get_from() << "\n";
                cout << "Expected from: 17" << "\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: find best move checkmate from a depth of 2!\n");
            }
        }

        void test_avoid_stalemate() {
            vector<char> char_board1 = {
                'K', '0', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', '0', '0', 'R', '0', '0',
                'b', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'n', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'b', '0', '0', '0',
                '0', '0', 'k', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };

            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            MoveGenRec* move_gen1 = new MoveGenRec(init, gen, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePickRec move_pick1 = MovePickRec(init, gen, move_gen1);

            Move to_play1 = move_pick1.get_best_move(5, true);

            bool fail = false;
            // We need to make sure we don't take the black rook leading to stalemate
            if ((to_play1.get_from() == 30) && (to_play1.get_to() == 13)) {
                print_error("FAIL: test avoid stalemate!");
                cout << "Received to: " << to_play1.get_to() << "\n";
                cout << "Expected to: not 13" << "\n";
                cout << "Received from: " << to_play1.get_from() << "\n";
                cout << "Expected from: not 30" << "\n";
                fail = true;
            }         
            if (!fail) {
                print_success("PASS: test avoid stalemate!\n");
            }   
        }
};

int main () {
    Initialize* init = new Initialize();
    init->init_all();
    Generate* gen = new Generate(*init);

    MovePickRecTest move_pick_test = MovePickRecTest(init, gen);
    move_pick_test.test_all();
}