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
            
            test_knight_middle();
            test_knight_edge();
            test_knight_corner();
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
            vector<vector<int>> correct_no_blockers = {
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
            U64 correct_U64_no_blockers = board_to_U64(correct_no_blockers);
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

            vector<vector<int>> correct_with_blockers = {
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
            U64 correct_U64_with_blockers = board_to_U64(correct_with_blockers);
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
        }

        void test_knight_corner() {
            // Test the knight moves on the edge of the board
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
