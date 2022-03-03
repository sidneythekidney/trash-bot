// Unit tests for the MovePick class

#include <iostream>
#include "../generate.h"
#include "../utils.hpp"
#include "../move_gen.h"
#include "../move_pick.h"

using namespace std;

class MovePickTest {
    public:
        MovePickTest(Initialize* init, Generate* gen) : init(init), gen(gen) {}
        
        void test_all() {
            // Run all test functions inside this function:
            test_init();
            test_init_move_pick();
            test_material_eval();
            test_position_eval();
            test_pawn_strucuture_eval();
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
            MoveGen* move_gen = new MoveGen(init, gen, 3, color::WHITE);
            MovePick move_pick = MovePick(init, gen, move_gen);

            move_pick.find_best_move_given_time(10);

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
            MoveGen* move_gen1 = new MoveGen(init, gen, 1, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePick move_pick1 = MovePick(init, gen, move_gen1);

            bool fail = false;
            if (move_pick1.material_eval() != -900) {
                print_error("FAIL: Failed material evaluation test!");
                cout << "Received: " << move_pick1.material_eval() << "\n";
                cout << "Expected: -900\n";
                fail = true;
            }

            // Test the material evaluation from the starting position
            MoveGen* move_gen2 = new MoveGen(init, gen, 1, color::WHITE);
            MovePick move_pick2 = MovePick(init, gen, move_gen2);

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
            MoveGen* move_gen1 = new MoveGen(init, gen, 1, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePick move_pick1 = MovePick(init, gen, move_gen1);

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

        void test_pawn_strucuture_eval() {
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
            MoveGen* move_gen1 = new MoveGen(init, gen, 1, color::WHITE, piece_board1, 0ULL, 0xf);
            MovePick move_pick1 = MovePick(init, gen, move_gen1);

            move_gen1->print_piece_board();

            bool fail = false;
            if (!almost_equals(move_pick1.pawn_structure_eval(), 28.5714285714,  0.001)) {
                print_error("FAIL: Failed pawn structure evaluation test!");
                cout << "Received: " << move_pick1.pawn_structure_eval() << "\n";
                cout << "Expected: 28.5714285714...\n";
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test pawn structure evaluation test!\n");
            }
        }
};

int main () {
    Initialize* init = new Initialize();
    init->init_all();
    Generate* gen = new Generate(*init);

    MovePickTest move_pick_test = MovePickTest(init, gen);
    move_pick_test.test_all();
}