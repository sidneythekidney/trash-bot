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
    curr_move = 0b1111 << 20;
    move_history.push(curr_move);
    for (int i = 0; i < depth+1; ++i) {
        move_vec.push_back({});
    }
    move_vec[0].push_back(curr_move);
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
    // cout << "init piece board:\n";
    // print_piece_board();
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
        move_history.push(curr_move);
        for (int i = 0; i < depth+1; ++i) {
            move_vec.push_back({});
        }
        move_vec[0].push_back(curr_move);

        // Initialize piece masks:
        p = vector<U64>(NUM_PIECE_TYPES, 0ULL);

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
        // cout << "init piece board:\n";
        // print_piece_board();
        // cout << "White Pawns:\n";
        // print_binary(p[Move::WHITE_PAWN]);
        // cout << "White knights final:\n";
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
    return move_vec[curr_depth][0];
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

vector<int> MoveGen::get_piece_board() {
    return piece_board;
}

void MoveGen::print_piece_board() {
    for (int i = 0; i < 8; ++i) {
        cout << (8-i) << "   "; 
        for (int j = 0; j < 8; ++j) {
            if (piece_board[i*8+j] == Move::WHITE_PAWN) cout << "p ";
            else if (piece_board[i*8+j] == Move::WHITE_KNIGHT) cout << "n ";
            else if (piece_board[i*8+j] == Move::WHITE_BISHOP) cout << "b ";
            else if (piece_board[i*8+j] == Move::WHITE_ROOK) cout << "r ";
            else if (piece_board[i*8+j] == Move::WHITE_QUEEN) cout << "q ";
            else if (piece_board[i*8+j] == Move::WHITE_KING) cout << "k ";
            else if (piece_board[i*8+j] == Move::BLACK_PAWN) cout << "P ";
            else if (piece_board[i*8+j] == Move::BLACK_KNIGHT) cout << "N ";
            else if (piece_board[i*8+j] == Move::BLACK_BISHOP) cout << "B ";
            else if (piece_board[i*8+j] == Move::BLACK_ROOK) cout << "R ";
            else if (piece_board[i*8+j] == Move::BLACK_QUEEN) cout << "Q ";
            else if (piece_board[i*8+j] == Move::BLACK_KING) cout << "K ";
            else cout << ". ";
        }
        cout << "\n";
    }
    cout << "\n    a b c d e f g h";
    cout << "\n\n\n";
}

U64 MoveGen::get_en_passant() {
    return en_passant;
}

int MoveGen::get_curr_depth() {
    return curr_depth;
}

int MoveGen::get_depth() {
    return depth;
}

Move MoveGen::get_curr_move() {
    return curr_move;
}


int MoveGen::num_calculated_moves() {
    return (int)move_vec[curr_depth].size();
}

int MoveGen::num_moves_in_moves_vec() {
    int moves = 0;
    for (size_t i = 0; i < move_vec.size(); ++i) {
        moves += (int)move_vec[i].size();
    }
    return moves;
}

bool MoveGen::in_check(int castle_status) {
    // if (castle_status == castle::KING_SIDE) {
    //     cout << "considering king side castle!\n";
    //     cout << "white rooks:\n";
    //     print_binary(get_white_rooks());
    //     cout << "white king:\n";
    //     print_binary(get_white_king());
    // }
    // print_piece_board();
    // Get the position of the active king
    int pos = get_ls1b((active_player == color::WHITE) ? p[Move::WHITE_KING] : p[Move::BLACK_KING]);
    // cout << "pos: " << pos << "\n";
    vector<int> king_sqs = {pos};
    // Add castle squares if necessary:
    if (castle_status == castle::KING_SIDE) {
        // NOTE: We need to look at squares to the left since pos is the final position after castle
        king_sqs.push_back(pos-1);
        king_sqs.push_back(pos-2);
    }
    else if (castle_status == castle::QUEEN_SIDE) {
        // NOTE: We need to look at squares to the right since pos is the final position after castle
        king_sqs.push_back(pos+1);
        king_sqs.push_back(pos+2);
    }
    // cout << "active player: " << active_player << "\n";
    // Get attacking pieces
    U64 pawns = (active_player == color::WHITE) ? get_black_pawns() : get_white_pawns();
    U64 knights = (active_player == color::WHITE) ? get_black_knights() : get_white_knights();
    U64 bishops = (active_player == color::WHITE) ? get_black_bishops() : get_white_bishops();
    U64 rooks = (active_player == color::WHITE) ? get_black_rooks() : get_white_rooks();
    U64 queens = (active_player == color::WHITE) ? get_black_queens() : get_white_queens();
    U64 king = (active_player == color::WHITE) ? get_black_king() : get_white_king();
    for (auto pos : king_sqs) {
        // Check if any enemy pieces are attacking the king
        // cout << "pawn mask\n";
        // print_binary(gen->get_pawn_mask(active_player, pos));
        if (
            (gen->get_pawn_mask(active_player, pos) & pawns) || \
            (gen->get_knight_mask(0ULL, pos) & knights) || \
            (gen->get_bishop_mask(get_white_pieces() | get_black_pieces(), pos) & (bishops | queens)) || \
            (gen->get_rook_mask(get_white_pieces() | get_black_pieces(), pos) & (rooks | queens)) || \
            (gen->get_king_mask(0ULL, pos) & king)
        ) {
            // Enemy piece can attack the king
            // if (castle_status == castle::KING_SIDE) {
            //     cout << "king_squares: ";
            //     for (auto sq : king_sqs) {
            //         cout << sq << " ";
            //     }
            //     cout << "\n";
            // }
            // cout << "position where check happened: " << pos << "\n";
            // cout << "check found!\n\n\n";
            return true;
        }
    }

    // No enemy piece can attack the king, move is valid
    return false;
}

