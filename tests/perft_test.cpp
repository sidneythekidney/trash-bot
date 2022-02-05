#include <iostream>
#include "../defs.h"
#include "../move.h"
#include "../generate.h"
#include "../move_gen.h" 
#include "../perft.h"

using namespace std;

// NOTE: num moves at certain depth comes from https://www.chess.com/forum/view/general/chess-combinatorics-number-of-legal-positions-after-gt2-turns

class PerftTest {
    public:
        PerftTest(Initialize* init, Generate* gen) : init(init), gen(gen) {};
        void test_all() {
            // Run initial test:
            test_init();
            // Run tests for depth up to 6 from start position
            test_perft_from_start_depth_1();
            test_perft_from_start_depth_2();
            test_perft_from_start_depth_3();
            test_perft_from_start_depth_4();
            test_perft_from_start_depth_5();
            test_perft_debug();
        };
    
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
            print_success("PASS: test_init");
        }

        void test_perft_from_start_depth_1() {
            MoveGen* move_gen = new MoveGen(init, gen, 1, color::WHITE);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 20) {
                fail = true;
                print_error("Failed test_perft_from_start_depth_1: generated the incorrect number of moves from depth=1!");
                cout << "Expected: 20\n";
                cout << "Received: " << num_moves << "\n";
                // move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_1");
            }
        }

        void test_perft_from_start_depth_2() {
            MoveGen* move_gen = new MoveGen(init, gen, 2, color::WHITE);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 400) {
                fail = true;
                print_error("Failed test_perft_from_start_depth_2: generated the incorrect number of moves from depth=2!");
                cout << "Expected: 400\n";
                cout << "Received: " << num_moves << "\n";
                // move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_2");
            }
        }

        void test_perft_from_start_depth_3() {
            MoveGen* move_gen = new MoveGen(init, gen, 3, color::WHITE);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 8902) {
                fail = true;
                print_error("Failed test_perft_from_start_depth_3: generated the incorrect number of moves from depth=3!");
                cout << "Expected: 8902\n";
                cout << "Received: " << num_moves << "\n";
                // move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_3");
            }
        }

        void test_perft_from_start_depth_4() {
            MoveGen* move_gen = new MoveGen(init, gen, 4, color::WHITE);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 197281) {
                fail = true;
                print_error("Failed test_perft_from_start_depth_4: generated the incorrect number of moves from depth=4!");
                cout << "Expected: 197281\n";
                cout << "Received: " << num_moves << "\n";
                // move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_4");
            }
        }

        void test_perft_from_start_depth_5() {
            MoveGen* move_gen = new MoveGen(init, gen, 5, color::WHITE);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 4865609) {
                fail = true;
                print_error("Failed test_perft_from_start_depth_5: generated the incorrect number of moves from depth=5!");
                cout << "Expected: 4865609\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_5");
            }
        }

        void test_perft_debug() {
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                'P', 'P', '0', 'P', 'P', 'P', 'P', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'p', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', '0', 'p', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen* move_gen = new MoveGen(init, gen, 2, color::BLACK, piece_board, 0ULL, 0xf);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();
            
            bool fail = false;
            if (num_moves != 646) {
                fail = true;
                print_error("Failed test_perft_debug: generated the incorrect number of moves from depth=2!");
                cout << "Expected: 646\n";
                cout << "Received: " << num_moves << "\n";
                // move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_debug");
            }
        }
};

int main() {
    Initialize* init = new Initialize();
    init->init_all();
    Generate* gen = new Generate(*init);

    PerftTest perft_test = PerftTest(init, gen);
    perft_test.test_all();

    delete init;
    delete gen;
}