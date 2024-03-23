#include "move_gen_rec.h"
#include "utils.hpp"

#include <iostream>
#include <algorithm>

using namespace std;

struct moveOrder {
    bool operator()( Move a, Move b ) const {
        // Captures are more important than non-captures:
        if (a.get_captured() && !b.get_captured()) {
            return true;
        }
        if (!a.get_captured() && b.get_captured()) {
            return false;
        }
        else {
            // Tiebreak capture comparisons using least valuable attacker:
            return a.get_moved() < b.get_moved();
        }
        // Otherwise make no change to the ordering
        return true;
    }
};

MoveGenRec::MoveGenRec(
    Initialize* init,
    Generate* gen,
    color active_player
):  init(init),
    gen(gen),
    active_player(active_player) {
    p.resize(NUM_PIECE_TYPES + 1);

    int last_move = 0b1111 << 20;
    move_history.push(last_move);

    move_list = {};

    castles = 0b1111;

    en_passant = 0ULL;

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
// Secondary constructor used for testing:
MoveGenRec::MoveGenRec(
    Initialize* init,
    Generate* gen,
    color active_player,
    vector<int> piece_board,
    U64 en_passant,
    unsigned int castles
):  init(init),
    gen(gen),
    active_player(active_player),
    piece_board(piece_board),
    en_passant(en_passant),
    castles(castles) 
{
    int last_move = castles << 20;
    move_history.push(last_move);

    move_list = {};

    // Initialize piece masks:
    p = vector<U64>(NUM_PIECE_TYPES+1, 0ULL);

    for (int i = 0; i < (int)piece_board.size();  ++i) {
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
}


void MoveGenRec::calculate_moves(){
    // Calculate all legaL moves for given position
    U64 friend_bl = (active_player == color::WHITE) ? get_white_pieces() : get_black_pieces();
    U64 enemy_bl = (active_player == color::WHITE) ? get_black_pieces() : get_white_pieces();

    // Calculate pawn moves
    get_gen_pawn_moves(active_player, friend_bl, enemy_bl);
    // Calculate knight moves:
    get_gen_knight_moves(active_player, friend_bl);
    // Caclulate bishop moves:
    get_gen_bishop_moves(active_player, friend_bl, enemy_bl);
    // Calculate rook moves:
    get_gen_rook_moves(active_player, friend_bl, enemy_bl);
    // Calculate queen moves:
    get_gen_queen_moves(active_player, friend_bl, enemy_bl);
    // calculate king moves:
    get_gen_king_moves(active_player, friend_bl, enemy_bl);
}

void MoveGenRec::make_move(Move move){
    // Update position bitboards accordingly
    en_passant = 0ULL;
    // Handle promotion:
    if (move.get_promotion()) {
        // Add the promoted piece
        p[move.get_moved()] |= 1ULL << move.get_to();
        // Modify piece board accordingly
        piece_board[move.get_to()] = move.get_moved();
        piece_board[move.get_from()] = Move::EMPTY;
        // Remove the pawn
        if (move.get_moved() > Move::BLACK_PAWN) {
            p[Move::BLACK_PAWN] -= 1ULL << move.get_from();
        }
        else {
            p[Move::WHITE_PAWN] -= 1ULL << move.get_from();
        }

        // Handle promotion capture
        if (move.get_captured()) {
            p[move.get_captured()] -= 1ULL << move.get_to();
        }
    }
    else if (move.get_castle()) {
        // Modify piece board
        piece_board[move.get_from()] = Move::EMPTY;
        piece_board[move.get_to()] = move.get_moved();
        // Determine type of castle made
        if (move.get_moved() == Move::WHITE_KING) {
            if (move.get_to() > move.get_from()) {
                // white king side castle
                p[Move::WHITE_KING] -= 1ULL << 60;
                p[Move::WHITE_KING] |= 1ULL << 62;
                p[Move::WHITE_ROOK] -= 1ULL << 63;
                p[Move::WHITE_ROOK] |= 1ULL << 61;

                // Modify rook on piece board
                piece_board[63] = Move::EMPTY;
                piece_board[61] = Move::WHITE_ROOK;
            }
            else {
                // white queen side castle
                p[Move::WHITE_KING] -= 1ULL << 60;
                p[Move::WHITE_KING] |= 1ULL << 58;
                p[Move::WHITE_ROOK] -= 1ULL << 56;
                p[Move::WHITE_ROOK] |= 1ULL << 59;

                // Modify rook on piece board
                piece_board[56] = Move::EMPTY;
                piece_board[59] = Move::WHITE_ROOK;
            }
        }
        else {
            if (move.get_to() > move.get_from()) {
                // black kingside castle
                p[Move::BLACK_KING] -= 1ULL << 4;
                p[Move::BLACK_KING] |= 1ULL << 6;
                p[Move::BLACK_ROOK] -= 1ULL << 7;
                p[Move::BLACK_ROOK] |= 1ULL << 5;

                // Modify rook on piece board
                piece_board[7] = Move::EMPTY;
                piece_board[5] = Move::BLACK_ROOK;
            }
            else {
                // black queenside castle
                p[Move::BLACK_KING] -= 1ULL << 4;
                p[Move::BLACK_KING] |= 1ULL << 2;
                p[Move::BLACK_ROOK] -= 1ULL << 0;
                p[Move::BLACK_ROOK] |= 1ULL << 3;

                // Modify rook on piece board
                piece_board[0] = Move::EMPTY;
                piece_board[3] = Move::BLACK_ROOK;
            }
        }
    }
    else if (move.get_en_passant()) {
        // Handle en passant capture
        p[move.get_moved()] |= 1ULL << move.get_to();
        p[move.get_moved()] -= 1ULL << move.get_from(); 
        piece_board[move.get_to()] = move.get_moved();
        piece_board[move.get_from()] = Move::EMPTY;
        if (active_player == color::WHITE) {
            p[move.get_captured()] -= 1ULL << (move.get_to()+8);
            piece_board[move.get_to()+8] = Move::EMPTY;
        }
        else {
            p[move.get_captured()] -= 1ULL << (move.get_to()-8);
            piece_board[move.get_to()-8] = Move::EMPTY;
        }
    }
    else {
        // Handle normal captures and non-captures:
        p[move.get_moved()] |= 1ULL << move.get_to();
        p[move.get_moved()] -= 1ULL << move.get_from(); 
        piece_board[move.get_from()] = Move::EMPTY;
        piece_board[move.get_to()] = move.get_moved();
        if (move.get_captured()) {
            p[move.get_captured()] -= 1ULL << move.get_to();
        }
        if (move.get_double_push()) {
            // Update en passant as necessary
            en_passant = 1ULL << move.get_to();
        }
    }
    castles = move.get_castles();
    move_history.push(move);
    // Change active player
    (active_player == color::WHITE) ? active_player = color::BLACK : active_player = color::WHITE;
}

void MoveGenRec::undo_move(Move move){
    // Handle promotion
    if (move.get_promotion()) {
        // remove the promoted piece
        p[move.get_moved()] -= 1ULL << move.get_to();
        // Modify piece board accordingly
        piece_board[move.get_from()] = ((active_player == color::BLACK) ? Move::WHITE_PAWN : Move::BLACK_PAWN);
        piece_board[move.get_to()] = move.get_captured();
        // add the pawn to piece bitboards
        if (active_player == color::WHITE) {
            p[Move::BLACK_PAWN] |= 1ULL << move.get_from();
        }
        else {
            p[Move::WHITE_PAWN] |= 1ULL << move.get_from();
        }

        // Handle promotion capture
        if (move.get_captured()) {
            p[move.get_captured()] |= 1ULL << move.get_to();
        }
    }
    else if (move.get_castle()) {
        // Handle castling
        piece_board[move.get_to()] = Move::EMPTY;
        piece_board[move.get_from()] = move.get_moved();
        // Determine type of castle made
        if (move.get_moved() == Move::WHITE_KING) {
            if (move.get_to() > move.get_from()) {
                // white king side castle
                p[Move::WHITE_KING] |= 1ULL << 60;
                p[Move::WHITE_KING] -= 1ULL << 62;
                p[Move::WHITE_ROOK] |= 1ULL << 63;
                p[Move::WHITE_ROOK] -= 1ULL << 61;

                // Modify rook on piece board
                piece_board[61] = Move::EMPTY;
                piece_board[63] = Move::WHITE_ROOK;
            }
            else {
                // white queen side castle
                p[Move::WHITE_KING] |= 1ULL << 60;
                p[Move::WHITE_KING] -= 1ULL << 58;
                p[Move::WHITE_ROOK] |= 1ULL << 56;
                p[Move::WHITE_ROOK] -= 1ULL << 59;

                // Modify rook on piece board
                piece_board[59] = Move::EMPTY;
                piece_board[56] = Move::WHITE_ROOK;
            }
        }
        else {
            if (move.get_to() > move.get_from()) {
                // black kingside castle
                p[Move::BLACK_KING] |= 1ULL << 4;
                p[Move::BLACK_KING] -= 1ULL << 6;
                p[Move::BLACK_ROOK] |= 1ULL << 7;
                p[Move::BLACK_ROOK] -= 1ULL << 5;

                // Modify rook on piece board
                piece_board[5] = Move::EMPTY;
                piece_board[7] = Move::BLACK_ROOK;
            }
            else {
                // black queenside castle
                p[Move::BLACK_KING] |= 1ULL << 4;
                p[Move::BLACK_KING] -= 1ULL << 2;
                p[Move::BLACK_ROOK] |= 1ULL << 0;
                p[Move::BLACK_ROOK] -= 1ULL << 3;

                // Modify rook on piece board
                piece_board[3] = Move::EMPTY;
                piece_board[0] = Move::BLACK_ROOK;
            }
        }
    }
    else if (move.get_en_passant()) {
        p[move.get_moved()] -= 1ULL << move.get_to();
        p[move.get_moved()] |= 1ULL << move.get_from(); 
        piece_board[move.get_from()] = move.get_moved(); 
        piece_board[move.get_to()] = Move::EMPTY;
        if (active_player == color::BLACK) {
            p[move.get_captured()] |= 1ULL << (move.get_to()+8);
            piece_board[move.get_to()+8] = move.get_captured();
        }
        else {
            p[move.get_captured()] |= 1ULL << (move.get_to()-8);
            piece_board[move.get_to()-8] = move.get_captured();
        }
    }
    else {
        // Handle normal captures and non-captures:
        p[move.get_moved()] -= 1ULL << move.get_to();
        p[move.get_moved()] |= 1ULL << move.get_from(); 
        piece_board[move.get_to()] = Move::EMPTY;
        piece_board[move.get_from()] = move.get_moved(); 
        if (move.get_captured()) {
            p[move.get_captured()] |= 1ULL << move.get_to(); 
            piece_board[move.get_to()] = move.get_captured(); 
        }
    }
    // Update board state as needed
    move_history.pop();
    castles = move_history.top().get_castles();
    en_passant = 0ULL;
    if (move_history.top().get_double_push()) {
        en_passant = 1ULL << move_history.top().get_to(); 
    }
    // Change active player
    (active_player == color::WHITE) ? active_player = color::BLACK : active_player = color::WHITE;
}

void MoveGenRec::move_piece(Move move, int undo){
    // Update board piece positions (exclude castling)
    p[move.get_moved()] -= undo * 1ULL << move.get_from();
    p[move.get_moved()] += undo * 1ULL << move.get_to();

    if (move.get_captured() != Move::EMPTY) {
        if (move.get_en_passant()) {
            // Handle en passant captures
            if (active_player == color::WHITE) {
                p[move.get_captured()] -= undo * 1ULL << (move.get_to()+8);
            }
            else {
                p[move.get_captured()] -= undo * 1ULL << (move.get_to()-8);
            }
        }
        else {
            // Handle normal captures
            p[move.get_captured()] -= undo * 1ULL << move.get_to();
        }
    }
}

bool MoveGenRec::checked() {
    // Determine if a piece is in check given the current position
    int pos = get_ls1b((active_player == color::WHITE) ? p[Move::WHITE_KING] : p[Move::BLACK_KING], init->bit_counts);
    // Get attacking pieces
    U64 pawns = (active_player == color::WHITE) ? p[Move::BLACK_PAWN] : p[Move::WHITE_PAWN];
    U64 knights = (active_player == color::WHITE) ? p[Move::BLACK_KNIGHT] : p[Move::WHITE_KNIGHT];
    U64 bishops = (active_player == color::WHITE) ? p[Move::BLACK_BISHOP] : p[Move::WHITE_BISHOP];
    U64 rooks = (active_player == color::WHITE) ? p[Move::BLACK_ROOK] : p[Move::WHITE_ROOK];
    U64 queens = (active_player == color::WHITE) ? p[Move::BLACK_QUEEN] : p[Move::WHITE_QUEEN];
    U64 king = (active_player == color::WHITE) ? p[Move::BLACK_KING] : p[Move::WHITE_KING];

    if (
        (gen->get_pawn_mask(active_player, pos) & pawns) || \
        (gen->get_knight_mask(0ULL, pos) & knights) || \
        (gen->get_bishop_mask(get_white_pieces() | get_black_pieces(), pos) & (bishops | queens)) || \
        (gen->get_rook_mask(get_white_pieces() | get_black_pieces(), pos) & (rooks | queens)) || \
        (gen->get_king_mask(0ULL, pos) & king)
    ) {
        // Enemy piece can attack the king
        return true;
    }

    // No enemy piece can attack the king, move is valid
    return false;
}

bool MoveGenRec::in_check(Move move){
    // Get the position of the active king
    int pos = get_ls1b((active_player == color::WHITE) ? p[Move::WHITE_KING] : p[Move::BLACK_KING], init->bit_counts);
    vector<int> king_sqs = {pos};
    // Add castle squares if necessary:
    if (move.get_castle() && move.get_from() < move.get_to()) {
        // NOTE: We need to look at squares to the left since pos is the final position after castle
        king_sqs.push_back(pos-1);
        king_sqs.push_back(pos-2);
    }
    else if (move.get_castle() && move.get_to() < move.get_from()) {
        // NOTE: We need to look at squares to the right since pos is the final position after castle
        king_sqs.push_back(pos+1);
        king_sqs.push_back(pos+2);
    }

    // Get attacking pieces
    U64 pawns = (active_player == color::WHITE) ? p[Move::BLACK_PAWN] : p[Move::WHITE_PAWN];
    U64 knights = (active_player == color::WHITE) ? p[Move::BLACK_KNIGHT] : p[Move::WHITE_KNIGHT];
    U64 bishops = (active_player == color::WHITE) ? p[Move::BLACK_BISHOP] : p[Move::WHITE_BISHOP];
    U64 rooks = (active_player == color::WHITE) ? p[Move::BLACK_ROOK] : p[Move::WHITE_ROOK];
    U64 queens = (active_player == color::WHITE) ? p[Move::BLACK_QUEEN] : p[Move::WHITE_QUEEN];
    U64 king = (active_player == color::WHITE) ? p[Move::BLACK_KING] : p[Move::WHITE_KING];

    for (auto pos : king_sqs) {
        if (
            (gen->get_pawn_mask(active_player, pos) & pawns) || \
            (gen->get_knight_mask(0ULL, pos) & knights) || \
            (gen->get_bishop_mask(get_white_pieces() | get_black_pieces(), pos) & (bishops | queens)) || \
            (gen->get_rook_mask(get_white_pieces() | get_black_pieces(), pos) & (rooks | queens)) || \
            (gen->get_king_mask(0ULL, pos) & king)
        ) {
            // Enemy piece can attack the king
            return true;
        }
    }

    // No enemy piece can attack the king, move is valid
    return false;
}

U64 MoveGenRec::get_white_pieces() {
    // Return a bitboard of all the white pieces
    return p[Move::WHITE_PAWN] | \
            p[Move::WHITE_KNIGHT] | \
            p[Move::WHITE_BISHOP] | \
            p[Move::WHITE_ROOK] | \
            p[Move::WHITE_QUEEN] | \
            p[Move::WHITE_KING];
}

U64 MoveGenRec::get_black_pieces() {
    // Return a bitboard of all the black pieces
    return p[Move::BLACK_PAWN] | \
            p[Move::BLACK_KNIGHT] | \
            p[Move::BLACK_BISHOP] | \
            p[Move::BLACK_ROOK] | \
            p[Move::BLACK_QUEEN] | \
            p[Move::BLACK_KING];
}

// Calculate pawn moves
void MoveGenRec::get_gen_pawn_moves(color active_player, U64 friend_bl, U64 enemy_bl) {
    if (active_player == color::WHITE) {
        U64 wp_copy = p[Move::WHITE_PAWN];
        while (wp_copy) {
            // Get the position of the pawn we want to calculate moves for:
            int pos = get_ls1b(wp_copy, init->bit_counts);
            unsigned int new_flags = 0;
            // Check whether we promote or not (pawns on squares 8-15 promote when moved)
            if (pos-8 < 8) {
                new_flags |= 1 << 2;
            }
            // Move one square up
            if (!((friend_bl | enemy_bl) & 1ULL << (pos - 8))) {
                // Move one square forward
                add_move(pos, pos-8, Move::WHITE_PAWN, 0, move_history.top().get_castles(), new_flags);
                // Conactive_playerr moving two pawns up
                if ((pos / 8 == 6) && !((friend_bl | enemy_bl) & 1ULL << (pos-16))) {
                    add_move(pos, pos-16, Move::WHITE_PAWN, 0, move_history.top().get_castles(), 0b0010);
                }
            }
            // Check diagonal attack to the right, make sure not on right edge of board
            if (pos % 8 != 7) {
                if ((enemy_bl & 1ULL << (pos - 7))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos-7, Move::WHITE_PAWN, piece_board[pos-7], move_history.top().get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos + 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos-7, Move::WHITE_PAWN, piece_board[pos+1], move_history.top().get_castles(), 0b0001);
                    // Only the 1 is needed since you can only have 1 special move at a time (en passant or promotion)
                }
            }
            // Check diagonal attack to the left, make sure not on left edge of board
            if (pos % 8 != 0) {
                if ((enemy_bl & 1ULL << (pos - 9))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos-9, Move::WHITE_PAWN, piece_board[pos-9], move_history.top().get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos - 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos-9, Move::WHITE_PAWN, piece_board[pos-1], move_history.top().get_castles(), 0b0001);
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
            int pos = get_ls1b(bp_copy, init->bit_counts);
            unsigned int new_flags = 0;
            // Check whether we promote or not (pawns on squares 8-15 promote when moved)
            if (pos+8 >= 56) {
                new_flags |= 1 << 2;
            }

            // Move one square up
            if (!((friend_bl | enemy_bl) & 1ULL << (pos + 8))) {
                // Move one square forward
                add_move(pos, pos+8, Move::BLACK_PAWN, 0, move_history.top().get_castles(), new_flags);
                // Conactive_playerr moving two pawns up
                if ((pos / 8 == 1) && !((friend_bl | enemy_bl) & (1ULL << (pos+16)))) {
                    add_move(pos, pos+16, Move::BLACK_PAWN, 0, move_history.top().get_castles(), 0b0010);
                }
            }
            // Check diagonal attack to the left, make sure not on left edge of board
            if (pos % 8 != 0) {
                if ((enemy_bl & 1ULL << (pos + 7))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos+7, Move::BLACK_PAWN, piece_board[pos+7], move_history.top().get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos - 1))) {
                    // en passant is valid to the left
                    add_move(pos, pos+7, Move::BLACK_PAWN, piece_board[pos-1], move_history.top().get_castles(), 0b0001);
                    // Only the 1 is needed since you can only have 1 special move at a time (en passant or promotion)
                }
            }
            
            // Check diagonal attack to the right, make sure not on right edge of board
            if (pos % 8 != 7) {
                if ((enemy_bl & 1ULL << (pos + 9))) {
                    // Add the move, promotion set earlier
                    add_move(pos, pos+9, Move::BLACK_PAWN, piece_board[pos+9], move_history.top().get_castles(), new_flags);
                }
                else if (en_passant & (1ULL << (pos + 1))) {
                    // en passant is valid to the right
                    add_move(pos, pos+9, Move::BLACK_PAWN, piece_board[pos+1], move_history.top().get_castles(), 0b0001);
                    // Note: Only the 1 is needed since there can only be 1 special move at a time (en passant or promotion)
                }
            }
            // Remove the pawn from the copy mask to avoid checking repeated moves:
            bp_copy = pop_bit(bp_copy, pos);
        }
    }
}

