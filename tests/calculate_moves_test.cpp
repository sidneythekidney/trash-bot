#include <iostream>
#include "../defs.h"
#include "../move.h"
#include "../generate.h"
#include "../move_gen_rec.h"

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
            test_standard_white_pin();
            test_standard_black_pin();
            // block tests
            test_standard_white_block();
            test_en_passant_white_block();
            test_standard_black_block();
            test_en_passant_black_block();
            // checkmate tests
            test_standard_white_checkmate();
            test_white_checkmate_suffocation();
            test_standard_black_checkmate();
            test_black_checkmate_suffocation();
            // stalemate tests
            test_white_standard_stalemate();
            test_black_standard_stalemate();
            // double check tests
            test_white_double_check();
            test_black_double_check();
            // promotion tests:
            test_white_basic_promotion();
            test_black_basic_promotion();
            // additional tests
            test_white_pawn_cant_go_right_on_col_7();
            test_black_pawn_cant_go_right_on_col_0();
            test_black_pawn_cant_go_right_on_col_7();
            test_white_king_cant_go_left_on_col_0();
            test_white_king_cant_go_right_on_col_7();
            test_black_king_cant_go_right_on_col_0();
            test_black_king_cant_go_right_on_col_7();
            test_castle_on_second_rank();
            white_king_moves_into_black_pawn();
            black_king_moves_into_white_pawn();
            white_king_behind_black_pawns();
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

        bool check_if_move_calculated(const vector<Move> &legal_moves, unsigned int piece, unsigned int from, unsigned int to) {
            for (auto move : legal_moves) {
                // cout << "from: " << move.get_from() << " to: " << move.get_to() << "\n";
                if (move.get_moved() == piece && move.get_from() == from && move.get_to() == to) {
                    return true;
                }
            }
            return false;
        }

        void test_first_moves_depth_1_white() {
            // Create the MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE);
            // Now move_en contains the start position:
            bool fail = false;
            // Calculate starting moves for white:
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if 20 moves is calculated for opening white moves
            if (legal_moves.size() != 20) {
                print_error("Failed test_first_move_depth_1: number of opening white moves != 20\n");
                cout << "Expected: 20, Received: " << legal_moves.size() << "\n"; 
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_first_moved_depth_1_white");
            }
        }

        void test_first_moves_depth_1_black() {
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK);
            bool fail = false;
            // Calculate starting moves for white:
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if 20 moves is calculated for opening white moves
            if (legal_moves.size() != 20) {
                print_error("Failed test_first_moves_depth_1_black: number of opening black moves != 20\n");
                cout << "Expected: 20, Received: " << legal_moves.size() << "\n"; 
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
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castles is generated
            if (!check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 62)) {
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
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is generated
            if (check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 62)) {
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
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is generated
            if (check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 62)) {
                print_error("Failed test_castle_white_ks_with_check_no_blockers: incorrectly generated castle!\n");
                fail = true;
            }
            else if (check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 52) || \
                     check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 61)) 
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
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is not generated
            if (!check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 58)) {
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
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is not generated
            if (!check_if_move_calculated(legal_moves, Move::BLACK_KING, 4, 6)) {
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
            // Create MoveGenRec object:
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is not generated
            if (!check_if_move_calculated(legal_moves, Move::BLACK_KING, 4, 2)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is not generated
            if (check_if_move_calculated(legal_moves, Move::BLACK_KING, 4, 6)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            
            // Check if the castle is not generated
            if (check_if_move_calculated(legal_moves, Move::WHITE_KING, 60, 58)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            // NOTE: 0xf is used since we assume all 4 castles are legal for this position
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the castle is not generated
            if (check_if_move_calculated(legal_moves, Move::BLACK_KING, 4, 2)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (!check_if_move_calculated(legal_moves, Move::WHITE_PAWN, 28, 19)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (!check_if_move_calculated(legal_moves, Move::BLACK_PAWN, 35, 44)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if either en passant is not generated
            if (!check_if_move_calculated(legal_moves, Move::WHITE_PAWN, 28, 19) || 
                !check_if_move_calculated(legal_moves, Move::WHITE_PAWN, 26, 19)) {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if either en passant is not generated
            if (!check_if_move_calculated(legal_moves, Move::BLACK_PAWN, 34, 43) || 
                !check_if_move_calculated(legal_moves, Move::BLACK_PAWN, 36, 43)) {
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
            MoveGenRec move_gen1 = MoveGenRec(init, gen, color::WHITE, piece_board1, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves1 = move_gen1.get_legal_moves();
            // Check if the en passant is not generated
            if (check_if_move_calculated(legal_moves1, Move::WHITE_PAWN, 28, 19)) {
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
            MoveGenRec move_gen2 = MoveGenRec(init, gen, color::WHITE, piece_board2, en_passant, 0xf);
            vector<Move> legal_moves2 = move_gen2.get_legal_moves();
            // Check if the en passant is not generated
            if (check_if_move_calculated(legal_moves2, Move::WHITE_PAWN, 28, 19)) {
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
            MoveGenRec move_gen1 = MoveGenRec(init, gen, color::BLACK, piece_board1, en_passant1, 0xf);
            bool fail = false;
            vector<Move> legal_moves1 = move_gen1.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (check_if_move_calculated(legal_moves1, Move::BLACK_PAWN, 35, 44)) {
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
            MoveGenRec move_gen2 = MoveGenRec(init, gen, color::BLACK, piece_board2, en_passant2, 0xf);
            vector<Move> legal_moves2 = move_gen2.get_legal_moves();
            // move_gen.print_cur_moves();
            // Check if the en passant is not generated
            if (check_if_move_calculated(legal_moves2, Move::BLACK_PAWN, 36, 43)) {
                print_error("Failed test_black_en_passant_does_not_induce_check: generated en passant induces check (white queen)!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_black_en_passant_does_not_induce_check");
            }
        }

        // Pin tests:
        void test_standard_white_pin() {
            // Test if the white_knight can move given its pinned:
            vector<char> char_board = {
                '0', '0', 'Q', '0', '0', '0', '0', '0',
                '0', '0', 'n', 'P', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'K',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'k', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            vector<Move> legal_moves = move_gen.get_legal_moves();
            bool fail = false;
            if (check_if_move_calculated(legal_moves, Move::WHITE_KNIGHT, 10, 0)) {
                print_error("Failed test_standard_white_pin: white knight can move when pinned!\n");
                fail = true;
            }
            // Make sure the only moves generated are king moves:
            if (!(legal_moves.size() == 8)) {
                print_error("Failed test_standard_white_pin: incorrect number of moves generated!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_standard_white_pin");
            }
        }
        void test_standard_black_pin() {
             // Test if the black_rook can move given its pinned:
            vector<char> char_board = {
                '0', '0', 'Q', '0', '0', '0', '0', '0',
                '0', '0', 'n', 'P', 'b', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'R', '0',
                '0', '0', '0', '0', '0', '0', '0', 'K',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'k', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            vector<Move> legal_moves = move_gen.get_legal_moves();
            bool fail = false;
            if (check_if_move_calculated(legal_moves, Move::BLACK_ROOK, 30, 22)) {
                print_error("Failed test_standard_black_pin: black rook can move when pinned!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_standard_black_pin");
            }
        }
        void test_standard_white_block() {
            // Test white queen block:
            vector<char> char_board = {
                '0', '0', 'Q', '0', '0', '0', '0', '0',
                '0', '0', '0', 'P', 'b', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'R', '0',
                '0', '0', '0', '0', '0', '0', '0', 'K',
                '0', '0', '0', '0', 'q', '0', '0', '0',
                '0', '0', 'k', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            vector<Move> legal_moves = move_gen.get_legal_moves();
            bool fail = false;
            if (!check_if_move_calculated(legal_moves, Move::WHITE_QUEEN, 44, 42)) {
                print_error("Failed test_standard_white_block: not generating white queen!\n");
                fail = true;
            }
            // Make sure white queen can't move elsewhere (randomly selected square)
            if (check_if_move_calculated(legal_moves, Move::WHITE_QUEEN, 44, 36)) {
                print_error("Failed test_standard_white_block: generating incorrect queen move!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_standard_white_block");
            }
        }
        void test_en_passant_white_block() {
            // Test en passant to block check
            vector<char> char_board1 = {
                '0', '0', 'K', '0', 'R', '0', '0', 'R',
                '0', '0', '0', '0', '0', 'P', 'P', '0',
                '0', 'k', '0', '0', '0', 'R', '0', 'P',
                '0', '0', '0', 'P', 'p', '0', '0', '0',
                '0', '0', 'b', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', '0', '0', '0', 'r'
            };
            vector<int> piece_board1 = char_board_to_piece_board(char_board1);
            U64 en_passant = 1ULL << 27; // the square with the pawn that can be en passanted
            MoveGenRec move_gen1 = MoveGenRec(init, gen, color::WHITE, piece_board1, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen1.get_legal_moves();
            // Check if the en passant is not generated
            if (!check_if_move_calculated(legal_moves, Move::WHITE_PAWN, 28, 19)) {
                print_error("Failed test_white_en_passant_does_not_induce_check: generated en passant induces check (black rook)!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_en_passant_white_block");
            }
        }
        void test_standard_black_block() {
            // Test black rook block:
            vector<char> char_board = {
                '0', '0', '0', '0', '0', '0', 'R', '0',
                '0', '0', '0', 'P', 'b', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'b', '0',
                '0', '0', '0', '0', '0', '0', '0', 'K',
                '0', '0', '0', '0', 'q', '0', '0', '0',
                '0', '0', 'k', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            vector<Move> legal_moves = move_gen.get_legal_moves();
            bool fail = false;
            if (!check_if_move_calculated(legal_moves, Move::BLACK_ROOK, 6, 30)) {
                print_error("Failed test_standard_black_block: not generating black rook move!\n");
                fail = true;
            }
            // Make sure white queen can't move elsewhere (randomly selected square)
            if (check_if_move_calculated(legal_moves, Move::BLACK_ROOK, 6, 7)) {
                print_error("Failed test_standard_black_block: generating incorrect rook move!\n");
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_standard_black_block");
            }
        }
        void test_en_passant_black_block() {
            // Test en passant to block check
            vector<char> char_board = {
                '0', '0', '0', '0', 'R', '0', '0', 'R',
                '0', '0', '0', '0', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'R', '0', 'P',
                '0', 'K', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'p', 'P', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', '0', 'b', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', '0', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            U64 en_passant = 1ULL << 35; // the square with the pawn that can be en passanted
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, en_passant, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Check if the en passant is not generated
            if (!check_if_move_calculated(legal_moves, Move::BLACK_PAWN, 36, 43)) {
                print_error("Failed test_en_passant_black_block: not generating en passant block!\n");
                return;
            }
            // Make sure a non-blocking pawn move isn't generated
            if (check_if_move_calculated(legal_moves, Move::BLACK_PAWN, 36, 44)) {
                print_error("Failed test_en_passant_black_block: move induces check!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_en_passant_black_block");
            }
        }
        // checkmate tests
        void test_standard_white_checkmate() {
            // If black is checkmated there should be no moves:
            vector<char> char_board = {
                'R', '0', 'B', 'Q', 'K', 'B', '0', 'R',
                '0', 'P', 'P', 'P', '0', 'q', 'P', 'P',
                'P', '0', 'N', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', 'p', 'p', 'p', '0', 'p', 'p', 'p',
                'r', 'n', 'b', '0', 'k', '0', 'n', 'r'
            };
            // NOTE: The above is a scholar's move for white
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (legal_moves.size() != 0) {
                print_error("Failed test_standard_white_checkmate: found moves for black in checkmate!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_standard_white_checkmate");
            }
        }
        void test_white_checkmate_suffocation() {
            // If there is checkmate by suffocation, there should be no moves:
            vector<char> char_board = {
                'K', 'R', '0', '0', '0', '0', '0', 'B',
                'P', 'P', 'n', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'k', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (legal_moves.size() != 0) {
                print_error("Failed test_white_checkmate_suffocation: found moves for black in suffocation checkmate!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_white_checkmate_suffocation");
            }
        }
        void test_standard_black_checkmate() {
            // Test back rank checkmate for black:
            vector<char> char_board = {
                'K', 'R', '0', '0', '0', '0', '0', 'B',
                'P', 'P', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'b', '0', '0', '0', '0',
                'p', 'p', 'p', '0', '0', '0', '0', '0',
                '0', 'k', '0', '0', 'R', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (legal_moves.size() != 0) {
                print_error("Failed test_standard_black_checkmate: found moves for white in back-rank checkmate!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_standard_black_checkmate");
            }
        }
        void test_black_checkmate_suffocation() {
            vector<char> char_board = {
                'K', 'R', '0', '0', '0', '0', '0', 'B',
                'P', 'P', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', 'b', '0', '0', '0', '0',
                'p', 'p', 'p', '0', '0', 'N', 'p', 'p',
                '0', 'r', '0', '0', '0', '0', 'r', 'k'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (legal_moves.size() != 0) {
                print_error("Failed test_black_checkmate_suffocation: found moves for white in suffocation checkmate!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_black_checkmate_suffocation");
            }
        }
        // stalemate tests
        void test_white_standard_stalemate() {
            vector<char> char_board = {
                '0', 'R', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'K',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'R',
                'k', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (legal_moves.size() != 0) {
                print_error("Failed test_white_standard_stalemate: found moves for white in stalemate position!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_white_standard_stalemate");
            }
        }
        void test_black_standard_stalemate() {
            vector<char> char_board = {
                '0', '0', '0', 'K', '0', 'b', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', 'b', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'r', '0', '0', '0', '0', 'k'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (legal_moves.size() != 0) {
                print_error("Failed test_black_standard_stalemate: found moves for black in stalemate position!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_black_standard_stalemate");
            }
        }
        // Test double check:
        void test_white_double_check() {
            // When we have double check the only moves we can make are king moves:
            vector<char> char_board = {
                '0', '0', 'K', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'b', '0', '0', '0', '0', 'B',
                '0', '0', '0', '0', 'R', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', 'k', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (check_if_move_calculated(legal_moves, Move::WHITE_BISHOP, 34, 52)) {
                print_error("Failed test_white_double_check: found non-blocking moves for white in check!\n");
                return;
            }
            if (legal_moves.size() != 3) {
                print_error("Failed test_white_double_check: incorrect number of moves generated!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_white_double_check");
            }
        }
        void test_black_double_check() {
            vector<char> char_board = {
                '0', '0', 'K', '0', '0', '0', '0', '0',
                '0', '0', '0', 'b', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', 'r', '0', '0', '0', '0', 'B',
                '0', '0', '0', '0', 'R', '0', '0', '0',
                '0', 'k', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (check_if_move_calculated(legal_moves, Move::BLACK_ROOK, 44, 41)) {
                print_error("Failed test_black_double_check: found non-blocking moves for black in check!\n");
                return;
            }
            if (legal_moves.size() != 4) {
                print_error("Failed test_black_double_check: incorrect number of moves generated!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_black_double_check");
            }
        }

        void test_white_basic_promotion() {
            vector<char> char_board = {
                '0', '0', 'k', '0', '0', '0', '0', 'R',
                'R', '0', '0', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', 'K', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (!check_if_move_calculated(legal_moves, Move::WHITE_QUEEN, 12, 4)) {
                print_error("Failed test_white_basic_promotion: did not generate white pawn promotion to queen!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_white_basic_promotion");
            }
        }

        void test_black_basic_promotion() {
            vector<char> char_board = {
                '0', '0', 'k', '0', '0', '0', '0', '0',
                'R', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', 'p', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', 'K', '0', 'P', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (!check_if_move_calculated(legal_moves, Move::BLACK_KNIGHT, 51, 59)) {
                print_error("Failed test_knight_basic_promotion: did not generate black pawn promotion to knight!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_black_basic_promotion");
            }
        }

        void test_rook_does_not_move_through_pieces() {
            vector<char> char_board = {
                '0', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                '0', 'P', 'P', 'P', 'P', 'P', 'P', 'P',
                'P', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'n',
                'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', 'b', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (check_if_move_calculated(legal_moves, Move::WHITE_ROOK, 56, 62)) {
                print_error("Failed test_rook_does_not_move_through_pieces: generated rook move through friendly pieces!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_rook_does_not_move_through_pieces");
            }
        }

        void test_white_pawn_cant_go_left_on_col_0() {
            // This tests for a bug found in perft_test
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R',
                'P', 'P', 'P', 'P', 'P', 'P', 'P', '0',
                'P', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'P',
                '0', '0', '0', '0', '0', '0', '0', '0',
                'p', '0', '0', '0', '0', '0', '0', '0',
                '0', 'p', 'p', 'p', 'p', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (check_if_move_calculated(legal_moves, Move::WHITE_PAWN, 40, 31)) {
                print_error("Failed test_white_pawn_cant_go_left_on_col_0: white pawn can move through board edge!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_white_pawn_cant_go_left_on_col_0");
            }
        }

        void test_white_pawn_cant_go_right_on_col_7() {

        }

        void test_black_pawn_cant_go_right_on_col_0() {

        }

        void test_black_pawn_cant_go_right_on_col_7() {

        }

        void test_white_king_cant_go_left_on_col_0() {

        }

        void test_white_king_cant_go_right_on_col_7() {

        }

        void test_black_king_cant_go_right_on_col_0() {

        }

        void test_black_king_cant_go_right_on_col_7() {

        }

        void test_castle_on_second_rank() {
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                '0', 'P', 'P', '0', 'P', 'P', 'P', '0',
                'P', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', 'P', '0', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', '0', '0', 'n', '0', '0',
                'p', 'p', 'p', 'p', 'k', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', '0', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0b1100);
            bool fail = false;
            vector<Move> legal_moves = move_gen.get_legal_moves();
            if (check_if_move_calculated(legal_moves, Move::WHITE_KING, 52, 54)) {
                print_error("Failed test_castle_on_second_rank: white king can castle on 2nd rank!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: test_castle_on_second_rank");
            }
        }

        void white_king_moves_into_black_pawn() {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0);
            vector<Move> legal_moves = move_gen.get_legal_moves();

            // move_gen.print_piece_board();

            bool fail = false;
            if (check_if_move_calculated(legal_moves, Move::WHITE_KING, 8, 16)) {
                print_error("Failed white_king_moves_into_black_pawn: white king can move into check!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: white_king_moves_into_black_pawn");
            }
        }

        void black_king_moves_into_white_pawn() {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0);
            vector<Move> legal_moves = move_gen.get_legal_moves();

            bool fail = false;
            if (check_if_move_calculated(legal_moves, Move::BLACK_KING, 0, 1)) {
                print_error("FAILED: black_king_moves_into_white_pawn - white king can move into check!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: black_king_moves_into_white_pawn");
            }
        }

        void white_king_behind_black_pawns() {
            vector<char> char_board = {
                '0', '0', 'k', '0', '0', '0', '0', '0',
                'P', 'P', 'P', '0', '0', 'p', '0', '0',
                '0', 'Q', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', 'R',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', '0', '0', '0', '0', '0', '0', '0',
                '0', 'K', '0', '0', '0', '0', '0', '0'
            };

            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0);
            vector<Move> legal_moves = move_gen.get_legal_moves();

            bool fail = false;
            if (!check_if_move_calculated(legal_moves, Move::WHITE_KING, 2, 1)) {
                print_error("FAILED: white_king_behind_black_pawns - white king can't move behind black pawns!\n");
                return;
            }
            if (!fail) {
                print_success("PASS: white_king_behind_black_pawns");
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