void MoveGen::add_move(
    unsigned int from,
    unsigned int to,
    unsigned int moved_piece,
    unsigned int captured_piece,
    unsigned int legal_castles,
    unsigned int flags
) {
    /*
    Make sure the move being added doesn't induce check
    To do this, we preview the move by moving the moved piece and 
    removing the captured piece, which we restore at the end of the function
    after checking if the king is in attack

    Special Cases:
        - En passant: restored piece doesn't go to the to square.
                      Instead it goes to to-8 or to+8 depending on pawn color

        - Castle: Must make sure that the king is not in check in either its current square
                  or any of the other squares its moving through
    */
    bool valid_move = true;
    // Preview Move:
    // cout << "moved piece: " << moved_piece << "\n";

    // cout << "before move: \n";
    // print_binary(p[moved_piece]);

    // cout << "white_bishops before moving:\n";
    // print_binary(get_white_bishops());

    // if (print_moves) {
    //     cout << "move: from: " << from << " to: " << to << " moved: " << moved_piece << "\n";
    // }
    // cout << "moving piece!\n";
    // if (print_moves) {
    //     cout << "before moving: \n";
    //     print_piece_board();
    // }
    // cout << "moving piece in add_move\n";
    int castle_type = move_piece(from, to, moved_piece, captured_piece, flags, 1);
    // if (print_moves) {
    //     cout << "after moving: \n";
    //     print_piece_board();
    // }
    // cout << "checking check!\n";
    if (in_check(castle_type)) {
        valid_move = false;
    }
    // if (print_moves) {
    //     cout << "before unmoving: \n";
    //     print_piece_board();
    // }
    // cout << "unmoving piece in add_move!\n";
    move_piece(from, to, moved_piece, captured_piece, flags, -1);
    // if (print_moves) {
    //     cout << "after unmoving: \n";
    //     print_piece_board();
    //     cout << "black pawns:\n";
    //     print_binary(get_black_pawns());
    //     cout << "white pawns:\n";
    //     print_binary(get_white_pawns());
    // }

    // cout << "white_bishops afterclea moving:\n";
    // print_binary(get_white_bishops());
    // cout << "after move:\n";
    // print_binary(p[moved_piece]);

    // Finally add the move as a valid move
    // cout << "curr depth: " << curr_depth << "\n";
    // cout << "move vec size: " << move_vec[curr_depth].size() << "\n";
    if (valid_move) {
        if (!(flags & (1ULL << 2))) { // If the move is not a promotion
            move_vec[curr_depth].emplace_back(Move(from | (to << 6) | (moved_piece << 12) | (captured_piece << 16) | \
                            (legal_castles << 20) | (flags << 24)));
        }
        else { // Handle promotion of a pawn
            move_vec[curr_depth].emplace_back(Move(from | (to << 6) | ((moved_piece+1) << 12) | (captured_piece << 16) | \
                            (legal_castles << 20) | (flags << 24)));
            move_vec[curr_depth].emplace_back(Move(from | (to << 6) | ((moved_piece+2) << 12) | (captured_piece << 16) | \
                            (legal_castles << 20) | (flags << 24)));
            move_vec[curr_depth].emplace_back(Move(from | (to << 6) | ((moved_piece+3) << 12) | (captured_piece << 16) | \
                            (legal_castles << 20) | (flags << 24)));
            move_vec[curr_depth].emplace_back(Move(from | (to << 6) | ((moved_piece+4) << 12) | (captured_piece << 16) | \
                            (legal_castles << 20) | (flags << 24)));
        }
    }
    
}

