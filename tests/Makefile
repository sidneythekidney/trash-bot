CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++14

# Compile the mask_gen_tests and run
mask_tests : mask_gen_test.cpp
	make clean
	$(CXX) $(CXXFLAGS) mask_gen_test.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp -o mask_gen_test.exe

calculate_moves_test : calculate_moves_test.cpp
	make clean
	$(CXX) $(CXXFLAGS) calculate_moves_test.cpp ../move_gen_rec.h ../move_gen_rec.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp -o calculate_moves_test.exe

move_undo_test : make_undo_move_test.cpp
	make clean
	$(CXX) $(CXXFLAGS) make_undo_move_test.cpp ../move_gen_rec.h ../move_gen_rec.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp -o move_undo_test.exe

perft_test: perft_test.cpp
	make clean
	$(CXX) $(CXXFLAGS) perft_test.cpp ../move.h ../move.cpp ../move_gen_rec.h ../move_gen_rec.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp ../perft.h ../perft.cpp -o perft_test.exe

move_pick_test: move_pick_test.cpp
	make clean
	$(CXX) $(CXXFLAGS) move_pick_test.cpp ../move.h ../move.cpp ../move_gen_rec.h ../move_gen_rec.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp ../move_pick_rec.h ../move_pick_rec.cpp -o move_pick_test.exe

debug_test : test.cpp
	$(CXX) $(CXXFLAGS) -pthread test.cpp -o test

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