// Calculate knight moves:
void MoveGenRec::get_gen_knight_moves(color active_player, U64 friend_bl) {
    // Used to generate knight moves for either white or black knights
    U64 knights = (active_player == color::WHITE) ? p[Move::WHITE_KNIGHT] : p[Move::BLACK_KNIGHT];
    int piece_type = (active_player == color::WHITE) ? Move::WHITE_KNIGHT : Move::BLACK_KNIGHT;
    while (knights) {
        int pos = get_ls1b(knights, init->bit_counts);

        // Determine possible knight moves:
        U64 kn_moves = gen->get_knight_mask(friend_bl, pos);
        // add moves for each knight
        while (kn_moves) {
            int to_sq = get_ls1b(kn_moves, init->bit_counts);    

            // Add individual move:
            add_move(pos, to_sq, piece_type, piece_board[to_sq], move_history.top().get_castles(), 0b0000);

            kn_moves = pop_bit(kn_moves, to_sq);
        }
        knights = pop_bit(knights, pos);
    }
}

// Caclulate bishop moves:
void MoveGenRec::get_gen_bishop_moves(color active_player, U64 friend_bl, U64 enemy_bl) {
    U64 bishops = (active_player == color::WHITE) ? p[Move::WHITE_BISHOP] : p[Move::BLACK_BISHOP];
    int piece_type = (active_player == color::WHITE) ? Move::WHITE_BISHOP : Move::BLACK_BISHOP;
    while (bishops) {
        int pos = get_ls1b(bishops, init->bit_counts);    

        // Determine possible bishop moves:
        U64 bishop_moves = gen->get_bishop_mask(friend_bl | enemy_bl, pos);
        // Remove bishop moves which involve capturing friendly piece
        U64 rel_friendly_blockers = bishop_moves & friend_bl;
        bishop_moves ^= rel_friendly_blockers;
        // add moves for each bishop
        while (bishop_moves) {
            int to_sq = get_ls1b(bishop_moves, init->bit_counts);

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], move_history.top().get_castles(), 0b0000);

            bishop_moves = pop_bit(bishop_moves, to_sq);
        }

        bishops = pop_bit(bishops, pos);
    }
}

