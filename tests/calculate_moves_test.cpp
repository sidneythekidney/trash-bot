#include <iostream>
#include "../defs.h"
#include "../move.h"
#include "../generate.h"
#include "../move_gen.h"

using namespace std;

class MoveTest {
    public:
        MoveTest(Initialize* init, Generate* gen) : init(init), gen(gen) {}
        void test_all() {
            test_first_moves_depth_1_white();
            test_first_moves_depth_1_black();
            // Castle tests
            test_white_castle_ks_no_check_no_blockers();
            test_white_castle_ks_no_check_with_blockers();
            test_white_castle_ks_with_check_no_blockers();
            test_white_castle_qs_no_check_no_blockers();
            test_black_castle_ks_no_check_no_blockers();
            test_black_castle_qs_no_check_no_blockers();
            test_black_castle_ks_with_check_no_blockers();
            test_white_castle_qs_with_check_no_blockers();
            test_black_castle_qs_with_check_no_blockers();
            // en passant tests
            test_basic_white_en_passant();
            test_double_white_en_passant();
            test_double_black_en_passant();
            test_white_en_passant_does_not_induce_check();
            test_black_en_passant_does_not_induce_check();
            // pin tests
            // check tests
            // block tests
            // checkmate tests
            // stalemate tests
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

        void test_first_moves_depth_1_white() {
            // Create the MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE);
            // Now move_en contains the start position:
            bool fail = false;
            if (move_gen.num_calculated_moves() != 0) {
                print_error("Failed test_first_move_depth_1: number of initial moves != 0\n");
                cout << "Expected: 0, Received: " << move_gen.num_calculated_moves() << "\n"; 
                fail = true;
            }
            // Calculate starting moves for white:
            move_gen.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if 20 moves is calculated for opening white moves
            if (move_gen.num_calculated_moves() != 20) {
                print_error("Failed test_first_move_depth_1: number of opening white moves != 20\n");
                cout << "Expected: 20, Received: " << move_gen.num_calculated_moves() << "\n"; 
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_first_moved_depth_1_white");
            }
        }

        void test_first_moves_depth_1_black() {
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK);
            bool fail = false;
            if (move_gen.num_calculated_moves() != 0) {
                print_error("Failed test_first_move_depth_1: number of initial moves != 0\n");
                cout << "Expected: 0, Received: " << move_gen.num_calculated_moves() << "\n"; 
                fail = true;
            }
            // Calculate starting moves for white:
            move_gen.calculate_moves();
            // Check if 20 moves is calculated for opening white moves
            if (move_gen.num_calculated_moves() != 20) {
                print_error("Failed test_first_move_depth_1: number of opening white moves != 20\n");
                cout << "Expected: 20, Received: " << move_gen.num_calculated_moves() << "\n"; 
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_first_moved_depth_1_black");
            }
        }

