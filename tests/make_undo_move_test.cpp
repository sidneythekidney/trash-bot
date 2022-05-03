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
            test_make_move_basic();
            test_white_castle_ks();
            test_black_castle_ks();
            test_white_castle_qs();
            test_black_castle_qs();
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

        void test_make_move_basic() {
            // Start from the opening and make the first pawn move
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE);
            vector<Move> legal_moves = move_gen.get_legal_moves();
            // Make the first available pawn move:
            move_gen.make_move(legal_moves[0]);
            vector<vector<int>> correct_white_pawns = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {0,1,1,1,1,1,1,1},
                {0,0,0,0,0,0,0,0},
            };
            U64 white_pawns_u64 = board_to_U64(correct_white_pawns);
            bool fail = false;
            if (!(white_pawns_u64 == move_gen.get_white_pawns())) {
                print_error("FAIL: test_make_move_basic() - Did not make the first correct pawn move!\n");
                cout << "correct white pawns: \n";
                print_binary(white_pawns_u64);
                cout << "current white pawns: \n";
                print_binary(move_gen.get_white_pawns());
                fail = true;
            }            

            if (!fail) {
                print_success("PASS: test_make_move_basic");
            }
        }

        void test_white_castle_ks() {
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
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            // Add only the castle move:
            Move move = create_move(60, 62, Move::WHITE_KING, 0, 0b0011, 0b1000);
            // Make and undo castling move
            move_gen.make_move(move);
            move_gen.undo_move(move);

            // Make sure piece board is correct:
            bool fail = false;
            vector<int> move_gen_pb = move_gen.get_piece_board();
            if (move_gen_pb[61] == Move::WHITE_ROOK || move_gen_pb[63] != Move::WHITE_ROOK) {
                print_error("FAIL: test_white_castle_ks - white rook not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            if (move_gen_pb[60] != Move::WHITE_KING || move_gen_pb[62] == Move::WHITE_KING) {
                print_error("FAIL: test_white_castle_ks - white king not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            // Check the white rook mask:
            vector<vector<int>> correct_white_rooks = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,1},
            };
            U64 white_rooks_u64 = board_to_U64(correct_white_rooks);
            if (white_rooks_u64 != move_gen.get_white_rooks()) {
                print_error("FAIL: test_white_castle_ks - incorrect white rook mask generated!");
                cout << "expected white rooks: \n";
                print_binary(white_rooks_u64);
                cout << "actual white rooks:\n";
                print_binary(move_gen.get_white_rooks());
                fail = true;
            }

            U64 white_king_u64 = 1ULL << 60;
            if (white_king_u64 != move_gen.get_white_king()) {
                print_error("FAIL: test_white_castle_ks - incorrect white king mask generated!");
                cout << "expected white king: \n";
                print_binary(white_king_u64);
                cout << "actual white king:\n";
                print_binary(move_gen.get_white_king());
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test_white_castle_ks");
            }
        }

        void test_black_castle_ks() {
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', '0', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', 'B', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', 'p', '0', 'n', '0', '0',
                'p', 'p', 'p', '0', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            // Add only the castle move:
            Move move = create_move(4, 6, Move::BLACK_KING, 0, 0b1100, 0b1000);
            // Make and undo castling move
            move_gen.make_move(move);
            move_gen.undo_move(move);

            // Make sure piece board is correct:
            bool fail = false;
            vector<int> move_gen_pb = move_gen.get_piece_board();
            if (move_gen_pb[4] == Move::BLACK_ROOK || move_gen_pb[7] != Move::BLACK_ROOK) {
                print_error("FAIL: test_black_castle_ks - black rook not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            if (move_gen_pb[4] != Move::BLACK_KING || move_gen_pb[6] == Move::BLACK_KING) {
                print_error("FAIL: test_black_castle_ks - black king not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            // Check the white rook mask:
            vector<vector<int>> correct_black_rooks = {
                {1,0,0,0,0,0,0,1},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
            };
            U64 black_rooks_u64 = board_to_U64(correct_black_rooks);
            if (black_rooks_u64 != move_gen.get_black_rooks()) {
                print_error("FAIL: test_black_castle_ks - incorrect black rook mask generated!");
                cout << "expected black rooks: \n";
                print_binary(black_rooks_u64);
                cout << "actual black rooks:\n";
                print_binary(move_gen.get_black_rooks());
                fail = true;
            }

            U64 black_king_u64 = 1ULL << 4;
            if (black_king_u64 != move_gen.get_black_king()) {
                print_error("FAIL: test_white_castle_ks - incorrect white king mask generated!");
                cout << "expected black king: \n";
                print_binary(black_king_u64);
                cout << "actual black king:\n";
                print_binary(move_gen.get_black_king());
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test_black_castle_ks");
            }
        }

        void test_white_castle_qs() {
            vector<char> char_board = {
                'R', 'N', 'B', 'Q', 'K', 'B', '0', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', 'N', '0', 'P',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', '0', '0', 'p', '0', '0', '0',
                '0', '0', 'n', 'p', '0', 'q', '0', '0',
                'p', 'p', 'p', 'b', '0', 'p', 'p', 'p',
                'r', '0', '0', '0', 'k', 'b', 'n', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::WHITE, piece_board, 0ULL, 0xf);
            // Add only the castle move:
            Move move = create_move(60, 58, Move::WHITE_KING, 0, 0b0011, 0b1000);
            // Make and undo castling move
            move_gen.make_move(move);
            move_gen.undo_move(move);

            // Make sure piece board is correct:
            bool fail = false;
            vector<int> move_gen_pb = move_gen.get_piece_board();
            if (move_gen_pb[59] == Move::WHITE_ROOK || move_gen_pb[56] != Move::WHITE_ROOK) {
                print_error("FAIL: test_white_castle_qs - white rook not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            if (move_gen_pb[60] != Move::WHITE_KING || move_gen_pb[58] == Move::WHITE_KING) {
                print_error("FAIL: test_white_castle_qs - white king not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            // Check the white rook mask:
            vector<vector<int>> correct_white_rooks = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,1},
            };
            U64 white_rooks_u64 = board_to_U64(correct_white_rooks);
            if (white_rooks_u64 != move_gen.get_white_rooks()) {
                print_error("FAIL: test_white_castle_qs - incorrect white rook mask generated!");
                cout << "expected white rooks: \n";
                print_binary(white_rooks_u64);
                cout << "actual white rooks:\n";
                print_binary(move_gen.get_white_rooks());
                fail = true;
            }

            U64 white_king_u64 = 1ULL << 60;
            if (white_king_u64 != move_gen.get_white_king()) {
                print_error("FAIL: test_white_castle_qs - incorrect white king mask generated!");
                cout << "expected white king: \n";
                print_binary(white_king_u64);
                cout << "actual white king:\n";
                print_binary(move_gen.get_white_king());
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test_white_castle_qs");
            }
        }

        void test_black_castle_qs() {
            vector<char> char_board = {
                'R', '0', '0', '0', 'K', 'B', 'N', 'R',
                'P', 'P', 'P', 'P', '0', 'P', 'P', '0',
                '0', '0', '0', '0', '0', '0', '0', 'P',
                '0', '0', '0', '0', 'P', '0', '0', '0',
                '0', '0', 'b', '0', 'p', '0', '0', '0',
                '0', '0', '0', 'p', '0', 'n', '0', '0',
                'p', 'p', 'p', '0', '0', 'p', 'p', 'p',
                'r', 'n', 'b', 'q', 'k', '0', '0', 'r'
            };
            vector<int> piece_board = char_board_to_piece_board(char_board);
            MoveGenRec move_gen = MoveGenRec(init, gen, color::BLACK, piece_board, 0ULL, 0xf);
            // Add only the castle move:
            Move move = create_move(4, 2, Move::BLACK_KING, 0, 0b1100, 0b1000);
            // Make and undo castling move
            move_gen.make_move(move);
            move_gen.undo_move(move);

            // Make sure piece board is correct:
            bool fail = false;
            vector<int> move_gen_pb = move_gen.get_piece_board();
            if (move_gen_pb[3] == Move::BLACK_ROOK || move_gen_pb[0] != Move::BLACK_ROOK) {
                print_error("FAIL: test_black_castle_qs - black rook not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            if (move_gen_pb[4] != Move::BLACK_KING || move_gen_pb[2] == Move::BLACK_KING) {
                print_error("FAIL: test_black_castle_qs - black king not moved correctly!\n");
                cout <<  "current piece board:\n";
                move_gen.print_piece_board();
                fail = true;
            }
            // Check the white rook mask:
            vector<vector<int>> correct_black_rooks = {
                {1,0,0,0,0,0,0,1},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
            };
            U64 black_rooks_u64 = board_to_U64(correct_black_rooks);
            if (black_rooks_u64 != move_gen.get_black_rooks()) {
                print_error("FAIL: test_black_castle_qs - incorrect black rook mask generated!");
                cout << "expected black rooks: \n";
                print_binary(black_rooks_u64);
                cout << "actual black rooks:\n";
                print_binary(move_gen.get_black_rooks());
                fail = true;
            }

            U64 black_king_u64 = 1ULL << 4;
            if (black_king_u64 != move_gen.get_black_king()) {
                print_error("FAIL: test_white_castle_qs - incorrect white king mask generated!");
                cout << "expected black king: \n";
                print_binary(black_king_u64);
                cout << "actual black king:\n";
                print_binary(move_gen.get_black_king());
                fail = true;
            }

            if (!fail) {
                print_success("PASS: test_black_castle_qs");
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