// Calculate rook moves:
void MoveGenRec::get_gen_rook_moves(color active_player, U64 friend_bl, U64 enemy_bl) {
    U64 rooks = (active_player == color::WHITE) ? p[Move::WHITE_ROOK] : p[Move::BLACK_ROOK];
    int piece_type = (active_player == color::WHITE) ? Move::WHITE_ROOK : Move::BLACK_ROOK;
    
    int castles_cp = castles;
    while (rooks) {
        int pos = get_ls1b(rooks, init->bit_counts);    

        // Determine possible rook moves:
        U64 rook_moves = gen->get_rook_mask(friend_bl | enemy_bl, pos);
        // Remove bishop moves which involve capturing friendly piece
        U64 rel_friendly_blockers = rook_moves & friend_bl;
        rook_moves ^= rel_friendly_blockers;
        // add moves for each rook
        while (rook_moves) {
            int to_sq = get_ls1b(rook_moves, init->bit_counts);

            // Handle castling logic bits 0-1, white castles, 2-3 black castles
            unsigned int castles = move_history.top().get_castles();
            if (pos == 63 && castles & 1) {
                // Check whether white castle kingactive_player can be performed
                castles_cp &= 0b1110;
            }
            else if (pos == 56 && castles & (1 << 1)) {
                // Check whether white castle queen active_player can be performed
                castles_cp &= 0b1101;
            }
            else if (pos == 7 && castles & (1 << 2)) {
                // Check whether black castle king active_player can be performed
                castles_cp &= 0b1011;
            }
            else if (pos == 0 && castles & (1 << 3)) {
                // Check whether black castle queen active_player can be performed
                castles_cp &= 0b0111;
            };

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], castles_cp, 0b0000);

            rook_moves = pop_bit(rook_moves, to_sq);
        }
        rooks = pop_bit(rooks, pos);
    }
}

