CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g -pg --std=c++14 # Contains pg option to allow for programming

# Compile the peft test for profiling and run
perft_test_profiling: perft_test_profiling.cpp
	make clean
	$(CXX) $(CXXFLAGS) perft_test_profiling.cpp ../move.h ../move.cpp ../move_gen_rec.h ../move_gen_rec.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp ../perft.h ../perft.cpp -o perft_test.exe

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
