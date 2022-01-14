#include <iostream>
#include "../move_gen.h"
#include "../defs.h"
#include "../move.h"

using namespace std;

class MoveTest {
    public:
        MoveTest(MoveGen move_gen) : move_gen(move_gen) {}
        void test_all() {
            test_pawn_move();
        }


    private:

        void print_success(string success_msg) {
            cout << "\033[32;40m" + success_msg + "\033[0m\n";
        }

        void print_error(string error_msg) {
            cout << "\033[31;40m" + error_msg + "\033[0m\n";
        }

        MoveGen move_gen = MoveGen(1, color::WHITE);
        void test_pawn_move() {
            // Check to make sure the first pawn move is made
            Move first_move = move_gen.get_first_move();
            unsigned short from = 0b110110 << 10; // square 56
            unsigned short to   = 0b101000 << 4; // square 40
            unsigned short piece = Move::WHITE_PAWN;
            if (!(first_move == Move(from | to | piece))) {
                print_error("First move being generated for white is incorrect.");
            }

            // Print success message:
            print_success("Generating pawn moves correctly");
            // Make Move:
            move_gen.make_move();
            // Check new pawn position:

        }
};

int main() {
    // Initialize everything to start MoveTests

}