#include "defs.h"

// uint8_t mailbox[120] = {
//     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//     -1,  0,  1,  2,  3,  4,  5,  6,  7, -1,
//     -1,  8,  9, 10, 11, 12, 13, 14, 15, -1,
//     -1, 16, 17, 18, 19, 20, 21, 22, 23, -1,
//     -1, 24, 25, 26, 27, 28, 29, 30, 31, -1,
//     -1, 32, 33, 34, 35, 36, 37, 38, 39, -1,
//     -1, 40, 41, 42, 43, 44, 45, 46, 47, -1,
//     -1, 48, 49, 50, 51, 52, 53, 54, 55, -1,
//     -1, 56, 57, 58, 59, 60, 61, 62, 63, -1,
//     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
//     -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
// }; // 120 bytes;
// uint8_t mailbox64[64] = {
//     21, 22, 23, 24, 25, 26, 27, 28,
//     31, 32, 33, 34, 35, 36, 37, 38,
//     41, 42, 43, 44, 45, 46, 47, 48,
//     51, 52, 53, 54, 55, 56, 57, 58,
//     61, 62, 63, 64, 65, 66, 67, 68,
//     71, 72, 73, 74, 75, 76, 77, 78,
//     81, 82, 83, 84, 85, 86, 87, 88,
//     91, 92, 93, 94, 95, 96, 97, 98
// }; // 64 bytes;

uint8_t piece_class[16] = {
    KING, QUEEN, ROOK, ROOK, KNIGHT, KNIGHT, BISHOP, BISHOP, 
    PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN, PAWN,
}; // 16 bytes, translates piece number to class
/* 

king - 0
queen - 1
rook - 2
knight - 3
bishop - 4
pawn - 5

*/

uint8_t offsets[5] = {
    8, 8, 4, 8, 4
}; // 5 bytes - Holds number of directions sliding pieces can move

uint8_t offset[5][8] = {
    {-11, -10, -9, -1, 1, 9, 10, 11}, // King
    {-11, -10, -9, -1, 1, 9, 10, 11}, // Queen
    {-10, -1, 1, 10, 0, 0, 0, 0}, // Rook
    {-21, -19, -12, -8, 8, 12, 19, 21}, // Knight
    {-11, -9, 9, 11, 0, 0, 0, 0}, //Bishop
    {0, 0, 0, 0, 0, 0, 0, 0} // Pawn
}; // 40 bytes - holds offset amounts for each piece and direction

const U64 RookMagic[64] = {
    0x2080020500400f0ULL,
    0x28444000400010ULL,
    0x20000a1004100014ULL,
    0x20010c090202006ULL,
    0x8408008200810004ULL,
    0x1746000808002ULL,
    0x2200098000808201ULL,
    0x12c0002080200041ULL,
    0x104000208e480804ULL,
    0x8084014008281008ULL,
    0x4200810910500410ULL,
    0x100014481c20400cULL,
    0x4014a4040020808ULL,
    0x401002001010a4ULL,
    0x202000500010001ULL,
    0x8112808005810081ULL,
    0x40902108802020ULL,
    0x42002101008101ULL,
    0x459442200810c202ULL,
    0x81001103309808ULL,
    0x8110000080102ULL,
    0x8812806008080404ULL,
    0x104020000800101ULL,
    0x40a1048000028201ULL,
    0x4100ba0000004081ULL,
    0x44803a4003400109ULL,
    0xa010a00000030443ULL,
    0x91021a000100409ULL,
    0x4201e8040880a012ULL,
    0x22a000440201802ULL,
    0x30890a72000204ULL,
    0x10411402a0c482ULL,
    0x40004841102088ULL,
    0x40230000100040ULL,
    0x40100010000a0488ULL,
    0x1410100200050844ULL,
    0x100090808508411ULL,
    0x1410040024001142ULL,
    0x8840018001214002ULL,
    0x410201000098001ULL,
    0x8400802120088848ULL,
    0x2060080000021004ULL,
    0x82101002000d0022ULL,
    0x1001101001008241ULL,
    0x9040411808040102ULL,
    0x600800480009042ULL,
    0x1a020000040205ULL,
    0x4200404040505199ULL,
    0x2020081040080080ULL,
    0x40a3002000544108ULL,
    0x4501100800148402ULL,
    0x81440280100224ULL,
    0x88008000000804ULL,
    0x8084060000002812ULL,
    0x1840201000108312ULL,
    0x5080202000000141ULL,
    0x1042a180880281ULL,
    0x900802900c01040ULL,
    0x8205104104120ULL,
    0x9004220000440aULL,
    0x8029510200708ULL,
    0x8008440100404241ULL,
    0x2420001111000bdULL,
    0x4000882304000041ULL,

}; // Calculated from find_magics.c

