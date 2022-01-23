#include "move_gen.h"
#include "defs.h"
#include "utils.hpp"
#include "move.h"
#include <vector>

MoveGen::MoveGen(Initialize* init,
        Generate* gen,
        int depth,
        color active_player) : 
        init(init),
        gen(gen),
        depth(depth), 
        active_player(active_player) {
    // Set the initial board position:
    // Initialize the white pos masks:
    p.reserve(NUM_PIECE_TYPES);
    vector<vector<int>> white_pawn_vec = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::WHITE_PAWN] = board_to_U64(white_pawn_vec);
    
    vector<vector<int>> white_knight_vec = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,1,0,0,0,0,1,0},
    };
    p[Move::WHITE_KNIGHT] = board_to_U64(white_knight_vec);
    
    vector<vector<int>> white_bishop_vec = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,1,0,0,1,0,0},
    };
    p[Move::WHITE_BISHOP] = board_to_U64(white_bishop_vec);
    
    vector<vector<int>> white_rook_vec = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {1,0,0,0,0,0,0,1},
    };
    p[Move::WHITE_ROOK] = board_to_U64(white_rook_vec);

    vector<vector<int>> white_queen_vec = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,1,0,0,0,0},
    };
    p[Move::WHITE_QUEEN] = board_to_U64(white_queen_vec);

    vector<vector<int>> white_king_vec = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,1,0,0,0},
    };
    p[Move::WHITE_KING] = board_to_U64(white_king_vec);

    // Initialize the black pos masks:
    vector<vector<int>> black_pawn_vec = {
        {0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::BLACK_PAWN] = board_to_U64(black_pawn_vec);
    
    vector<vector<int>> black_knight_vec = {
        {0,1,0,0,0,0,1,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::BLACK_KNIGHT] = board_to_U64(black_knight_vec);
    
    vector<vector<int>> black_bishop_vec = {
        {0,0,1,0,0,1,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::BLACK_BISHOP] = board_to_U64(black_bishop_vec);
    
    vector<vector<int>> black_rook_vec = {
        {1,0,0,0,0,0,0,1},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::BLACK_ROOK] = board_to_U64(black_rook_vec);

    vector<vector<int>> black_queen_vec = {
        {0,0,0,1,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::BLACK_QUEEN] = board_to_U64(black_queen_vec);

    vector<vector<int>> black_king_vec = {
        {0,0,0,0,1,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
    };
    p[Move::BLACK_KING] = board_to_U64(black_king_vec);

    en_passant = 0ULL;

    // Using the default constructor, set initial position
    piece_board = {
        Move::BLACK_ROOK, Move::BLACK_KNIGHT, Move::BLACK_BISHOP, Move::BLACK_QUEEN, 
        Move::BLACK_KING, Move::BLACK_BISHOP, Move::BLACK_KNIGHT, Move::BLACK_ROOK,
        Move::BLACK_PAWN, Move::BLACK_PAWN, Move::BLACK_PAWN, Move::BLACK_PAWN, 
        Move::BLACK_PAWN, Move::BLACK_PAWN, Move::BLACK_PAWN, Move::BLACK_PAWN,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        0,0,0,0,0,0,0,0,
        Move::WHITE_PAWN, Move::WHITE_PAWN, Move::WHITE_PAWN, Move::WHITE_PAWN, 
        Move::WHITE_PAWN, Move::WHITE_PAWN, Move::WHITE_PAWN, Move::WHITE_PAWN, 
        Move::WHITE_ROOK, Move::WHITE_KNIGHT, Move::WHITE_BISHOP, Move::WHITE_QUEEN, 
        Move::WHITE_KING, Move::WHITE_BISHOP, Move::WHITE_KNIGHT, Move::WHITE_ROOK
    }; 
}

// Secondary constructor used for testing
MoveGen::MoveGen(
    Initialize* init,
    Generate* gen,
    int depth,
    color active_player,
    vector<int> piece_board,
    U64 en_passant,
    unsigned int castles
) : init(init),
    gen(gen),
    en_passant(en_passant),
    depth(depth),
    active_player(active_player),
    piece_board(piece_board) {
        // Initialize whatever the most recent castles are
        curr_move = castles << 20;

        // Initialize piece masks:
        p.reserve(NUM_PIECE_TYPES);
        for (int i = 0; i < (int)piece_board.size(); ++i) {
            if (piece_board[i] == Move::WHITE_PAWN) p[Move::WHITE_PAWN] |= 1ULL << i;
            else if (piece_board[i] == Move::WHITE_KNIGHT) p[Move::WHITE_KNIGHT] |= 1ULL << i;
            else if (piece_board[i] == Move::WHITE_BISHOP) p[Move::WHITE_BISHOP] |= 1ULL << i;
            else if (piece_board[i] == Move::WHITE_ROOK) p[Move::WHITE_ROOK] |= 1ULL << i;
            else if (piece_board[i] == Move::WHITE_QUEEN) p[Move::WHITE_QUEEN] |= 1ULL << i;
            else if (piece_board[i] == Move::WHITE_KING) p[Move::WHITE_KING] = 1ULL << i;
            else if (piece_board[i] == Move::BLACK_PAWN) p[Move::BLACK_PAWN] |= 1ULL << i;
            else if (piece_board[i] == Move::BLACK_KNIGHT) p[Move::BLACK_KNIGHT] |= 1ULL << i;
            else if (piece_board[i] == Move::BLACK_BISHOP) p[Move::BLACK_BISHOP] |= 1ULL << i;
            else if (piece_board[i] == Move::BLACK_ROOK) p[Move::BLACK_ROOK] |= 1ULL << i;
            else if (piece_board[i] == Move::BLACK_QUEEN) p[Move::BLACK_QUEEN] |= 1ULL << i;
            else if (piece_board[i] == Move::BLACK_KING) p[Move::BLACK_KING] = 1ULL << i;
        }

        // cout << "White Pawns:\n";
        // print_binary(p[Move::WHITE_PAWN]);
        // cout << "White knights:\n";
        // print_binary(p[Move::WHITE_KNIGHT]);
        // cout << "White Bishops:\n";
        // print_binary(p[Move::WHITE_BISHOP]);
        // cout << "White Queens:\n";
        // print_binary(p[Move::WHITE_QUEEN]);
        // cout << "White King:\n";
        // print_binary(p[Move::WHITE_KING]);
        // cout << "Black pawns:\n";
        // print_binary(p[Move::BLACK_PAWN]);
        // cout << "Black knights:\n";
        // print_binary(p[Move::BLACK_KNIGHT]);
        // cout << "Black bishops:\n";
        // print_binary(p[Move::BLACK_BISHOP]);
        // cout << "Black queens:\n";
        // print_binary(p[Move::BLACK_QUEEN]);
        // cout << "Black kings:\n";
        // print_binary(p[Move::BLACK_KING]);
    }

U64 MoveGen::get_white_pieces() {
    // Return a bitboard of all the white pieces
    return p[Move::WHITE_PAWN] | \
            p[Move::WHITE_KNIGHT] | \
            p[Move::WHITE_BISHOP] | \
            p[Move::WHITE_ROOK] | \
            p[Move::WHITE_QUEEN] | \
            p[Move::WHITE_KING];
}
U64 MoveGen::get_black_pieces() {
    // Return a bitboard of all the black pieces
    return p[Move::BLACK_PAWN] | \
            p[Move::BLACK_KNIGHT] | \
            p[Move::BLACK_BISHOP] | \
            p[Move::BLACK_ROOK] | \
            p[Move::BLACK_QUEEN] | \
            p[Move::BLACK_KING];
}
// Obtain the first move from the move list
Move MoveGen::get_first_move() {
    return move_vec[0];
}
// White pieces getters
U64 MoveGen::get_white_pawns() {
    return p[Move::WHITE_PAWN];
}
U64 MoveGen::get_white_knights() {
    return p[Move::WHITE_KNIGHT];
}
U64 MoveGen::get_white_bishops() {
    return p[Move::WHITE_BISHOP];
}
U64 MoveGen::get_white_rooks() {
    return p[Move::WHITE_ROOK];
}
U64 MoveGen::get_white_queens() {
    return p[Move::WHITE_QUEEN];
}
U64 MoveGen::get_white_king() {
    return p[Move::WHITE_KING];
}
// Black pieces getters
U64 MoveGen::get_black_pawns() {
    return p[Move::BLACK_PAWN];
}
U64 MoveGen::get_black_knights() {
    return p[Move::BLACK_KNIGHT];
}
U64 MoveGen::get_black_bishops() {
    return p[Move::BLACK_BISHOP];
}
U64 MoveGen::get_black_rooks() {
    return p[Move::BLACK_ROOK];
}
U64 MoveGen::get_black_queens() {
    return p[Move::BLACK_QUEEN];
}
U64 MoveGen::get_black_king() {
    return p[Move::BLACK_KING];
}

U64 MoveGen::get_en_passant() {
    return en_passant;
}

int MoveGen::num_calculated_moves() {
    return (int)move_vec.size();
}

void MoveGen::add_move(
    unsigned int from,
    unsigned int to,
    unsigned int moved_piece,
    unsigned int captured_piece,
    unsigned int legal_castles,
    unsigned int flags
) {
    // Make sure move is legal: move cannot cause check
    
    // If piece is a king
    // If piece is not a king
    move_vec.emplace_back(Move(from | (to << 6) | (moved_piece << 12) | (captured_piece << 16) | \
                        (legal_castles << 20) | (flags << 24)));
}

void MoveGen::get_gen_pawn_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    cout << "generating pawn moves\n";
    if (side == color::WHITE) {
        U64 wp_copy = p[Move::WHITE_PAWN];
        print_binary(wp_copy);
        while (wp_copy) {
            // Get the position of the pawn we want to calculate moves for:
            int pos = get_ls1b(wp_copy);
            unsigned int promote = 0;
            // Check whether we promote or not (pawns on squares 8-15 promote when moved)
            if (pos-8 < 8) {
                promote |= 1ULL << 2;
            }

            // Move one square up
            if (!((friend_bl | enemy_bl) & 1ULL << (pos - 8))) {
                // Move one square forward
                add_move(pos, pos-8, Move::WHITE_PAWN, 0, curr_move.get_castles(), curr_move.get_flags() | promote);
                // Consider moving two pawns up
                if ((pos / 8 == 6) && !((friend_bl | enemy_bl) & 1ULL << (pos-16))) {
                    add_move(pos, pos-16, Move::WHITE_PAWN, 0, curr_move.get_castles(), curr_move.get_flags() | promote);
                }
            }
            // Check diagonal attack to the right, make sure not on right edge of board
            if (pos % 8 != 0) {
                if ((enemy_bl & 1ULL << (pos - 7))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos-7, Move::WHITE_PAWN, piece_board[pos-7], curr_move.get_castles(), curr_move.get_flags() | promote);
                }
                else if (en_passant & (1ULL << (pos + 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos-7, Move::WHITE_PAWN, piece_board[pos+1], curr_move.get_castles(), curr_move.get_flags() | 1);
                    // Only the 1 is needed since you can only have 1 special move at a time (en passant or promotion)
                }
            }
            
            // Check diagonal attack to the left, make sure not on left edge of board
            if (pos % 8 != 7) {
                cout << "\n\ncurr_move.get_en_passant(): " << curr_move.get_en_passant() << "\n\n"; 
                if ((enemy_bl & 1ULL << (pos - 9))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos-9, Move::WHITE_PAWN, piece_board[pos-9], curr_move.get_castles(), curr_move.get_flags() | promote);
                }
                else if (en_passant & (1ULL << (pos - 1))) {
                    cout << "can en_passant to the left!\n";
                    // en passant is valid to the right
                    add_move(pos, pos-9, Move::WHITE_PAWN, piece_board[pos-1], curr_move.get_castles(), curr_move.get_flags() | 1);
                    // Note: Only the 1 is needed since there can only be 1 special move at a time (en passant or promotion)
                }
            }
            // Remove the pawn from the copy mask to avoid checking repeated moves:
            wp_copy = pop_bit(wp_copy, pos);
        }
    }
    else {
        // Calculate black pawn moves:
        U64 bp_copy = p[Move::BLACK_PAWN];
        while (bp_copy) {
            // Get the position of the pawn we want to calculate moves for:
            int pos = get_ls1b(bp_copy);
            unsigned int promote = 0;
            // Check whether we promote or not (pawns on squares 8-15 promote when moved)
            if (pos+8 >= 56) {
                promote |= 1ULL << 2;
            }

            // Move one square up
            if (!((friend_bl | enemy_bl) & 1ULL << (pos + 8))) {
                // Move one square forward
                add_move(pos, pos+8, Move::BLACK_PAWN, 0, curr_move.get_castles(), curr_move.get_flags() | promote);
                // Consider moving two pawns up
                if ((pos / 8 == 1) && !((friend_bl | enemy_bl) & (1ULL << (pos+16)))) {
                    add_move(pos, pos+16, Move::BLACK_PAWN, 0, curr_move.get_castles(), curr_move.get_flags() | promote);
                }
            }
            // Check diagonal attack to the left, make sure not on left edge of board
            if (pos % 8 != 0) {
                if ((enemy_bl & 1ULL << (pos + 7))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos+7, Move::BLACK_PAWN, piece_board[pos+7], curr_move.get_castles(), curr_move.get_flags() | promote);
                }
                else if (en_passant & (1ULL << (pos - 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos+7, Move::BLACK_PAWN, piece_board[pos-1], curr_move.get_castles(), curr_move.get_flags() | 1);
                    // Only the 1 is needed since you can only have 1 special move at a time (en passant or promotion)
                }
            }
            
            // Check diagonal attack to the right, make sure not on right edge of board
            if (pos % 8 != 7) {
                if ((enemy_bl & 1ULL << (pos + 9))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos+9, Move::BLACK_PAWN, piece_board[pos+9], curr_move.get_castles(), curr_move.get_flags() | promote);
                }
                else if (en_passant & (1ULL << (pos + 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos+9, Move::BLACK_PAWN, piece_board[pos+1], curr_move.get_castles(), curr_move.get_flags() | 1);
                    // Note: Only the 1 is needed since there can only be 1 special move at a time (en passant or promotion)
                }
            }
            // Remove the pawn from the copy mask to avoid checking repeated moves:
            bp_copy = pop_bit(bp_copy, pos);
        }
    }
    cout << "move list length after pawn moves: " << move_vec.size() << "\n";
}

void MoveGen::get_gen_knight_moves(int side, const U64 &friend_bl) {
    // Used to generate knight moves for either white or black knights
    U64 knights = (side == color::WHITE) ? p[Move::WHITE_KNIGHT] : p[Move::BLACK_KNIGHT];
    int piece_type = (side == color::WHITE) ? Move::WHITE_KNIGHT : Move::BLACK_KNIGHT;
    while (knights) {
        int pos = get_ls1b(knights);

        // Determine possible knight moves:
        U64 kn_moves = gen->get_knight_mask(friend_bl, pos);
        // add moves for each knight
        while (kn_moves) {
            int to_sq = get_ls1b(kn_moves);                

            // Add individual move:
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), curr_move.get_flags());

            kn_moves = pop_bit(kn_moves, to_sq);
        }

        knights = pop_bit(knights, pos);
    }
    cout << "move list length after knight moves: " << move_vec.size() << "\n";
}

