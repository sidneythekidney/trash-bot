#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include "defs.h"
#include <vector>

using namespace std;

void print_binary(U64 bitboard);

void pop_bit(U64 bitboard, uint8_t sq);

uint8_t get_ls1b(U64 bitboard);

#endif