void MoveGen::get_gen_pawn_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    // cout << "generating pawn moves\n";
    if (side == color::WHITE) {
        U64 wp_copy = p[Move::WHITE_PAWN];
        while (wp_copy) {
            // Get the position of the pawn we want to calculate moves for:
            int pos = get_ls1b(wp_copy);
            // cout << "position: " << pos << "\n";
            unsigned int new_flags = 0;
            // Check whether we promote or not (pawns on squares 8-15 promote when moved)
            if (pos-8 < 8) {
                new_flags |= 1 << 2;
            }
            // cout << "getting here 1\n";
            // Move one square up
            if (!((friend_bl | enemy_bl) & 1ULL << (pos - 8))) {
                // Move one square forward
                add_move(pos, pos-8, Move::WHITE_PAWN, 0, curr_move.get_castles(), new_flags);
                // Consider moving two pawns up
                // cout << "checking two pawns up\n";
                if ((pos / 8 == 6) && !((friend_bl | enemy_bl) & 1ULL << (pos-16))) {
                    add_move(pos, pos-16, Move::WHITE_PAWN, 0, curr_move.get_castles(), 0b0010);
                }
                // cout << "end of if statement\n";
            }
            // cout << "getting here 2\n";
            // Check diagonal attack to the right, make sure not on right edge of board
            if (pos % 8 != 7) {
                if ((enemy_bl & 1ULL << (pos - 7))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos-7, Move::WHITE_PAWN, piece_board[pos-7], curr_move.get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos + 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos-7, Move::WHITE_PAWN, piece_board[pos+1], curr_move.get_castles(), 0b0001);
                    // Only the 1 is needed since you can only have 1 special move at a time (en passant or promotion)
                }
            }
            // cout << "getting here 3\n";
            // Check diagonal attack to the left, make sure not on left edge of board
            if (pos % 8 != 0) {
                if ((enemy_bl & 1ULL << (pos - 9))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos-9, Move::WHITE_PAWN, piece_board[pos-9], curr_move.get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos - 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos-9, Move::WHITE_PAWN, piece_board[pos-1], curr_move.get_castles(), 0b0001);
                    // Note: Only the 1 is needed since there can only be 1 special move at a time (en passant or promotion)
                }
            }
            // cout << "getting here 4\n";
            // Remove the pawn from the copy mask to avoid checking repeated moves:
            wp_copy = pop_bit(wp_copy, pos);
        }
    }
    else {
        // cout << "generating black pawn moves\n";
        // Calculate black pawn moves:
        U64 bp_copy = p[Move::BLACK_PAWN];
        while (bp_copy) {
            // Get the position of the pawn we want to calculate moves for:
            int pos = get_ls1b(bp_copy);
            unsigned int new_flags = 0;
            // Check whether we promote or not (pawns on squares 8-15 promote when moved)
            if (pos+8 >= 56) {
                new_flags |= 1 << 2;
            }

            // Move one square up
            if (!((friend_bl | enemy_bl) & 1ULL << (pos + 8))) {
                // Move one square forward
                add_move(pos, pos+8, Move::BLACK_PAWN, 0, curr_move.get_castles(), new_flags);
                // Consider moving two pawns up
                if ((pos / 8 == 1) && !((friend_bl | enemy_bl) & (1ULL << (pos+16)))) {
                    add_move(pos, pos+16, Move::BLACK_PAWN, 0, curr_move.get_castles(), 0b0010);
                }
            }
            // Check diagonal attack to the left, make sure not on left edge of board
            if (pos % 8 != 0) {
                if ((enemy_bl & 1ULL << (pos + 7))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos+7, Move::BLACK_PAWN, piece_board[pos+7], curr_move.get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos - 1))) {
                    // en passant is valid to the left
                    add_move(pos, pos+7, Move::BLACK_PAWN, piece_board[pos-1], curr_move.get_castles(), 0b0001);
                    // Only the 1 is needed since you can only have 1 special move at a time (en passant or promotion)
                }
            }
            
            // Check diagonal attack to the right, make sure not on right edge of board
            if (pos % 8 != 7) {
                if ((enemy_bl & 1ULL << (pos + 9))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos+9, Move::BLACK_PAWN, piece_board[pos+9], curr_move.get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos + 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos+9, Move::BLACK_PAWN, piece_board[pos+1], curr_move.get_castles(), 0b0001);
                    // Note: Only the 1 is needed since there can only be 1 special move at a time (en passant or promotion)
                }
            }
            // Remove the pawn from the copy mask to avoid checking repeated moves:
            bp_copy = pop_bit(bp_copy, pos);
        }
    }
    // cout << "move list length after pawn moves: " << move_vec[curr_depth].size() << "\n";
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
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), 0b0000);

            kn_moves = pop_bit(kn_moves, to_sq);
        }

        knights = pop_bit(knights, pos);
    }
    // cout << "move list length after knight moves: " << move_vec[curr_depth].size() << "\n";
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
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), 0b0000);

            bishop_moves = pop_bit(bishop_moves, to_sq);
        }

        bishops = pop_bit(bishops, pos);
    }
    // cout << "move list length after bishop moves: " << move_vec[curr_depth].size() << "\n";
}

