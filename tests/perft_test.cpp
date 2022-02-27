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
            // Run test for Perft with checks:
            test_perft_check();
            // Run test for Perft with castling:
            test_perft_castle();
            // Run test for Perft with promotion:
            test_perft_promotion();
            // Run test for Perft with checkmate
            test_perft_checkmate();
            // Run test for Perft with stalemate
            test_perft_stalemate();
            // debug test
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
                print_error("FAIL test_perft_from_start_depth_1: generated the incorrect number of moves from depth=1!");
                cout << "Expected: 20\n";
                cout << "Received: " << num_moves << "\n";
                // move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_1");
            }
            cout << "final piece config:\n";
            move_gen->print_piece_board();
        }

        void test_perft_from_start_depth_2() {
            MoveGen* move_gen = new MoveGen(init, gen, 2, color::WHITE);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 400) {
                fail = true;
                print_error("FAIL test_perft_from_start_depth_2: generated the incorrect number of moves from depth=2!");
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
                print_error("FAIL test_perft_from_start_depth_3: generated the incorrect number of moves from depth=3!");
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
                print_error("FAIL test_perft_from_start_depth_4: generated the incorrect number of moves from depth=4!");
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
                print_error("FAIL test_perft_from_start_depth_5: generated the incorrect number of moves from depth=5!");
                cout << "Expected: 4865609\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_from_start_depth_5");
            }
        }

        void test_perft_check() {
            /*
            Set position on StockFish:
            position fen r1bqkbnr/1ppp1ppp/p1n5/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR w KQkq - 2 2
            go perft 3
            */
            vector<char> char_board = {
                'R', '0', 'B', 'Q', 'K', 'B', 'N', 'R',
                '0', 'P', 'P', 'P', '0', 'P', 'P', 'P',
                'P', '0', 'N', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'q', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', '0', 'k', '0', 'n', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen* move_gen = new MoveGen(init, gen, 4, color::WHITE, piece_board, 0ULL, 0xf);
            move_gen->print_piece_board();
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 1533700) {
                fail = true;
                print_error("FAIL test_perft_check: generated the incorrect number of moves from depth=4!");
                cout << "Expected: 1533700\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_check");
            }
        }

        void test_perft_castle() {
            /*
            Set position on StockFish:
            position fen rnbqkb1r/ppp1ppp1/5n1p/3p4/2B1P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 2
            go perft 4
            */
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', '0', 'P', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', 'P', '0', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen* move_gen = new MoveGen(init, gen, 4, color::WHITE, piece_board, 0ULL, 0xf);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 1088397) {
                fail = true;
                print_error("FAIL test_perft_castle: generated the incorrect number of moves from depth=4!");
                cout << "Expected: 1088397\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_castle");
            }

            // TODO: Add more tests for different possible castles:

        }

        void test_perft_promotion() {
            /*
            Set position on StockFish:
            position fen 1K6/ppp2P2/1q6/8/7r/8/8/1k6 w - - 2 2
            go perft 5
            */
             vector<char> char_board = {
                '0', 'k', '0', '0', '0', '0', '0', '0',
                'P', 'P', 'P', '0', '0', 'p', '0', '0',
                '0', 'Q', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'R',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', 'K', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen* move_gen = new MoveGen(init, gen, 5, color::WHITE, piece_board, 0ULL, 0);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 1405997) {
                fail = true;
                print_error("FAIL test_perft_promotion: generated the incorrect number of moves from depth=5!");
                cout << "Expected: 1405997\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_promotion");
            }
        }

        void test_perft_checkmate() {
        /*
        Set position on StockFish:
        position fen k1q1r3/8/8/8/8/8/PP6/1K6 b - - 2 2
        go perft 4
        */
            vector<char> char_board = {
                'K', '0', 'Q', '0', 'R', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', '0', '0', '0', '0', '0', '0',
                '0', 'k', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen* move_gen = new MoveGen(init, gen, 4, color::BLACK, piece_board, 0ULL, 0);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 28185) {
                fail = true;
                print_error("FAIL test_perft_checkmate: generated the incorrect number of moves from depth=4!");
                cout << "Expected: 28185\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_checkmate");
            }
        }

        void test_perft_stalemate() {
            /*
            Set position on StockFish
            position fen k7/P7/8/8/3Q4/8/PP6/1K6 w - - 2 2
            go perft 5
            */
            vector<char> char_board = {
                'K', '0', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'q', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', '0', '0', '0', '0', '0', '0',
                '0', 'k', '0', '0', '0', '0', '0', '0'
            };

            vector<int> piece_board = char_board_to_piece_board(char_board);
            cout << "creating move gen object\n";
            MoveGen* move_gen = new MoveGen(init, gen, 5, color::WHITE, piece_board, 0ULL, 0);
            cout << "created move_gen object\n";
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();
            cout << "Obtained number of moves\n";
            bool fail = false;
            if (num_moves != 174694) {
                fail = true;
                print_error("FAIL test_perft_stalemate: generated the incorrect number of moves from depth=5!");
                cout << "Expected: 174694\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
            }
            if (!fail) {
                print_success("PASS: test_perft_stalemate");
            }
        }

        void test_perft_debug() {
            /*
            Set position on StockFish:
            position fen 8/Kp3P2/2p5/8/7r/8/1q6/1k6 w - - 2 2
            go perft 1
            */
            vector<char> char_board = {
                '0', '0', '0', '0', '0', '0', '0', '0',
                'k', 'P', '0', '0', '0', 'p', '0', '0',
                '0', '0', 'P', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'R',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', 'Q', '0', '0', '0', '0', '0', '0',
                '0', 'K', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen* move_gen = new MoveGen(init, gen, 1, color::WHITE, piece_board, 0ULL, 0);
            Perft perft = Perft(move_gen);
            int num_moves = perft.calculate_num_moves();

            bool fail = false;
            if (num_moves != 6) {
                fail = true;
                print_error("FAIL test_perft_promotion: generated the incorrect number of moves from depth=1!");
                cout << "Expected: 6\n";
                cout << "Received: " << num_moves << "\n";
                move_gen->print_cur_moves();
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