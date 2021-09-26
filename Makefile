CXX ?= g++
CXXFLAGS ?= -Wall -Werror -pedantic -g --std=c++11

# Compile the main executable
find : find_br_moves.cpp
	$(CXX) $(CXXFLAGS) find_br_moves.cpp -o find.exe

initialize : initialize.cpp
	$(CXX) $(CXXFLAGS) initialize.cpp initialize.h -o initialize.exe

test : test.cpp
	$(CXX) $(CXXFLAGS) test.cpp -o test.exe

# Remove automatically generated files
clean :
	rm -rvf *.exe *~ *.out *.dSYM *.stackdump

# Disable built-in rules
.SUFFIXES:

# Run style check tools