void MoveGen::get_gen_rook_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    // cout << "friend bl:\n";
    // print_binary(friend_bl);

    // cout << "enemy bl:\n";
    // print_binary(enemy_bl);


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
            add_move(pos, to_sq, piece_type, piece_board[to_sq], castles, 0b0000);

            rook_moves = pop_bit(rook_moves, to_sq);
        }

        rooks = pop_bit(rooks, pos);
    }
    // cout << "move list length after rook moves: " << move_vec[curr_depth].size() << "\n";
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
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), 0b0000);

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
            add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles(), 0b0000);

            rook_moves = pop_bit(rook_moves, to_sq);
        }
        queens = pop_bit(queens, pos);
    }
    // cout << "move list length after queen moves: " << move_vec[curr_depth].size() << "\n";
}

void MoveGen::get_gen_king_moves(int side, const U64 &friend_bl, const U64 &enemy_bl) {
    // Used to generate king moves for either white or black kings
    U64 king = (side == color::WHITE) ? p[Move::WHITE_KING] : p[Move::BLACK_KING];
    int piece_type = (side == color::WHITE) ? Move::WHITE_KING : Move::BLACK_KING;
    int castle_mask = (side == color::WHITE) ? black_castles : white_castles;
    int pos = get_ls1b(king);

    // Determine possible king moves:
    // cout << "side: " << side << "\n";
    // print_binary(p[Move::BLACK_KING]);
    // cout << "pos: " << pos << "\n";
    U64 k_moves = gen->get_king_mask(friend_bl, pos);

    // Consider white castling:
    if (side == color::WHITE) {
        // cout << "checking white moves\n";
        // print_piece_board();
        // cout << "allowed to castle ks: " << (curr_move.get_castles() & 1) << "\n";
        // cout << "blocked ks: " << !((enemy_bl | friend_bl) & white_ks_blockers) << "\n";
        // cout << "blockers: \n";
        // print_binary(enemy_bl | friend_bl);
        // cout << "rook on last square?: " << (p[Move::WHITE_ROOK] & (1ULL << 63)) << "\n";
        if ((curr_move.get_castles() & 1) && !((enemy_bl | friend_bl) & white_ks_blockers) && (p[Move::WHITE_ROOK] & (1ULL << 63))) {
            // white castle kingside
            // cout << "\nadding white ks move!!\n";
            add_move(pos, pos+2, piece_type, 0, curr_move.get_castles() & black_castles, 0b1000);
            // Note we flag that castling is occurring and remove the white castle option
        }
        if ((curr_move.get_castles() & (1 << 1)) && !((enemy_bl | friend_bl) & white_qs_blockers) && (p[Move::WHITE_ROOK] & (1ULL << 56))) {
            // white castle queenside
            add_move(pos, pos-2, piece_type, 0, curr_move.get_castles() & black_castles, 0b1000);
        }
    }
    else {
        // cout << "checking black moves\n";
        // print_piece_board();
        // Consider black castling:
        if ((curr_move.get_castles() & (1 << 2)) && !((enemy_bl | friend_bl) & black_ks_blockers) && (p[Move::BLACK_ROOK] & (1ULL << 7))) {
            // black castle kingside
            add_move(pos, pos+2, piece_type, 0, curr_move.get_castles() & white_castles, 0b1000);
            // Note we flag that castling is occurring and remove the black castle option
        }
        if ((curr_move.get_castles() & (1 << 3)) && !((enemy_bl | friend_bl) & black_qs_blockers) && (p[Move::BLACK_ROOK] & 1ULL)) {
            // black castle queenside
            add_move(pos, pos-2, piece_type, 0, curr_move.get_castles() & white_castles, 0b1000);
        }
    }
    // cout << "adding normal king moves\n";
    // print_binary(k_moves);
    while (k_moves) {
        int to_sq = get_ls1b(k_moves);            

        // Add individual moves (non-castle moves):
        // cout << "attempting to add king move from calculate_moves\n";

        // cout << "from: " << pos << "\n";
        // cout << "to_sq: " << to_sq << "\n";
        // cout << "active player: " << active_player << "\n";
        add_move(pos, to_sq, piece_type, piece_board[to_sq], curr_move.get_castles() & castle_mask, 0b0000);

        k_moves = pop_bit(k_moves, to_sq);
    }
    // cout << "returning after calculating moves\n";
    // cout << "move list length after king moves: " << move_vec[curr_depth].size() << "\n";
}