// Calculate queen moves:
void MoveGenRec::get_gen_queen_moves(color active_player, U64 friend_bl, U64 enemy_bl) {
    // Calculating queen moves is the same as calculating bishop and rook moves on the same square:
    U64 queens = (active_player == color::WHITE) ? p[Move::WHITE_QUEEN] : p[Move::BLACK_QUEEN];
    int piece_type = (active_player == color::WHITE) ? Move::WHITE_QUEEN : Move::BLACK_QUEEN;
    while (queens) {
        int pos = get_ls1b(queens, init->bit_counts);
        // Determine possible bishop moves from this square:
        U64 bishop_moves = gen->get_bishop_mask(friend_bl | enemy_bl, pos);
        U64 rel_friendly_blockers_bishop = bishop_moves & friend_bl;
        bishop_moves ^= rel_friendly_blockers_bishop;
        // add moves for each bishop
        while (bishop_moves) {
            int to_sq = get_ls1b(bishop_moves, init->bit_counts);

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], move_history.top().get_castles(), 0b0000);

            bishop_moves = pop_bit(bishop_moves, to_sq);
        }
        // Determine possible rook moves from this square:
        U64 rook_moves = gen->get_rook_mask(friend_bl | enemy_bl, pos);
        // Remove bishop moves which involve capturing friendly piece
        U64 rel_friendly_blockers_rook = rook_moves & friend_bl;
        rook_moves ^= rel_friendly_blockers_rook;
        while (rook_moves) {
            int to_sq = get_ls1b(rook_moves, init->bit_counts);

            // Add individual move
            add_move(pos, to_sq, piece_type, piece_board[to_sq], move_history.top().get_castles(), 0b0000);

            rook_moves = pop_bit(rook_moves, to_sq);
        }
        queens = pop_bit(queens, pos);
    }
}

