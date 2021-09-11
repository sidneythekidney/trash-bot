// Generates all valid moves for a position
#include <generate.h>

void Generate::gen_moves(const uint8_t &side) {
    
}

void Generate::check(const uint8_t &side) {
    std::bitset<64> is_attacked = gen_capture_moves(side);
    if (is_attacked[piece_list[side][0]]) {
        // The king is checked

    }
}

U64 Generate::gen_capture_moves(const uint8_t &side) {
    // Generates all passive moves for a particular side:
    U64 passive_moves = 0;
    // Iterate through all side pieces:
    for (uint8_t i = 0; i < 16;) {
        // for each piece, generate all passive moves:
        // Does the piece exist?
        if (piece_list[side][i] != -1) {
            // If so, check if it's not a pawn:
            if (piece_class[i] != 5) {
                for (uint8_t j = 0; j < offsets[piece_class[i]]) { // Check all ray directions
                    for (uint8_t k = piece_list[side][i];;) { // Start on current square
                        k = mailbox[mailbox64[k] + offset[piece_class[i]][j]]; //Next square on 10x12 board
                        if (k == -1) {
                            // out of bounds
                            break;
                        }
                        // Check if a piece contains this square:
                        if (piece_board[k] != -1) {
                            // If it is an enemy piece, we can capture
                            if (side != (piece_board[k] >> 4)) {
                                passive_moves[k] = 1;
                            }
                            break;
                        }
                        passive_moves[k] = 1;
                        if (!slide[piece_class[i]]) {
                            break;
                        }
                    }
                }
            }
            else {
                // Generate pawn left and right moves if possible
                uint8_t l_sq = piece_list[side][i] + (1-side) * 9;
                uint8_t r_sq = piece_list[side][i] + (1-side) * 11;

                if (mailbox[l_sq] != -1) {
                    passive_moves[l_sq] = 1;
                }
                if (mailbox[r_sq] != -1) {
                    passive_moves[r_sq] = 1;
                }
            }
        }
    }
    return passive_moves;
}

U64 Generate::gen_bishop_moves(uint8_t sq, U64 blockers) {
    
}