CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++14

# Compile the mask_gen_tests and run
convert_openings : convert_openings.cpp
	make clean
	$(CXX) $(CXXFLAGS) convert_openings.cpp ../move.h ../move.cpp ../move_gen.h ../move_gen.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp -o convert_openings.exe

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