// calculate king moves:
void MoveGenRec::get_gen_king_moves(color active_player, U64 friend_bl, U64 enemy_bl) {
    // Used to generate king moves for either white or black kings
    U64 king = (active_player == color::WHITE) ? p[Move::WHITE_KING] : p[Move::BLACK_KING];
    int piece_type = (active_player == color::WHITE) ? Move::WHITE_KING : Move::BLACK_KING;
    int castle_mask = (active_player == color::WHITE) ? black_castles : white_castles;
    int pos = get_ls1b(king, init->bit_counts);

    // Determine possible king moves:
    U64 k_moves = gen->get_king_mask(friend_bl, pos);

    // Conactive_playerr white castling:
    if (active_player == color::WHITE) {
        if ((move_history.top().get_castles() & 1) && !((enemy_bl | friend_bl) & white_ks_blockers) && (p[Move::WHITE_ROOK] & (1ULL << 63))) {
            // white castle kingactive_player
            add_move(pos, pos+2, piece_type, 0, move_history.top().get_castles() & black_castles, 0b1000);
            // Note we flag that castling is occurring and remove the white castle option
        }
        if ((move_history.top().get_castles() & (1 << 1)) && !((enemy_bl | friend_bl) & white_qs_blockers) && (p[Move::WHITE_ROOK] & (1ULL << 56))) {
            // white castle queenactive_player
            add_move(pos, pos-2, piece_type, 0, move_history.top().get_castles() & black_castles, 0b1000);
        }
    }
    else {
        // Conactive_playerr black castling:
        if ((move_history.top().get_castles() & (1 << 2)) && !((enemy_bl | friend_bl) & black_ks_blockers) && (p[Move::BLACK_ROOK] & (1ULL << 7))) {
            // black castle kingactive_player
            add_move(pos, pos+2, piece_type, 0, move_history.top().get_castles() & white_castles, 0b1000);
            // Note we flag that castling is occurring and remove the black castle option
        }
        if ((move_history.top().get_castles() & (1 << 3)) && !((enemy_bl | friend_bl) & black_qs_blockers) && (p[Move::BLACK_ROOK] & 1ULL)) {
            // black castle queenactive_player
            add_move(pos, pos-2, piece_type, 0, move_history.top().get_castles() & white_castles, 0b1000);
        }
    }
    while (k_moves) {
        int to_sq = get_ls1b(k_moves, init->bit_counts);            
        // Add individual moves (non-castle moves):
        add_move(pos, to_sq, piece_type, piece_board[to_sq], move_history.top().get_castles() & castle_mask, 0b0000);

        k_moves = pop_bit(k_moves, to_sq);
    }
}