void MoveGen::calculate_moves() {
    // Make sure we don't calculate moves past the currrent depth:
    if (curr_depth > depth) {
        return;
    }
    
    // Obtain blockers
    U64 friend_bl = (active_player == color::WHITE) ? get_white_pieces() : get_black_pieces();
    U64 enemy_bl = (active_player == color::WHITE) ? get_black_pieces() : get_white_pieces(); 

    // cout << "calculating pawn moves\n";
    // Calculate pawn moves
    get_gen_pawn_moves(active_player, friend_bl, enemy_bl);
    // if (print_moves) {
    //     cout << "after generating pawn moves:\n";
    //     print_piece_board();
    // }

    // cout << "calculating knight moves\n";
    // Calculate knight moves:
    get_gen_knight_moves(active_player, friend_bl);
    
    // cout << "calculating bishop moves\n";
    // Caclulate bishop moves:
    get_gen_bishop_moves(active_player, friend_bl, enemy_bl);

    // cout << "calculating rook moves\n";
    // Calculate rook moves:
    get_gen_rook_moves(active_player, friend_bl, enemy_bl);

    // cout << "calculating queen moves\n";
    // Calculate queen moves:
    get_gen_queen_moves(active_player, friend_bl, enemy_bl);

    // cout << "calculating king moves\n";
    // calculate king moves:
    get_gen_king_moves(active_player, friend_bl, enemy_bl);
    // cout << "finishing calculating moves\n";
}

