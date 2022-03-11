#ifndef INITIALIZE_H
#define INITIALIZE_H

#include <cmath>
#include <iostream>
#include "utils.hpp"

using namespace std;

// Initializer class
class Initialize {
    public:

    U64 pawn_attacks[2][64];
    U64 knight_attacks[64];
    U64 king_attacks[64];
    U64 bishop_masks[64];
    U64 rook_masks[64]; 
    U64 bishop_attacks[64][512];
    U64 rook_attacks[64][4096];

    const U64 RookMagic[64] = {
        0xa8002c000108020ULL,
        0x6c00049b0002001ULL,
        0x100200010090040ULL,
        0x2480041000800801ULL,
        0x280028004000800ULL,
        0x900410008040022ULL,
        0x280020001001080ULL,
        0x2880002041000080ULL,
        0xa000800080400034ULL,
        0x4808020004000ULL,
        0x2290802004801000ULL,
        0x411000d00100020ULL,
        0x402800800040080ULL,
        0xb000401004208ULL,
        0x2409000100040200ULL,
        0x1002100004082ULL,
        0x22878001e24000ULL,
        0x1090810021004010ULL,
        0x801030040200012ULL,
        0x500808008001000ULL,
        0xa08018014000880ULL,
        0x8000808004000200ULL,
        0x201008080010200ULL,
        0x801020000441091ULL,
        0x800080204005ULL,
        0x1040200040100048ULL,
        0x120200402082ULL,
        0xd14880480100080ULL,
        0x12040280080080ULL,
        0x100040080020080ULL,
        0x9020010080800200ULL,
        0x813241200148449ULL,
        0x491604001800080ULL,
        0x100401000402001ULL,
        0x4820010021001040ULL,
        0x400402202000812ULL,
        0x209009005000802ULL,
        0x810800601800400ULL,
        0x4301083214000150ULL,
        0x204026458e001401ULL,
        0x40204000808000ULL,
        0x8001008040010020ULL,
        0x8410820820420010ULL,
        0x1003001000090020ULL,
        0x804040008008080ULL,
        0x12000810020004ULL,
        0x1000100200040208ULL,
        0x430000a044020001ULL,
        0x280009023410300ULL,
        0xe0100040002240ULL,
        0x200100401700ULL,
        0x2244100408008080ULL,
        0x8000400801980ULL,
        0x2000810040200ULL,
        0x8010100228810400ULL,
        0x2000009044210200ULL,
        0x4080008040102101ULL,
        0x40002080411d01ULL,
        0x2005524060000901ULL,
        0x502001008400422ULL,
        0x489a000810200402ULL,
        0x1004400080a13ULL,
        0x4000011008020084ULL,
        0x26002114058042ULL,
    };

    // bishop magic number
    const U64 BishopMagic[64] = {
        0x89a1121896040240ULL,
        0x2004844802002010ULL,
        0x2068080051921000ULL,
        0x62880a0220200808ULL,
        0x4042004000000ULL,
        0x100822020200011ULL,
        0xc00444222012000aULL,
        0x28808801216001ULL,
        0x400492088408100ULL,
        0x201c401040c0084ULL,
        0x840800910a0010ULL,
        0x82080240060ULL,
        0x2000840504006000ULL,
        0x30010c4108405004ULL,
        0x1008005410080802ULL,
        0x8144042209100900ULL,
        0x208081020014400ULL,
        0x4800201208ca00ULL,
        0xf18140408012008ULL,
        0x1004002802102001ULL,
        0x841000820080811ULL,
        0x40200200a42008ULL,
        0x800054042000ULL,
        0x88010400410c9000ULL,
        0x520040470104290ULL,
        0x1004040051500081ULL,
        0x2002081833080021ULL,
        0x400c00c010142ULL,
        0x941408200c002000ULL,
        0x658810000806011ULL,
        0x188071040440a00ULL,
        0x4800404002011c00ULL,
        0x104442040404200ULL,
        0x511080202091021ULL,
        0x4022401120400ULL,
        0x80c0040400080120ULL,
        0x8040010040820802ULL,
        0x480810700020090ULL,
        0x102008e00040242ULL,
        0x809005202050100ULL,
        0x8002024220104080ULL,
        0x431008804142000ULL,
        0x19001802081400ULL,
        0x200014208040080ULL,
        0x3308082008200100ULL,
        0x41010500040c020ULL,
        0x4012020c04210308ULL,
        0x208220a202004080ULL,
        0x111040120082000ULL,
        0x6803040141280a00ULL,
        0x2101004202410000ULL,
        0x8200000041108022ULL,
        0x21082088000ULL,
        0x2410204010040ULL,
        0x40100400809000ULL,
        0x822088220820214ULL,
        0x40808090012004ULL,
        0x910224040218c9ULL,
        0x402814422015008ULL,
        0x90014004842410ULL,
        0x1000042304105ULL,
        0x10008830412a00ULL,
        0x2520081090008908ULL,
        0x40102000a0a60140ULL,
    };