        void test_white_castle_ks_no_check_no_blockers() {
            // Test with no check and no blockers

            // Create the piece init
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castles is generated
            if (!move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 62)) {
                print_error("Failed test_castle_white_ks_no_check_no_blockers: did not generate castle!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_white_ks_no_check_no_blockers");
            }
        }

        void test_white_castle_ks_no_check_with_blockers() {
            // Test with no check and with blockers
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', 'P',
                '0', '0', '0', '0', '0', 'N', '0', '0',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', 'n', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is generated
            if (move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 62)) {
                print_error("Failed test_castle_white_ks_no_check_with_blockers: incorrectly generated castle!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_white_castle_ks_no_check_with_blockers");
            }
        }

        void test_white_castle_ks_with_check_no_blockers() {
            // Test with check and no blockers
            vector<char> char_board = {
                'R', 'N', '0', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', 'P',
                '0', '0', '0', '0', '0', 'N', '0', '0',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'B', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };

            vector<int> piece_board = char_board_to_piece_board(char_board);
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is generated
            if (move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 62)) {
                print_error("Failed test_castle_white_ks_with_check_no_blockers: incorrectly generated castle!\n");
                fail = true;
            }
            else if (move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 52) || \
                     move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 61)) 
                {
                    print_error("Failed test_castle_white_ks_with_check_no_blockers: incorrectly generated king moves!\n");
                    fail = true;
                }
            if (!fail) {
                print_success("PASS: test_castle_white_ks_with_check_no_blockers");
            }
        }

        void test_white_castle_qs_no_check_no_blockers() {
            // Test with no check and no blockers

            // Create the piece init
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', 'b', 'n', '0', '0',
                'p', 'p', 'p', '0', '0', 'p', 'p', 'p',
                'r', '0', '0', '0', 'k', '0', '0', 'r'
            };
            // NOTE: White bishop shouldn't prevent white king from castling qs
            vector<int> piece_board = char_board_to_piece_board(char_board);
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is not generated
            if (!move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 58)) {
                print_error("Failed test_castle_white_qs_no_check_no_blockers: did not generate castle!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_white_qs_no_check_no_blockers");
            }
        }

        void test_black_castle_ks_no_check_no_blockers() {
            // Test with no check and no blockers

            // Create the piece init
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', '0', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is not generated
            if (!move_gen.check_if_move_calculated(Move::BLACK_KING, 4, 6)) {
                print_error("Failed test_black_castle_ks_no_check_no_blockers: did not generate castle!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_black_castle_ks_no_check_no_blockers");
            }
        }

        void test_black_castle_qs_no_check_no_blockers() {
            // Test with no check and no blockers

            // Create the piece init
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            // Create MoveGen object:
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is not generated
            if (!move_gen.check_if_move_calculated(Move::BLACK_KING, 4, 2)) {
                print_error("Failed test_castle_black_qs_no_check_no_blockers: did not generate castle!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_castle_black_qs_no_check_no_blockers");
            }
        }

        void test_black_castle_ks_with_check_no_blockers() {
            // Test black cannot castle ks with check and no blockers
            // Create the character board
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'n',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            // NOTE: The knight prevents the castle of the king
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is not generated
            if (move_gen.check_if_move_calculated(Move::BLACK_KING, 4, 6)) {
                print_error("Failed test_castle_black_ks_with_check_no_blockers: generate castle through check!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_castle_black_ks_with_check_no_blockers");
            }
        }

        void test_white_castle_qs_with_check_no_blockers() {
            vector<char> char_board = {
                '0', '0', 'Q', '0', 'K', '0', '0', 'R',
                'P', 'P', '0', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'n',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'b', '0', '0',
                'p', 'p', '0', 'p', '0', 'p', 'p', 'p',
                'r', '0', '0', '0', 'k', '0', '0', 'r'
            };
            // NOTE: The black queen prevents the white king from castling qs

            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            
            // Check if the castle is not generated
            if (move_gen.check_if_move_calculated(Move::WHITE_KING, 60, 58)) {
                print_error("Failed test_castle_white_qs_with_check_no_blockers: generate castle through check!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_castle_white_qs_with_check_no_blockers");
            }
        }

        void test_black_castle_qs_with_check_no_blockers() {
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', '0', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'n',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'b', '0', '0',
                'p', 'p', '0', 'p', '0', 'p', 'p', 'p',
                '0', 'k', 'r', '0', '0', '0', '0', 'r'
            };
            // NOTE: The white rook prevents the black king from castling qs
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            move_gen.calculate_moves();
            // Check if the castle is not generated
            if (move_gen.check_if_move_calculated(Move::BLACK_KING, 4, 2)) {
                print_error("Failed test_castle_black_qs_with_check_no_blockers: generate castle through check!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_castle_black_qs_with_check_no_blockers");
            }
        }

        void test_basic_white_en_passant() {
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', 'P', 'p', '0', '0', '0',
                '0', '0', 'b', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            U64 en_passant = 1ULL << 27; // the square with the pawn that can be en passanted
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, en_passant, 0xf);
            bool fail = false;
            move_gen.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (!move_gen.check_if_move_calculated(Move::WHITE_PAWN, 28, 19)) {
                print_error("Failed test_basic_white_en_passant: did not generate en passant!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_basic_white_en_passant");
            }
        }

        void test_basic_black_en_passant() {
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'b', 'P', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            U64 en_passant = 1ULL << 36; // the square with the pawn that can be en passanted
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK, piece_board, en_passant, 0xf);
            bool fail = false;
            move_gen.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (!move_gen.check_if_move_calculated(Move::BLACK_PAWN, 35, 44)) {
                print_error("Failed test_basic_black_en_passant: did not generate en passant!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_basic_black_en_passant");
            }
        }

        void test_double_white_en_passant() {
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', 'p', 'P', 'p', '0', '0', '0',
                '0', '0', 'b', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', '0', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            U64 en_passant = 1ULL << 27; // the square with the pawn that can be en passanted (captured)
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE, piece_board, en_passant, 0xf);
            bool fail = false;
            move_gen.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if either en passant is not generated
            if (!move_gen.check_if_move_calculated(Move::WHITE_PAWN, 28, 19) || 
                !move_gen.check_if_move_calculated(Move::WHITE_PAWN, 26, 19)) {
                // Missing one or both of the en_passants
                print_error("Failed test_double_white_en_passant: did not generate en passant!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_double_white_en_passant");
            }
        }

        void test_double_black_en_passant() {
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', '0', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'P', 'p', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', '0', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            U64 en_passant = 1ULL << 35; // the square with the pawn that can be en passanted (captured)
            MoveGen move_gen = MoveGen(init, gen, 1, color::BLACK, piece_board, en_passant, 0xf);
            bool fail = false;
            move_gen.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if either en passant is not generated
            if (!move_gen.check_if_move_calculated(Move::BLACK_PAWN, 34, 43) || 
                !move_gen.check_if_move_calculated(Move::BLACK_PAWN, 36, 43)) {
                // Missing one or both of the en_passants
                print_error("Failed test_double_black_en_passant: did not generate en passant!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_double_black_en_passant");
            }
        }

        void test_white_en_passant_does_not_induce_check() {
            // Test 1: moving white pawn exposes white king
            vector<char> char_board1 = {
                '0', '0', 'K', '0', 'R', '0', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', 'P', 'p', '0', '0', '0',
                '0', '0', 'b', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            // NOTE: taking en passant exposes the white king to the black rook
            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            U64 en_passant = 1ULL << 27; // the square with the pawn that can be en passanted
            MoveGen move_gen1 = MoveGen(init, gen, 1, color::WHITE, piece_board1, en_passant, 0xf);
            bool fail = false;
            move_gen1.calculate_moves();
            // Check if the en passant is not generated
            if (move_gen1.check_if_move_calculated(Move::WHITE_PAWN, 28, 19)) {
                print_error("Failed test_white_en_passant_does_not_induce_check: generated en passant induces check (black rook)!\n");
                return;
            }
            // Test 2: capturing black pawn exposes white king
            vector<char> char_board2 = {
                '0', '0', 'K', '0', 'R', '0', '0', 'R',
                'P', 'P', 'P', '0', '0', 'P', 'P', '0',
                '0', '0', 'Q', '0', '0', 'N', '0', 'P',
                '0', '0', '0', 'P', 'p', '0', 'n', '0',
                '0', '0', 'b', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', '0', 'p',
                'r', 'n', 'b', 'q', '0', 'r', '0', 'k'
            };
            // NOTE: taking en passant exposes the white king to the black queen
            vector<int> piece_board2 = char_board_to_piece_board(char_board2);
            MoveGen move_gen2 = MoveGen(init, gen, 1, color::WHITE, piece_board2, en_passant, 0xf);
            move_gen2.calculate_moves();
            // Check if the en passant is not generated
            if (move_gen2.check_if_move_calculated(Move::WHITE_PAWN, 28, 19)) {
                print_error("Failed test_white_en_passant_does_not_induce_check: generated en passant induces check (black queen)!\n");
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test_white_en_passant_does_not_induce_check");
            }
        }
        void test_black_en_passant_does_not_induce_check() {
            // Test 1: Moving the black pawn exposes the black king
            vector<char> char_board1 = {
                'R', '0', '0', '0', '0', 'R', '0', 'K',
                'P', 'P', 'P', '0', '0', 'P', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'P', 'p', '0', 'n', '0',
                '0', '0', 'b', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            U64 en_passant1 = 1ULL << 36; // the square with the pawn that can be en passanted
            MoveGen move_gen1 = MoveGen(init, gen, 1, color::BLACK, piece_board1, en_passant1, 0xf);
            bool fail = false;
            move_gen1.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (move_gen1.check_if_move_calculated(Move::BLACK_PAWN, 35, 44)) {
                print_error("Failed test_black_en_passant_does_not_induce_check: generated en passant induces check (white bishop)!\n");
                return;
            }

            // Test 2: Capturing the white pawn exposes the black king
            vector<char> char_board2 = {
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'q', '0', '0', 'p', 'P', '0', '0', 'K',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'k', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board2 = char_board_to_piece_board(char_board2);
            U64 en_passant2 = 1ULL << 35; // the square with the pawn that can be en passanted
            MoveGen move_gen2 = MoveGen(init, gen, 1, color::BLACK, piece_board2, en_passant2, 0xf);
            move_gen2.calculate_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (move_gen2.check_if_move_calculated(Move::BLACK_PAWN, 36, 43)) {
                print_error("Failed test_black_en_passant_does_not_induce_check: generated en passant induces check (white queen)!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_black_en_passant_does_not_induce_check");
            }
        }
};

int main() {
    // Initialize everything to start MoveTests
    Initialize* init = new Initialize();
    init->init_all();
    Generate* gen = new Generate(*init);

    MoveTest test = MoveTest(init, gen);
    test.test_all();

    delete init;
    delete gen;
}   