void MoveGenRec::add_move(
    unsigned int from,
    unsigned int to,
    unsigned int moved_piece,
    unsigned int captured_piece,
    unsigned int legal_castles,
    unsigned int flags
) {
    // Check if move is valid before adding to move list
    Move to_consider = create_move(from, to, moved_piece, captured_piece, legal_castles, flags);
    move_piece(to_consider, 1);

    if (!in_check(to_consider)) {

        if (!(flags & (1ULL << 2))) { 
            // If the move is not a promotion
            move_list.emplace_back(to_consider);
        }
        else { // Handle promotion of a pawn
            move_list.emplace_back(create_move(from, to, moved_piece+1, captured_piece, legal_castles, flags));
            move_list.emplace_back(create_move(from, to, moved_piece+2, captured_piece, legal_castles, flags));
            move_list.emplace_back(create_move(from, to, moved_piece+3, captured_piece, legal_castles, flags));
            move_list.emplace_back(create_move(from, to, moved_piece+4, captured_piece, legal_castles, flags));
        }
    }

    move_piece(to_consider, -1);
}

vector<Move> MoveGenRec::get_legal_moves() {
    move_list.clear();
    calculate_moves();
    // Sort the move list to maximize pruning
    sort(move_list.begin(), move_list.end(), moveOrder());
    return move_list;
}

