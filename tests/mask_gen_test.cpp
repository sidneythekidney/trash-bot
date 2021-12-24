#include "../generate.h"
// #include "../initialize.h"
#include "../utils.hpp"
#include <vector>
#include <assert.h>
#include <iostream>

using namespace std;

class Test {
    public:
        Test(const Initialize* init, const Generate* generate) : init(*init), generate(*generate) {
            cout << "Initializing!\n";
        };

        void test_all() {
            // Knight tests:
            test_knight_middle();
            test_knight_edge();
            test_knight_corner();
            // Pawn tests:
            test_pawn_standard();
            test_pawn_initial();
            // King tests:
            test_king_center();
            test_king_edge();
            test_king_corner();
        }

        void do_nothing() { }

    private:
        Initialize init;
        Generate generate;

        void print_success(string success_msg) {
            cout << "\033[32;40m" + success_msg + "\033[0m\n";
        }

        void print_error(string error_msg) {
            cout << "\033[31;40m" + error_msg + "\033[0m\n";
        }

        U64 board_to_U64(vector<vector<int>> vis) {
            // This function is used for testing
            // Allows conversion from visual 2D vector to bitset:
            U64 ret = 0ULL;
            for (int r = 0; r < 8; ++r) {
                for (int c = 0; c < 8; ++c) {
                    if (vis[r][c]) {
                        ret += 1ULL << (r * 8 + c);
                    }
                }
            }
            return ret;
        }

