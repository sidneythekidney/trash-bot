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
            test_init();
            test_make_move_basic();
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
            cout << "init passing\n";
        }

        void test_make_move_basic() {
            // Start from the opening and make the first pawn move
            MoveGen move_gen = MoveGen(init, gen, 1, color::WHITE);
            move_gen.calculate_moves();
            // Make the first available pawn move:
            move_gen.make_move();
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

            // Make sure piece board is correct:
            

            if (!fail) {
                print_success("PASS: test_make_move_basic()");
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