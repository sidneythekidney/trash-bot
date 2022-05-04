CXX ?= g++
CXX_WINDOWS ?= x86_64-w64-mingw32-g++ -static-libgcc -static-libstdc++
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
	$(CXX) $(CXXFLAGS) main.cpp utils.hpp utils.cpp move.h move.cpp game.h game.cpp move_gen_rec.h move_gen_rec.cpp generate.h generate.cpp initialize.h initialize.cpp move_pick_rec.h move_pick_rec.cpp opening.h opening.cpp -o game.exe

trash_bot_linux: uci.cpp
	$(CXX) $(CXXFLAGS) uci.cpp uci.h utils.hpp utils.cpp move.h move.cpp game.h game.cpp move_gen_rec.h move_gen_rec.cpp generate.h generate.cpp initialize.h initialize.cpp move_pick_rec.h move_pick_rec.cpp opening.h opening.cpp -o trash_bot_linux.exe

trash_bot_windows: uci.cpp
	$(CXX_WINDOWS) $(CXXFLAGS) uci.h uci.cpp utils.hpp utils.cpp move.h move.cpp game.h game.cpp move_gen_rec.h move_gen_rec.cpp generate.h generate.cpp initialize.h initialize.cpp move_pick_rec.h move_pick_rec.cpp opening.h opening.cpp -o trash_bot_w64.exe

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
