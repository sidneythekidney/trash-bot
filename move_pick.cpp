#include "move_pick.h"


using namespace std;

MovePick::MovePick(MoveGen* move_gen) : move_gen(move_gen) {
    // Generate a 64x12 table of random numbers used for Zobrist hashing:
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            // Initialize random value:
            zob_rand_nums[i][j] = rand();
        }
    }

    // Initialize piece-square tables used by position_eval:
    // https://www.chessprogramming.org/Simplified_Evaluation_Function
    init_piece_squares();
}

int MovePick::zob_hash(const vector<int> &board) {
    // Compute the zobrist hash for the given board:
    // Algorithm described here: https://en.wikipedia.org/wiki/Zobrist_hashing
    // TODO: Consider adding some collision avoidance, since this is not a perfect hash
    int z_hash = 0;
    for (int i = 0; i < 64; ++i) {
        if (board[i]) {
            z_hash ^= zob_rand_nums[i][board[i]-1];
        }
    }
    return z_hash;
}

double MovePick::eval_current_pos() {
    // Check if this position has been evaluated before:
    int hash = zob_hash(move_gen->get_piece_board());
    if (eval_pos.find(hash) != eval_pos.end()) {
        return eval_pos[hash];
    }

    // If the position has not been evaluated, evaluate the position:
    // TODO: change this so scores are normalized
    double eval = 0;
    eval += material_eval();

    // Remember the evaluation for this position:
    eval_pos[hash] = eval;
    return eval;
}

int MovePick::material_eval() {
    // TODO: Place this all in a hash table, add data structure to quickly determine if a game is a draw by insufficient material
    // Get the counts for white pieces
    int num_white_pawns = count_bits(move_gen->get_white_pawns());
    int num_white_knights = count_bits(move_gen->get_white_knights());
    int num_white_bishops = count_bits(move_gen->get_white_bishops());
    int num_white_rooks = count_bits(move_gen->get_white_rooks());
    int num_white_queens = count_bits(move_gen->get_white_queens());

    // Get the counts for black pieces
    int num_black_pawns = count_bits(move_gen->get_black_pawns());
    int num_black_knights = count_bits(move_gen->get_black_knights());
    int num_black_bishops = count_bits(move_gen->get_black_bishops());
    int num_black_rooks = count_bits(move_gen->get_black_rooks());
    int num_black_queens = count_bits(move_gen->get_black_queens());

    // TODO: Determine the endgame based on this material (both sides have < 13 points of material)
    

    /*
    For material, we use the value found here:
    https://www.chessprogramming.org/Simplified_Evaluation_Function

    p - 100
    kn - 320
    b - 330 - most agree today that the bishop is slightly stronger than the knight
    r - 500
    q - 900
    k - 20000
    */
    // Calculate the raw material score
    int mat_diff = 100 * (num_white_pawns - num_black_pawns) + \ 
                320 * (num_white_knights - num_black_knights) + \
                330 * (num_white_bishops - num_black_bishops)+ \
                500 * (num_white_rooks - num_black_rooks)+\
                900 * (num_white_queens - num_black_queens);

    // Factor in bonus piece configs:
    /*

    bishop pair - 50 (https://www.chessprogramming.org/Bishop_Pair)
    rook pair penalty - -20 (penalty)
    knight pair penalty - -20
    no pawn penalty - -30 (hard to win the endgame)

    */

    int bonus_diff = 50 * ((num_white_bishops == 2) - (num_black_bishops == 2)) + \
                    -20 * ((num_white_rooks == 2) - (num_black_rooks == 2)) + \
                    -20 * ((num_white_knights == 2) - (num_black_knights == 2)) + \
                    -30 * ((num_white_pawns  == 0) - (num_black_pawns == 0));

    return mat_diff + bonus_diff;
}

int MovePick::position_eval() {

}

Move MovePick::find_best_move() { // Find best move up to the given depth
    // Iterate through all possible moves, similar to perft implementation
    move_gen->calculate_moves();
    // Set initial best move value to null move - Guaranteed to change
    Move best_move = Move(0);
    // Keep track of the actual move to be played
    Move played_move = Move(0);
    // Set best score to be lowest possible value
    double best_score = -numeric_limits<double>::infinity();
    while (move_gen->make_move()) {
        // Update the played move if we're at a depth of 2
        if (move_gen->get_curr_depth() == 2) {
            // NOTE: When at depth = 2, move_gen.curr_move is the move played from depth = 1
            played_move = move_gen->get_curr_move();
        }
        // Evaluate current move and update best move if we exceed current depth:
        double score = eval_current_pos();
        // If we're at a leaf node, update the best score:
        if (move_gen->get_curr_depth() > move_gen->get_depth()) {
            if (score > best_score) {
                best_score = score;
                best_move = played_move;
            }
        }
    }
    return best_move;
}

void MovePick::set_piece_arr(int piece, int game, int squares[64]) {
    for (int i = 0; i < 64; ++i) {
        piece_square[piece][game][i] = squares[i];
    }
}

void MovePick::init_piece_squares() {
    // Initialize pawn moves:
    int white_pawn_pos_eval[64] = {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };
    set_piece_arr(Move::WHITE_PAWN, 0, white_pawn_pos_eval);
    set_piece_arr(Move::WHITE_PAWN, 1, white_pawn_pos_eval);
    // Mirror and initialize for the black pawns:
}