void MoveGen::get_gen_bishop_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    U64 bishops = (side == color::WHITE) ? p[Move::WHITE_BISHOP] : p[Move::BLACK_BISHOP];
    int piece_type = (side == color::WHITE) ? Move::WHITE_BISHOP : Move::BLACK_BISHOP;
    while (bishops) {
        int pos = get_ls1b(bishops);    

        // Determine possible bishop moves:
        U64 bishop_moves = gen->get_bishop_mask(friend_bl | enemy_bl, pos);
        // Remove bishop moves which involve capturing friendly piece
        U64 rel_friendly_blockers = bishop_moves & friend_bl;
        bishop_moves ^= rel_friendly_blockers;
        // add moves for each bishop
        while (bishop_moves) {
            int to_sq = get_ls1b(bishop_moves);

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), curr_move.get_flags());

            bishop_moves = pop_bit(bishop_moves, to_sq);
        }

        bishops = pop_bit(bishops, pos);
    }
    cout << "move list length after bishop moves: " << move_vec.size() << "\n";
}

void MoveGen::get_gen_rook_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    U64 rooks = (side == color::WHITE) ? p[Move::WHITE_ROOK] : p[Move::BLACK_ROOK];
    int piece_type = (side == color::WHITE) ? Move::WHITE_ROOK : Move::BLACK_ROOK;
    while (rooks) {
        int pos = get_ls1b(rooks);    

        // Determine possible rook moves:
        U64 rook_moves = gen->get_rook_mask(friend_bl | enemy_bl, pos);
        // Remove bishop moves which involve capturing friendly piece
        U64 rel_friendly_blockers = rook_moves & friend_bl;
        rook_moves ^= rel_friendly_blockers;
        // add moves for each rook
        while (rook_moves) {
            int to_sq = get_ls1b(rook_moves);

            // Handle castling logic bits 0-1, white castles, 2-3 black castles
            unsigned int castles = curr_move.get_castles();
            if (pos == 63 && castles & 1) {
                // Check whether white castle kingside can be performed
                --castles;
            }
            if (pos == 56 && castles & (1 << 1)) {
                // Check whether white castle queen side can be performed
                castles -= 2;
            };
            if (pos == 7 && castles & (1 << 2)) {
                // Check whether black castle king side can be performed
                castles -= 4;
            }
            if (pos == 0 && castles & (1 << 3)) {
                // Check whether black castle queen side can be performed
                castles -= 8;
            };

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], castles, curr_move.get_flags());

            rook_moves = pop_bit(rook_moves, to_sq);
        }

        rooks = pop_bit(rooks, pos);
    }
    cout << "move list length after rook moves: " << move_vec.size() << "\n";
}

