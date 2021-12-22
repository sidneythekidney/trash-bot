.PHONY: mask_tests

CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++11

# Compile the mask_gen_tests and run
mask_tests : mask_gen_test.cpp
	make clean
	$(CXX) $(CXXFLAGS) mask_gen_test.cpp ../generate.h ../generate.cpp ../initialize.h ../initialize.cpp ../utils.hpp ../utils.cpp -o mask_gen_test.exe

debug_test : test.cpp
	$(CXX) $(CXXFLAGS) test.cpp -o test


# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