int MoveGen::move_piece(
    unsigned int from,
    unsigned int to,
    unsigned int moved_piece,
    unsigned int captured_piece,
    unsigned int flags,
    int undo // +1 when making a move, -1 when unmaking a move
) {
    // cout << "starting move_piece()\n";
    // cout << "piece board before make_move:\n";
    // print_piece_board();

    // Preview Move:
    if (!(flags & (1 << 2))) { // No pawn promotion
        p[moved_piece] -= undo * (1ULL << from);
    }
    else { // Handle pawn promotion
        if (moved_piece > 6) {
            p[Move::BLACK_PAWN] -= undo * (1ULL << from);
        }
        else {
            p[Move::WHITE_PAWN] -= undo * (1ULL << from);
        }
    }
    // cout << "handled promotion\n";
    p[moved_piece] += undo * (1ULL << to);

    if (undo == 1) {
        // cout << "from:" << from << "\n";
        // cout << "to:" << to << "\n";
        piece_board[to] = moved_piece;
        piece_board[from] = 0;
    }
    else {
        if (!(flags & 1)) {
            piece_board[to] = captured_piece;
        } 
        else { // undo en passant move
            piece_board[to] = 0;
        }
        if (!(flags & (1 << 2))) { // No pawn promotion
            piece_board[from] = moved_piece;
        }   
        else { // Undo pawn promotion
            if (moved_piece > 6) {
                piece_board[from] = Move::BLACK_PAWN;
            }
            else {
                piece_board[from] = Move::WHITE_PAWN;
            }
        }
    }

    // cout << "got here 1\n";

    // cout << "white king: " << "\n";
    // print_binary(get_white_king());
    // cout << "white rooks: " << "\n";
    // print_binary(get_white_rooks());
    // cout << "flags: " << flags << "\n";
    // Preview capture:
    if (captured_piece) {
        // cout << "capturing piece!\n";
        if (flags & 1) { // Handle en passant capture:
            if (moved_piece == Move::WHITE_PAWN){
                p[captured_piece] -= undo * (1ULL << (to+8));
                // Set the piece board correctly
                (undo == 1) ? piece_board[to+8] = 0 : piece_board[to+8] = Move::BLACK_PAWN;
            }
            else {
                p[captured_piece] -= undo * (1ULL << (to-8));
                // Set the piece board correctly:
                (undo == 1) ? piece_board[to-8] = 0 : piece_board[to-8] = Move::WHITE_PAWN; 
            }
        }
        else {
            p[captured_piece] -= undo * (1ULL << to);
        }
    }

    // cout << "got here 2\n";
    // make sure king is not checked:
    // NOTE: flags >> 3 yields the castling bit
    int castle_type = castle::NONE;
    if (flags & (1 << 3)) {
        // cout << "attempting to castle!!\n";
        if (to > from) {
            castle_type = castle::KING_SIDE;
            // Make rook move
            if (active_player == color::WHITE) {
                // cout << "king side castle for white!\n";
                // cout << "undo: " << undo << "\n";
                p[Move::WHITE_ROOK] += undo * (1ULL << 61);
                p[Move::WHITE_ROOK] -= undo * (1ULL << 63);
                
                piece_board[61] += undo * Move::WHITE_ROOK;
                piece_board[63] -= undo * Move::WHITE_ROOK;

            }
            else {
                // cout << "king side castle for black!\n";
                p[Move::BLACK_ROOK] += undo * (1ULL << 5);
                p[Move::BLACK_ROOK] -= undo * (1ULL << 7);

                piece_board[5] += undo * Move::BLACK_ROOK;
                piece_board[7] -= undo * Move::BLACK_ROOK;
            }
        } 
        else {
            castle_type = castle::QUEEN_SIDE;
            if (active_player == color::WHITE) {
                p[Move::WHITE_ROOK] += undo * (1ULL << 59);
                p[Move::WHITE_ROOK] -= undo * (1ULL << 56);

                piece_board[59] += undo * Move::WHITE_ROOK;
                piece_board[56] -= undo * Move::WHITE_ROOK;
            }
            else {
                p[Move::BLACK_ROOK] += undo * (1ULL << 3);
                p[Move::BLACK_ROOK] -= undo * (1ULL << 0);

                piece_board[3] += undo * Move::BLACK_ROOK;
                piece_board[0] -= undo * Move::BLACK_ROOK;
            }
        }
    }

    // cout << "got here 3\n";

    // if (piece_board[0] != Move::WHITE_ROOK) {
    //     cout << "deleted white rook:\n";
    //     cout << "moved: " << moved_piece << "\n";
    //     cout << "from: " << from << "\n";
    //     cout << "to: " << to << "\n";

    //     cout << "\n";

    //     print_piece_board();
    // }

    // cout << "piece board after make_move:\n";
    // print_piece_board();

    return castle_type;
}