void MoveGen::get_gen_queen_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    // Calculating queen moves is the same as calculating bishop and rook moves on the same square:
    U64 queens = (side == color::WHITE) ? p[Move::WHITE_QUEEN] : p[Move::BLACK_QUEEN];
    int piece_type = (side == color::WHITE) ? Move::WHITE_QUEEN : Move::BLACK_QUEEN;
    while (queens) {
        int pos = get_ls1b(queens);
        // Determine possible bishop moves from this square:
        U64 bishop_moves = gen->get_bishop_mask(friend_bl | enemy_bl, pos);
        U64 rel_friendly_blockers_bishop = bishop_moves & friend_bl;
        bishop_moves ^= rel_friendly_blockers_bishop;
        // add moves for each bishop
        while (bishop_moves) {
            int to_sq = get_ls1b(bishop_moves);

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), curr_move.get_flags());

            bishop_moves = pop_bit(bishop_moves, to_sq);
        }
        // Determine possible rook moves from this square:
        U64 rook_moves = gen->get_rook_mask(friend_bl | enemy_bl, pos);
        // Remove bishop moves which involve capturing friendly piece
        U64 rel_friendly_blockers_rook = rook_moves & friend_bl;
        rook_moves ^= rel_friendly_blockers_rook;
        while (rook_moves) {
            int to_sq = get_ls1b(rook_moves);

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), curr_move.get_flags());

            rook_moves = pop_bit(rook_moves, to_sq);
        }
        queens = pop_bit(queens, pos);
    }
    cout << "move list length after queen moves: " << move_vec.size() << "\n";
}