        void test_knight_middle() {
            // Test the knight moves for middle of the board
            bool fail = false;
            // 1 -- test without blockers:
            U64 blockers1 = 0ULL;
            vector<vector<int>> correct1 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,0,1,0,0,0,1,0},
                {0,0,0,0,0,0,0,0},
                {0,0,1,0,0,0,1,0},
                {0,0,0,1,0,1,0,0},
                {0,0,0,0,0,0,0,0},
            };
            // the square will be (4,4)
            uint8_t sq = 4 * 8 + 4;
            U64 kn_mask_no_blockers = generate.getKnightMask(blockers1, sq);
            U64 correct_U64_no_blockers = board_to_U64(correct1);
            if (correct_U64_no_blockers != kn_mask_no_blockers) {
                print_error("FAIL: test_knight_middle() - no blockers");
                cout << "The output was:\n";
                print_binary(kn_mask_no_blockers);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_U64_no_blockers);
                cout << "\n\n";
                fail = true;
            }
            // 2 -- test with blockers:

            vector<vector<int>> blockers2 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,1,0,0,0,0},
                {0,0,1,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,1,0,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,0,0,0,0,0,0,0},                
            };

            vector<vector<int>> correct2 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,1,0,0},
                {0,0,0,0,0,0,1,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,1,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // the square will be (4,4)
            U64 kn_mask_with_blockers = generate.getKnightMask(board_to_U64(blockers2), sq);
            U64 correct_U64_with_blockers = board_to_U64(correct2);
            if (correct_U64_with_blockers != kn_mask_with_blockers) {
                print_error("FAIL: test_knight_middle() - with blockers");
                cout << "The output was:\n";
                print_binary(correct_U64_with_blockers);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_U64_with_blockers);
                cout << "\n\n";
                fail = true;
            }

            // Print success statement
            if (!fail) {
                print_success("PASS: test_knight_middle()");
            }
        }

        void test_knight_edge() {
            // Test the knight moves on the edge of the board
            // 1 -- test with middle edge of board, no blockers
            bool fail  = false;
            U64 blockers1 = 0ULL;
            // The square the knight occupies is (7,4)
            vector<vector<int>> correct1 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,0,1,0,0,0,1,0},
                {0,0,0,0,0,0,0,0},                
            };
            int sq1 = 7 * 8 + 4;
            U64 kn_mask1 = generate.getKnightMask(blockers1, sq1);
            U64 correct_kn_mask1 = board_to_U64(correct1);
            if (kn_mask1 != correct_kn_mask1) {
                print_error("FAIL: test_knight_edge() - middle edge, no blockers");
                cout << "The output was:\n";
                print_binary(kn_mask1);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_kn_mask1);
                cout << "\n\n";
                fail = true;
            }
            // 2 -- test with edge near corner, with blockers
            vector<vector<int>> blockers2 = {
                {0,0,0,0,0,0,0,0},
                {0,1,1,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,1,0,0},
                {0,0,0,0,1,0,0,0},
                {1,0,0,0,0,0,0,0},                
            };
            vector<vector<int>> correct2 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,1},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // The square the knight occupies is (7,6)
            int sq2 = 7 * 8 + 6;
            U64 kn_mask2 = generate.getKnightMask(board_to_U64(blockers2), sq2);
            U64 correct_kn_mask2 = board_to_U64(correct2);
            if (kn_mask2 != correct_kn_mask2) {
                print_error("FAIL: test_knight_edge() - middle edge close to corner, with blockers");
                cout << "The output was:\n";
                print_binary(kn_mask2);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_kn_mask2);
                cout << "\n\n";
                fail = true;
            }
            
            // Print success statement
            if (!fail) {
                print_success("PASS: test_knight_edge()");
            }
        }

        void test_knight_corner() {
            // Test the knight moves on the edge of the board
            // 1 -- Test the knight moves on a corner with blockers:
            bool fail = false;
            vector<vector<int>> blockers1 = {
                {0,0,0,0,0,0,0,0},
                {0,1,1,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,1,1,0},
                {0,0,0,0,1,0,0,0},
                {1,0,0,0,0,0,0,0},                
            };
            vector<vector<int>> correct1 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,1,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // The square the knight occupies is (7,7)
            int sq1 = 7 * 8 + 7;
            U64 kn_mask1 = generate.getKnightMask(board_to_U64(blockers1), sq1);
            U64 correct_kn_mask1 = board_to_U64(correct1);
            if (kn_mask1 != correct_kn_mask1) {
                print_error("FAIL: test_knight_corner() - with blockers");
                cout << "The output was:\n";
                print_binary(kn_mask1);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_kn_mask1);
                cout << "\n\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_knight_corner()");
            }
        }

        void test_pawn_standard() {
            return;
        }

        void test_pawn_initial() {
            return;
        }

        void test_king_center() {
            // 1 -- test with no blockers
            bool fail = false;
            U64 blockers1 = 0ULL;
            vector<vector<int>> correct1 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,1,1,1,0,0},
                {0,0,0,1,0,1,0,0},
                {0,0,0,1,1,1,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // The square the knight occupies is (3,4)
            int sq1 = 3 * 8 + 4;
            U64 k_mask1 = generate.getKingMask(blockers1, sq1);
            U64 correct_k_mask1 = board_to_U64(correct1);
            if (k_mask1 != correct_k_mask1) {
                print_error("FAIL: test_king_center() - no blockers");
                cout << "The output was:\n";
                print_binary(k_mask1);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_k_mask1);
                cout << "\n\n";
                fail = true;
            }

            // 2 -- test with blockers
            vector<vector<int>> blockers2 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,0,0,0,0,1,0,0},
                {0,0,0,0,1,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            vector<vector<int>> correct2 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,1,0,0,0},
                {0,0,0,1,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // The square the knight occupies is (3,4)
            int sq2 = 3 * 8 + 4;
            U64 k_mask2 = generate.getKingMask(board_to_U64(blockers2), sq2);
            U64 correct_k_mask2 = board_to_U64(correct2);
            if (k_mask2 != correct_k_mask2) {
                print_error("FAIL: test_king_center() - with blockers");
                cout << "The output was:\n";
                print_binary(k_mask2);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_k_mask2);
                cout << "\n\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_king_center()");
            }
        }

        void test_king_edge() {
            bool fail = false;
            // 1 -- test with blockers:
            vector<vector<int>> blockers1 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,1,0,0,0,1,0,0},
                {0,0,0,0,1,0,0,0},
                {1,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            vector<vector<int>> correct1 = {
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {0,1,0,0,0,0,0,0},
                {0,1,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // The square the knight occupies is (4,0)
            int sq1 = 4 * 8 + 0;
            U64 k_mask1 = generate.getKingMask(board_to_U64(blockers1), sq1);
            U64 correct_k_mask1 = board_to_U64(correct1);
            if (k_mask1 != correct_k_mask1) {
                print_error("FAIL: test_king_edge() - with blockers");
                cout << "The output was:\n";
                print_binary(k_mask1);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_k_mask1);
                cout << "\n\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_king_edge()");
            }
        }

        void test_king_corner() {
            bool fail = false;
            // 1 -- test with blockers:
            vector<vector<int>> blockers1 = {
                {0,0,0,0,0,0,0,0},
                {0,1,0,0,0,0,0,0},
                {0,0,0,1,0,1,0,0},
                {0,1,0,0,0,1,0,0},
                {0,0,0,0,1,0,0,0},
                {1,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            vector<vector<int>> correct1 = {
                {0,1,0,0,0,0,0,0},
                {1,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},
                {0,0,0,0,0,0,0,0},                
            };
            // The square the knight occupies is (0,0)
            int sq1 = 0 * 8 + 0;
            U64 k_mask1 = generate.getKingMask(board_to_U64(blockers1), sq1);
            U64 correct_k_mask1 = board_to_U64(correct1);
            if (k_mask1 != correct_k_mask1) {
                print_error("FAIL: test_king_corner() - with blockers");
                cout << "The output was:\n";
                print_binary(k_mask1);
                cout << "\n\n";
                cout << "When the ouput should have been:\n";
                print_binary(correct_k_mask1);
                cout << "\n\n";
                fail = true;
            }
            if (!fail) {
                print_success("PASS: test_king_corner()");
            }
        }
};

int main() {
    // Test the move generation of each piece given a blocker mask.
    cout << "creating init and generate\n";
    Initialize* init = new Initialize();
    init->init_all();
    Generate* generate = new Generate(*init);
    Test test = Test(init, generate);
    test.do_nothing();
    test.test_all();
    delete init;
    delete generate;
}