bool MoveGen::make_move() {
    // Retrieve the next move:
    // cout << "start make_move\n";
    // cout << "piece board before make move\n";
    // print_piece_board();
    // cout << "curr depth: " << curr_depth << "\n";
    // cout << "move vec size: " << move_vec[curr_depth].size() << "\n";
    while ((curr_depth > depth || move_vec[curr_depth].size() == 0)) {
        // cout << "move_vec[curr_depth].size() == 0: " << (int)(move_vec[curr_depth].size() == 0) << "\n";
        // // Print the available moves:
        // for (int i = 0; i <= min((int)move_vec.size(), depth); ++i) {
        //     cout << "level " << i << "\n";
        //     cout << "level size: " << move_vec[i].size() << "\n";
        //     for (int j = 0; j < (int)move_vec[i].size(); ++j) {
        //        cout <<  "   move " << j << " from " << move_vec[i][j].get_from() << " to " << move_vec[i][j].get_to() << "\n";
        //     }
        // }
        // If we have no moves to explore or we're at max depth, we need to traverse up
        // if (curr_depth > depth) {
        //     ++num_move_combs;
        // }
        // cout << "curr depth: " << curr_depth << "\n";
        if (curr_depth == 1) {
            // cout << "returned here\n";
            // cout << "returned: \n";
            // cout << "the curr_depth = 0, no need to continue removing moves\n";
            // print_piece_board();
            return false; // No more moves to make
        }
        // cout << "piece board before undo move\n";
        // print_piece_board();
        // cout << "curr move from: " << curr_move.get_from() << "curr move to: " << curr_move.get_to() << "\n";
        // cout << "curr depth: " << curr_depth << "\n";
        undo_move();
        // cout << "piece board after undo move\n";
        // print_piece_board();
        // for (int i = 0; i <= min((int)move_vec.size(), depth); ++i) {
        //     cout << "level " << i << "\n";
        //     cout << "level size: " << move_vec[i].size() << "\n";
        //     for (int j = 0; j < (int)move_vec[i].size(); ++j) {
        //        cout <<  "   move " << j << " from " << move_vec[i][j].get_from() << " to " << move_vec[i][j].get_to() << "\n";
        //     }
        // }
    }
    // cout << "piece board after while loop\n";
    // print_piece_board();
    // cout << "found curr_depth in make_move\n";
    // Make the next available move:
    Move move = move_vec[curr_depth][0];
    // Remove move from move_vec:
    move_vec[curr_depth].erase(move_vec[curr_depth].begin());

    if (curr_depth == 1) {
        // print_piece_board();

        // cout << "legal castles: " << move.get_castles() << "\n";
        if (get_num_move_combs() != 0) {
            cout << "num_move_combs: " << get_num_move_combs() - last_num_move_combs << "\n";
        }
        // cout << "move: " << move.get_moved() << " " << move.get_from() << " " << move.get_to() << "\n";
        string algebraic = "";
        vector<char> rows = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
        algebraic += (rows[move.get_from() % 8] + to_string(8-(int)(move.get_from() / 8)));
        algebraic += (rows[move.get_to() % 8] + to_string(8-(int)(move.get_to() / 8)));
        
        cout << "move: " << algebraic << "  " << move.get_moved() << "\n";
        // cout << "double pawn push: " << (move.get_flags() & (1 << 1)) << "\n";
        // cout << "en passant: " << (move.get_flags() & 1) << "\n";
        // cout << "from: " << move.get_from() << "\n";
        // cout << "to: " << move.get_to() << "\n";
        
        last_num_move_combs = get_num_move_combs();
        // cout << "before making move:\n";
        // print_piece_board();
        // for (int i = 0; i < 40; ++i) {
        //     cout << "\n";
        // }
        // if (move.get_moved() == Move::WHITE_PAWN && move.get_from() == 48 && move.get_to() == 40) {
        //     print_moves = true;
        // }
        // else {
        //     print_moves = false;
        // }
    }
    if (print_moves) {
        string algebraic = "";
        vector<char> rows = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
        algebraic += (rows[move.get_from() % 8] + to_string(8-(int)(move.get_from() / 8)));
        algebraic += (rows[move.get_to() % 8] + to_string(8-(int)(move.get_to() / 8)));
        
        cout << "move: " << algebraic << "  " << move.get_moved() << "\n";
        cout << "before making move:\n";
        print_piece_board();
    }
    // cout << "move piece in make_move\n";
    move_piece(move.get_from(),
               move.get_to(), 
               move.get_moved(), 
               move.get_captured(),
               move.get_flags(),
               1
    );
    // cout << "finished moving piece\n";
    // Set en passant as needed
    if (move.get_flags() & (1 << 1)) {
        en_passant = 1ULL << move.get_to();
        // cout << "generated en passant!\n";
    }
    else {
        en_passant = 0ULL;
    }

    // if (curr_depth == depth) {
    //     if (print_moves) {
    //         cout << "after making move!\n";
    //         print_piece_board();
    //     }
    // }

    curr_move = move;
    move_history.push(curr_move);
    // if (curr_depth == 1) {
    //     cout << "after making move:\n";
    //     print_piece_board();
    //     cout << "en passant: " << en_passant << "\n";
    // }
    ++curr_depth;
    // Switch the active player
    (active_player == color::WHITE) ? active_player = color::BLACK : active_player = color::WHITE;
    // cout << "curr_depth: " << curr_depth << "\n";
    // cout << "checking if depth exceeded\n";
    if (!(curr_depth > depth)) {
        // cout << "calculating moves\n";
        calculate_moves();
        // cout << "finished calculating moves\n";
        // if (print_moves) {
        //     cout << "after calculating moves:\n";
        //     print_piece_board();
        // }
    }
    else {
        // cout << "num_move_combs: " << num_move_combs << "\n";
        // cout << "piece board:\n";
        // print_piece_board();
        ++num_move_combs;
    }
    if (print_moves) {
        cout << "after making move:\n";
        print_piece_board();
    }
    // cout << "piece board after make move\n";
    // print_piece_board();
    // cout << "ending make move!\n";
    return true; // We were able to make a move
}