void MoveGen::get_gen_king_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    // Used to generate king moves for either white or black kings
    U64 king = (side == color::WHITE) ? p[Move::WHITE_KING] : p[Move::BLACK_KING];
    int piece_type = (side == color::WHITE) ? Move::WHITE_KING : Move::BLACK_KING;
    int pos = get_ls1b(king);

    // Determine possible king moves:
    U64 k_moves = gen->get_king_mask(friend_bl, pos);

    // Consider white castling:
    if (side == color::WHITE) { 
        if ((curr_move.get_castles() & 1) && !((enemy_bl | friend_bl) & white_ks_blockers) && (p[Move::WHITE_ROOK] & (1ULL << 63))) {
            // white castle kingside
            add_move(pos, pos+2, piece_type, 0, curr_move.get_castles() & 0b1100, curr_move.get_flags() | 0b1000);
            // Note we flag that castling is occurring and remove the white castle option
        }
        if ((curr_move.get_castles() & (1 << 1)) && !((enemy_bl | friend_bl) & white_qs_blockers) && (p[Move::WHITE_ROOK] & (1ULL << 56))) {
            // white castle queenside
            add_move(pos, pos-2, piece_type, 0, curr_move.get_castles() & 0b1100, curr_move.get_flags() | 0b1000);
        }
    }
    else {
        // Consider black castling:
        if ((curr_move.get_castles() & (1 << 2)) && !((enemy_bl | friend_bl) & black_ks_blockers) && (p[Move::BLACK_ROOK] & (1ULL << 7))) {
            // black castle kingside
            add_move(pos, pos+2, piece_type, 0, curr_move.get_castles() & 0b0011, curr_move.get_flags() | 0b1000);
            // Note we flag that castling is occurring and remove the black castle option
        }
        if ((curr_move.get_castles() & (1 << 3)) && !((enemy_bl | friend_bl) & black_qs_blockers) && (p[Move::BLACK_ROOK] & 1ULL)) {
            // black castle queenside
            add_move(pos, pos-2, piece_type, 0, curr_move.get_castles() & 0b0011, curr_move.get_flags() | 0b1000);
        }
    }

    while (k_moves) {
        int to_sq = get_ls1b(k_moves);                

        // Add individual move:
        add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), curr_move.get_flags());

        k_moves = pop_bit(k_moves, to_sq);
    }
    cout << "move list length after king moves: " << move_vec.size() << "\n";
}