    // bishop relevant occupancy bit count for every square on board
    const int bishop_relevant_bits[64] = {
        6, 5, 5, 5, 5, 5, 5, 6, 
        5, 5, 5, 5, 5, 5, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 9, 9, 7, 5, 5, 
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5, 
        6, 5, 5, 5, 5, 5, 5, 6
    };

    // rook relevant occupancy bit count for every square on board
    const int rook_relevant_bits[64] = {
        12, 11, 11, 11, 11, 11, 11, 12, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        11, 10, 10, 10, 10, 10, 10, 11, 
        12, 11, 11, 11, 11, 11, 11, 12
    };

    void init_all();

    private:

    U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask);

    U64 slow_bishop_attacks(int sq, U64 blockers);

    U64 slow_rook_attacks(int sq, U64 blockers);

    int countBits(U64 mask);

    void init_slider_masks();

    void init_slider_attack_tables();

    void init_knight_attack_table();

    void init_king_attack_table();

    void init_pawn_attacks();

    int castle; // 1 byte
    /*
    bit 0 - white ks castle
    bit 1 - white qs castle
    bit 2 - black ks castle
    bit 3 - black qs castle 
    */
    int piece_list[2][16]; //32 bytes
    int piece_loc[2][16];
    // int board[64]; // 64 bytes
    // total: 97 bytes (97 bytes reused/constant)
    bool side; // 1 byte - contains either 0 or 1
    U64 gen_bishop_moves(int sq, U64 blockers);

    int8_t mailbox[120] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
        -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
        -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
        -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
        -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
        -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
        -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
        -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    }; // 120 bytes;

    int8_t mailbox64[64] = {
        21, 22, 23, 24, 25, 26, 27, 28,
        31, 32, 33, 34, 35, 36, 37, 38,
        41, 42, 43, 44, 45, 46, 47, 48,
        51, 52, 53, 54, 55, 56, 57, 58,
        61, 62, 63, 64, 65, 66, 67, 68,
        71, 72, 73, 74, 75, 76, 77, 78,
        81, 82, 83, 84, 85, 86, 87, 88,
        91, 92, 93, 94, 95, 96, 97, 98
    }; // 64 bytes;

    int piece_class[16] = {
        KING, QUEEN, ROOK, ROOK, KNIGHT, KNIGHT, BISHOP, BISHOP, 
        PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
    };

    int offsets[5] = {
        8, 8, 4, 8, 4
    };

    U64 blockers[2];

    int offset[6][8] = {
        {-11, -10, -9, -1, 1, 9, 10, 11}, // King
        {-11, -10, -9, -1, 1, 9, 10, 11}, // Queen
        {-10, -1, 1, 10, 0, 0, 0, 0}, // Rook
        {-21, -19, -12, -8, 8, 12, 19, 21}, // Knight
        {-11, -9, 9, 11, 0, 0, 0, 0}, //Bishop
        {0, 0, 0, 0, 0, 0, 0, 0} // Pawn
    };
};

/*
    blockers[0] - white blockers
    blockers[1] - black blockers
    blockers should not include the king
*/

#endif