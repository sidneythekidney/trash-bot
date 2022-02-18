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

        void test_init_move_pick() {
            // Create move_gen and move_pick objects:
            MoveGen* move_gen = new MoveGen(init, gen, 3, color::WHITE);
            MovePick move_pick = MovePick(move_gen);

            move_pick.find_best_move();

            print_success("PASS: test_init_move_pick passed successfully");
        }
};

int main () {
    Initialize* init = new Initialize();
    init->init_all();
    Generate* gen = new Generate(*init);

    MovePickTest move_pick_test = MovePickTest(init, gen);
    move_pick_test.test_all();
}