void MoveGen::calculate_moves() {
    // Obtain blockers
    U64 friend_bl = (active_player == color::WHITE) ? get_white_pieces() : get_black_pieces();
    U64 enemy_bl = (active_player == color::WHITE) ? get_black_pieces() : get_white_pieces(); 

    cout << "calculating pawn moves\n";
    // Calculate pawn moves
    get_gen_pawn_moves(active_player, friend_bl, enemy_bl);

    cout << "calculating knight moves\n";
    // Calculate knight moves:
    get_gen_knight_moves(active_player, friend_bl);
    
    cout << "calculating bishop moves\n";
    // Caclulate bishop moves:
    get_gen_bishop_moves(active_player, friend_bl, enemy_bl);

    cout << "calculating rook moves\n";
    // Calculate rook moves:
    get_gen_rook_moves(active_player, friend_bl, enemy_bl);

    cout << "calculating queen moves\n";
    // Calculate queen moves:
    get_gen_queen_moves(active_player, friend_bl, enemy_bl);

    // calculate king moves:
    get_gen_king_moves(active_player, friend_bl, enemy_bl);
}

void MoveGen::make_move() {}

void MoveGen::undo_move() {}

bool MoveGen::check_if_move_calculated(int piece_type, int from, int to) {
    for (int i = 0; i < (int)move_vec.size(); ++i) {
        if (move_vec[i].get_moved() == (unsigned int)piece_type && \
            move_vec[i].get_from() == (unsigned int)from && \
            move_vec[i].get_to() == (unsigned int)to)
        {
            return true;
        }
    }
    return false;
}

void MoveGen::print_cur_moves() {
    for (int i = 0; i < (int)move_vec.size(); ++i) {
        cout << "Move #" << i << ": \n";
        cout << " piece type: " << move_vec[i].get_moved();
        cout << " from: [" << (move_vec[i].get_from() / 8) << ", " << (move_vec[i].get_from() % 8) << "] ";
        cout << " to: [" << (move_vec[i].get_to() / 8) << ", " << (move_vec[i].get_to() % 8) << "] \n";
    }
}