void MoveGen::undo_move() {
    // cout << "starting undo move\n";
    if (print_moves) {
        cout << "piece board before undo: \n";
        print_piece_board();
        cout << "castle: " << (curr_move.get_flags() & (1 << 3)) << "\n";
    }
    // Switch the active player
    (active_player == color::WHITE) ? active_player = color::BLACK : active_player = color::WHITE;

    move_piece(curr_move.get_from(),
               curr_move.get_to(), 
               curr_move.get_moved(), 
               curr_move.get_captured(),
               curr_move.get_flags(),
               -1
    );
    // cout << "get here 1\n";
    --curr_depth;
    
    move_history.pop();
    if (move_history.size() == 0) {
        return;
    }
    curr_move = move_history.top();
    // cout << "curr_move from " << curr_move.get_from() << "\n"; 
    // cout << "curr_move to " << curr_move.get_to() << "\n"; 
    // cout << "get here 2\n";
    // Set en passant as needed
    if (curr_move.get_flags() & (1 << 1)) {
        en_passant = 1ULL << curr_move.get_to();
    }
    else {
        en_passant = 0ULL;
    }
    // cout << "get here 4\n";
    // cout << "ending undo move\n";
    if (print_moves) {
        cout << "piece board after undo: \n";
        print_piece_board();
    }
}

void MoveGen::play_move(Move move) {
    // Clear the moves and add it to the calculated moves:
    clear_moves();
    // if (curr_depth != 1) {
    //     cout << "Attempted to play move but depth != 1\n";
    //     cout << "depth: " << depth << "\n";
    //     exit(1);
    // }
    move_vec[curr_depth].emplace_back(move);
    // Actually play the move:
    if (!make_move()) {
        cout << "Error trying to make move from play move function\n";
    }
    // Decrease the current depth from 2 to 1 since this is the new starting position
    curr_depth = 1;
}
 
bool MoveGen::check_if_move_calculated(int piece_type, int from, int to) {
    for (int i = 0; i < (int)move_vec[curr_depth].size(); ++i) {
        if (move_vec[curr_depth][i].get_moved() == (unsigned int)piece_type && \
            move_vec[curr_depth][i].get_from() == (unsigned int)from && \
            move_vec[curr_depth][i].get_to() == (unsigned int)to)
        {
            return true;
        }
    }
    return false;
}

Move MoveGen::get_move_from_pos(int from, int to) {
    // Return the move with matching to and from squares
    for (int i = 0; i < (int)move_vec[curr_depth].size(); ++i) {
        if (move_vec[curr_depth][i].get_from() == (unsigned int)from && \
            move_vec[curr_depth][i].get_to() == (unsigned int)to)
        {
            return move_vec[curr_depth][i];
        }
    }
    return Move(0);
}

void MoveGen::print_cur_moves() {
    for (int i = 0; i < (int)move_vec[curr_depth].size(); ++i) {
        cout << "Move #" << i << ": \n";
        cout << " piece type: " << move_vec[curr_depth][i].get_moved();
        cout << " from: [" << (move_vec[curr_depth][i].get_from() / 8) << ", " \
            << (move_vec[curr_depth][i].get_from() % 8) << "] ";
        cout << " to: [" << (move_vec[curr_depth][i].get_to() / 8) << ", " \
        << (move_vec[curr_depth][i].get_to() % 8) << "] \n";
    }
}

U64 MoveGen::get_num_move_combs() {
    return num_move_combs;
}

color MoveGen::get_active_player() {
    return active_player;
}

void MoveGen::set_max_depth(int max_depth) {
    depth = max_depth;
    while ((int)move_vec.size() != max_depth + 1) {
        move_vec.push_back({});
    }
}

void MoveGen::clear_moves() {
    // Clear the calculated moves:
    for (int i = 0; i < depth+1; ++i) {
        move_vec[i].clear();
    }
    curr_depth = 1;
}

int MoveGen::get_move_history_length() {
    return move_history.size();
}

void MoveGen::set_start_move(Move move) {
    clear_moves();
    move_vec[0].push_back(move);
}

int MoveGen::get_num_sibling_moves_left(int ply) {
    return move_vec[ply].size();
}