void MoveGenRec::print_piece_board() {
    for (int i = 0; i < 8; ++i) {
        cout << (8-i) << "   "; 
        for (int j = 0; j < 8; ++j) {
            if (piece_board[i*8+j] == Move::WHITE_PAWN) cout << "\u2659 ";
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

void MoveGenRec::print_color_piece_board(int friendly_side) {
    string white_start = (friendly_side == color::WHITE) ? "\033[32;40m" : "\033[31;40m";
    string black_start = (friendly_side == color::BLACK) ? "\033[32;40m" : "\033[31;40m";
    string end = "\033[0m";
    for (int i = 0; i < 8; ++i) {
        cout << (8-i) << "   "; 
        for (int j = 0; j < 8; ++j) {
            if (piece_board[i*8+j] == Move::WHITE_PAWN) cout << white_start << "\u2659 " << end;
            else if (piece_board[i*8+j] == Move::WHITE_KNIGHT) cout << white_start << "\u2658 " << end;
            else if (piece_board[i*8+j] == Move::WHITE_BISHOP) cout << white_start << "\u2657 " << end;
            else if (piece_board[i*8+j] == Move::WHITE_ROOK) cout << white_start << "\u2656 " << end;
            else if (piece_board[i*8+j] == Move::WHITE_QUEEN) cout << white_start << "\u2655 " << end;
            else if (piece_board[i*8+j] == Move::WHITE_KING) cout << white_start << "\u2654 " << end;
            else if (piece_board[i*8+j] == Move::BLACK_PAWN) cout << black_start << "\u2659 " << end;
            else if (piece_board[i*8+j] == Move::BLACK_KNIGHT) cout << black_start << "\u2658 " << end;
            else if (piece_board[i*8+j] == Move::BLACK_BISHOP) cout << black_start << "\u2657 " << end;
            else if (piece_board[i*8+j] == Move::BLACK_ROOK) cout << black_start << "\u2656 " << end;
            else if (piece_board[i*8+j] == Move::BLACK_QUEEN) cout << black_start << "\u2655 " << end;
            else if (piece_board[i*8+j] == Move::BLACK_KING) cout << black_start << "\u2654 " << end;
            else cout << ". ";
        }
        cout << "\n";
    }
    cout << "\n    a b c d e f g h";
    cout << "\n\n\n";
}

void MoveGenRec::print_all_boards() {
    cout << "white pawns\n";
    print_binary(p[Move::WHITE_PAWN]);
    cout << "white knights\n";
    print_binary(p[Move::WHITE_KNIGHT]);
    cout << "white bishops\n";
    print_binary(p[Move::WHITE_BISHOP]);
    cout << "white rooks\n";
    print_binary(p[Move::WHITE_ROOK]);
    cout << "white queens\n";
    print_binary(p[Move::WHITE_QUEEN]);
    cout << "white king\n";
    print_binary(p[Move::WHITE_KING]);
    cout << "black pawns\n";
    print_binary(p[Move::BLACK_PAWN]);
    cout << "black knights\n";
    print_binary(p[Move::BLACK_KNIGHT]);
    cout << "black bishops\n";
    print_binary(p[Move::BLACK_BISHOP]);
    cout << "black rooks\n";
    print_binary(p[Move::BLACK_ROOK]);
    cout << "black queens\n";
    print_binary(p[Move::BLACK_QUEEN]);
    cout << "black king\n";
    print_binary(p[Move::BLACK_KING]);
}

U64 MoveGenRec::get_white_pawns() {
    return p[Move::WHITE_PAWN];
}
U64 MoveGenRec::get_white_knights() {
    return p[Move::WHITE_KNIGHT];
}
U64 MoveGenRec::get_white_bishops() {
    return p[Move::WHITE_BISHOP];
}
U64 MoveGenRec::get_white_rooks() {
    return p[Move::WHITE_ROOK];
}
U64 MoveGenRec::get_white_queens() {
    return p[Move::WHITE_QUEEN];
}
U64 MoveGenRec::get_white_king() {
    return p[Move::WHITE_KING];
}
U64 MoveGenRec::get_black_pawns() {
    return p[Move::BLACK_PAWN];
}
U64 MoveGenRec::get_black_knights() {
    return p[Move::BLACK_KNIGHT];
}
U64 MoveGenRec::get_black_bishops() {
    return p[Move::BLACK_BISHOP];
}
U64 MoveGenRec::get_black_rooks() {
    return p[Move::BLACK_ROOK];
}
U64 MoveGenRec::get_black_queens() {
    return p[Move::BLACK_QUEEN];
}
U64 MoveGenRec::get_black_king() {
    return p[Move::BLACK_KING];
}
vector<int> MoveGenRec::get_piece_board() {
    return piece_board;
}
color MoveGenRec::get_active_player() {
    return active_player;
}
void MoveGenRec::set_active_player(color new_active_player) {
    active_player = new_active_player;
}
