CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++14

# Compile the main executable
find : find_br_moves.cpp
	$(CXX) $(CXXFLAGS) find_open_moves.cpp -o find.exe

initialize : initialize.cpp
	$(CXX) $(CXXFLAGS) initialize.cpp initialize.h -o initialize.exe

test : ./tests/test.cpp
	echo $(PWD)
	$(CXX) $(CXXFLAGS) ./tests/test.cpp -o ./test.exe

game: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp utils.hpp utils.cpp move.h move.cpp game.h game.cpp move_gen.h move_gen.cpp generate.h generate.cpp initialize.h initialize.cpp move_pick.h move_pick.cpp -o game.exe

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