const U64 BishopMagic[64] = {
    0x100420000431024ULL,
    0x280800101073404ULL,
    0x42000a00840802ULL,
    0xca800c0410c2ULL,
    0x81004290941c20ULL,
    0x400200450020250ULL,
    0x444a019204022084ULL,
    0x88610802202109aULL,
    0x11210a0800086008ULL,
    0x400a08c08802801ULL,
    0x1301a0500111c808ULL,
    0x1280100480180404ULL,
    0x720009020028445ULL,
    0x91880a9000010a01ULL,
    0x31200940150802b2ULL,
    0x5119080c20000602ULL,
    0x242400a002448023ULL,
    0x4819006001200008ULL,
    0x222c10400020090ULL,
    0x302008420409004ULL,
    0x504200070009045ULL,
    0x210071240c02046ULL,
    0x1182219000022611ULL,
    0x400c50000005801ULL,
    0x4004010000113100ULL,
    0x2008121604819400ULL,
    0xc4a4010000290101ULL,
    0x404a000888004802ULL,
    0x8820c004105010ULL,
    0x28280100908300ULL,
    0x4c013189c0320a80ULL,
    0x42008080042080ULL,
    0x90803000c080840ULL,
    0x2180001028220ULL,
    0x1084002a040036ULL,
    0x212009200401ULL,
    0x128110040c84a84ULL,
    0x81488020022802ULL,
    0x8c0014100181ULL,
    0x2222013020082ULL,
    0xa00100002382c03ULL,
    0x1000280001005c02ULL,
    0x84801010000114cULL,
    0x480410048000084ULL,
    0x21204420080020aULL,
    0x2020010000424a10ULL,
    0x240041021d500141ULL,
    0x420844000280214ULL,
    0x29084a280042108ULL,
    0x84102a8080a20a49ULL,
    0x104204908010212ULL,
    0x40a20280081860c1ULL,
    0x3044000200121004ULL,
    0x1001008807081122ULL,
    0x50066c000210811ULL,
    0xe3001240f8a106ULL,
    0x940c0204030020d4ULL,
    0x619204000210826aULL,
    0x2010438002b00a2ULL,
    0x884042004005802ULL,
    0xa90240000006404ULL,
    0x500d082244010008ULL,
    0x28190d00040014e0ULL,
    0x825201600c082444ULL,

}; // Calculated from find_magics.c

U64 BishopMask[64] = {
    0xffffffff88442210,
    0x10884520,
    0x20118a40,
    0x41221480,
    0xffffffff82442801,
    0x4885102,
    0x811a204,
    0x11224408,
    0x4422100a,
    0xffffffff88452015,
    0x118a402a,
    0x22148055,
    0x442801aa,
    0xffffffff88510254,
    0x11a204a8,
    0x22440850,
    0x22100a04,
    0x45201508,
    0xffffffff8a402a11,
    0x14805522,
    0x2801aa44,
    0x51025488,
    0xffffffffa204a810,
    0x44085020,
    0x100a040a,
    0x20150815,
    0x402a112a,
    0xffffffff80552255,
    0x1aa44aa,
    0x2548854,
    0x4a810a8,
    0x8502050,
    0xa040a10,
    0x15081520,
    0x2a112a40,
    0x55225580,
    0xffffffffaa44aa01,
    0x54885402,
    0xffffffffa810a804,
    0x50205008,
    0x40a1022,
    0x8152045,
    0x112a408a,
    0x22558014,
    0x44aa0128,
    0xffffffff88540251,
    0x10a804a2,
    0x20500844,
    0xa102244,
    0x15204588,
    0x2a408a11,
    0x55801422,
    0xffffffffaa012844,
    0x54025188,
    0xffffffffa804a211,
    0x50084422,
    0x10224488,
    0x20458810,
    0x408a1120,
    0xffffffff80142241,
    0x1284482,
    0x2518804,
    0x4a21108,
    0x8442211,
};

U64 RookMask[64] = {
    0x10101ff,
    0x20202ff,
    0x40404ff,
    0x80808ff,
    0x101010ff,
    0x202020ff,
    0x404040ff,
    0xffffffff808080ff,
    0x101ff01,
    0x202ff02,
    0x404ff04,
    0x808ff08,
    0x1010ff10,
    0x2020ff20,
    0x4040ff40,
    0xffffffff8080ff80,
    0x1ff0101,
    0x2ff0202,
    0x4ff0404,
    0x8ff0808,
    0x10ff1010,
    0x20ff2020,
    0x40ff4040,
    0xffffffff80ff8080,
    0xffffffffff010101,
    0xffffffffff020202,
    0xffffffffff040404,
    0xffffffffff080808,
    0xffffffffff101010,
    0xffffffffff202020,
    0xffffffffff404040,
    0xffffffffff808080,
    0x10101ff,
    0x20202ff,
    0x40404ff,
    0x80808ff,
    0x101010ff,
    0x202020ff,
    0x404040ff,
    0xffffffff808080ff,
    0x101ff01,
    0x202ff02,
    0x404ff04,
    0x808ff08,
    0x1010ff10,
    0x2020ff20,
    0x4040ff40,
    0xffffffff8080ff80,
    0x1ff0101,
    0x2ff0202,
    0x4ff0404,
    0x8ff0808,
    0x10ff1010,
    0x20ff2020,
    0x40ff4040,
    0xffffffff80ff8080,
    0xffffffffff010101,
    0xffffffffff020202,
    0xffffffffff040404,
    0xffffffffff080808,
    0xffffffffff101010,
    0xffffffffff202020,
    0xffffffffff404040,
    0xffffffffff808080,
};

U64 blockers[2];
/*
    blockers[0] - white blockers
    blockers[1] - black blockers
    blockers should not include the king
*/

class Generate {
    private:
        // Define data structures used for generating moves
        uint8_t castle; // 1 byte
        /*
        bit 0 - white ks castle
        bit 1 - white qs castle
        bit 2 - black ks castle
        bit 3 - black qs castle 
        */
        uint8_t piece_list[2][16]; //32 bytes
        uint8_t piece_loc[2][16];
        // uint8_t board[64]; // 64 bytes
        // total: 97 bytes (97 bytes reused/constant)
        bool side; // 1 byte - contains either 0 or 1
}