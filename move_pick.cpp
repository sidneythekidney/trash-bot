#include "move_pick.h"


using namespace std;

MovePick::MovePick(Initialize* init, Generate* gen, MoveGen* move_gen) : init(init), gen(gen), move_gen(move_gen) {
    // Generate a 64x12 table of random numbers used for Zobrist hashing:
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 12; ++j) {
            // Initialize random value:
            zob_rand_nums[i][j] = rand();
        }
    }
    iter_move_gen = new MoveGen(init, gen, 1, color::WHITE, move_gen->get_piece_board(), 0, 0);
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
    int hash = zob_hash(iter_move_gen->get_piece_board());
    if (eval_pos.find(hash) != eval_pos.end()) {
        return eval_pos[hash];
    }

    // If the position has not been evaluated, evaluate the position:
    // TODO: change this so scores are normalized
    double eval = 0;
    eval += material_eval();
    eval += position_eval();
    eval += pawn_structure_eval();

    // Remember the evaluation for this position:
    eval_pos[hash] = eval;
    // This evaluation is for when playing with white, negate if playing for black
    return (move_gen->get_active_player() ? -1 : 1) * eval;
}

double MovePick::material_eval() {
    // TODO: Place this all in a hash table, add data structure to quickly determine if a game is a draw by insufficient material
    // Get the counts for white pieces
    int num_white_pawns = count_bits(iter_move_gen->get_white_pawns());
    int num_white_knights = count_bits(iter_move_gen->get_white_knights());
    int num_white_bishops = count_bits(iter_move_gen->get_white_bishops());
    int num_white_rooks = count_bits(iter_move_gen->get_white_rooks());
    int num_white_queens = count_bits(iter_move_gen->get_white_queens());

    // Get the counts for black pieces
    int num_black_pawns = count_bits(iter_move_gen->get_black_pawns());
    int num_black_knights = count_bits(iter_move_gen->get_black_knights());
    int num_black_bishops = count_bits(iter_move_gen->get_black_bishops());
    int num_black_rooks = count_bits(iter_move_gen->get_black_rooks());
    int num_black_queens = count_bits(iter_move_gen->get_black_queens());

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

double MovePick::position_eval() {
    double position_score = 0;
    vector<int> piece_board = iter_move_gen->get_piece_board();
    for (int i = 0; i < 64; ++i) {
        int piece = piece_board[i];
        if (piece != Move::EMPTY) {
            // Calculate the multiplier: -1 for black, 1 for white
            int multiplier = (piece >= Move::BLACK_PAWN) ? -1 : 1;
            // Add the position score for this piece:
            position_score += multiplier * piece_square[piece][endgame][i];
        }
    }
    return position_score;
}

double MovePick::pawn_structure_eval() {
    double eval = 0;
    U64 white_pawns = iter_move_gen->get_white_pawns();
    U64 black_pawns = iter_move_gen->get_black_pawns();
    // Check to see if we evaluated this pawn struct before:
    // TODO: Add in a hash table to avoid recomputing the same vals

    /*****************
     * DOUBLED PAWNS * 
     ****************/
    // Doubled pawns are bad, even worse in the endgame
    // Having two pawns still usually better than having just one pawn 
    for (int i = 0; i < 8; ++i) {
        eval -= (endgame ? 75 : 50) * (count_bits(white_pawns & col_masks[i]) > 1);
        eval += (endgame ? 75 : 50) * (count_bits(black_pawns & col_masks[i]) > 1);
    }

    /***************
     * PAWN CHAINS * 
     **************/
    /*
        - Pawns are stronger when they support each other
    */
    double w_protects = 0, w_attacks = 0; 
    int sq;
    double b_protects = 0, b_attacks = 0;
    U64 white_pawns_cp = white_pawns;
    U64 black_pawns_cp = black_pawns;
    while (white_pawns_cp) {
        sq = get_ls1b(white_pawns_cp);
        w_attacks += count_bits(gen->get_pawn_mask(color::WHITE, sq));
        w_protects += count_bits(white_pawns & gen->get_pawn_mask(color::WHITE, sq)); 
        white_pawns_cp = pop_bit(white_pawns_cp, sq);
    }
    while (black_pawns_cp) {
        sq = get_ls1b(black_pawns_cp);
        b_attacks += count_bits(gen->get_pawn_mask(color::BLACK, sq));
        b_protects += count_bits(black_pawns & gen->get_pawn_mask(color::BLACK, sq)); 
        black_pawns_cp = pop_bit(black_pawns_cp, sq);
    }
    // Avoid divide by zero error:
    w_attacks = max(w_attacks, 1.0);
    b_attacks = max(b_attacks, 1.0);

    // Get eval score:
    // protected pawns more important in endgame:
    eval += (endgame ? 125 : 100) * (w_protects / w_attacks);
    eval -= (endgame ? 125 : 100) * (b_protects / b_attacks);
    return eval;
}

Move MovePick::find_best_move(int max_runtime) { // Find best move up to the given depth
    // Iterate through all possible moves, similar to perft implementation
    // Check and make sure path scores is empty:
    if (!path_scores.empty()) {
        cout << "path_scores should have been empty but had size = " << path_scores.size() << "\n";
        exit(1);
    }
    cout << "time left: " << max_runtime << "\n";
    iter_move_gen->calculate_moves();
    // Set initial best move value to null move - Guaranteed to change
    Move best_move = Move(0);
    double best_score = -numeric_limits<double>::infinity();
    // Keep track of the actual move to be played
    Move curr_move = Move(0);
    // Keep track of the time when this function started running
    auto start_time = clock();

    // Initialize path_scores:
    for (int i = 0; i < iter_move_gen->get_depth(); ++i) {
        path_scores.push((i % 2) ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity());
    }

    while (iter_move_gen->make_move()) {
        // If the depth == 2 the current move is a move the CPU can play:
        if (iter_move_gen->get_curr_depth() == 2) {
            curr_move = iter_move_gen->get_curr_move();
        }
        // Update path scores based on NegaMax algorithm
        for (int d = iter_move_gen->get_curr_depth(); d > iter_move_gen->get_depth() || (iter_move_gen->get_num_sibling_moves_left(d) == 0); --d) {
            // Travel up and evaluate:
            if (d > iter_move_gen->get_depth()) {
                // Evaluate and add position
                double eval = eval_current_pos();
                path_scores.push(eval);
            }
            if (path_scores.size() == 1) {
                // Make final move (undo_move) and return
                iter_move_gen->make_move();
                // Return the best move:
                return best_move;
            }
            int new_score = path_scores.top();
            path_scores.pop();
            int old_score = path_scores.top();
            path_scores.pop();
            int mult = (d % 2) ? -1 : 1;
            path_scores.push(mult * max(mult * old_score, mult * new_score));
            // Update best move as needed:
            if (path_scores.size() == 1 && path_scores.top() != old_score) {
                best_score = path_scores.top();
                best_move = curr_move;
                cout << "new best score: " << best_score << "\n";
            }
        }

        // Add placeholder values back in to path_scores needed:
        for (int i = path_scores.size(); i < iter_move_gen->get_depth(); ++i) {
            path_scores.push((i % 2) ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity());
        }
        
        // Check to make sure there is time remaining:
        if ((float)((clock() - start_time) / CLOCKS_PER_SEC) > (float)max_runtime) {
            // Stop searching moves if the time limit has exceeded
            return Move(0);
        }
    }
    return best_move;
}

Move MovePick::find_best_move_given_time(int time) {
    // NOTE: time is the total time the program should search for a move
    // Use iterative deepening to find the best move given the max time:
    auto start_time = clock();
    Move best_move = Move(0), pot_best_move = Move(0);
    Move curr_move = move_gen->get_curr_move();
    for (int depth = 1; clock() < (start_time + time * CLOCKS_PER_SEC); ++depth) {
        if (depth == 4) {
            break;
        }
        iter_depth = depth;
        // Clear the path score stack:
        while (!path_scores.empty()) {
            path_scores.pop();
        }

        // Set iter_move_gen based on move_gen:
        if (iter_move_gen) {
            delete iter_move_gen;
        }
        
        iter_move_gen = new MoveGen(init, gen, depth, move_gen->get_active_player(), move_gen->get_piece_board(), \
                                    move_gen->get_en_passant(), curr_move.get_castles());

        // Set the maximum depth for the move_gen object:
        // Calculate available runtime
        int rem_time = ((start_time + time * CLOCKS_PER_SEC) - clock()) / CLOCKS_PER_SEC;
        // Find the potential best move for this depth:
        pot_best_move = find_best_move(rem_time);
        if (pot_best_move.get_move() != 0) {
            best_move = pot_best_move;
        }
        // NOTE: If we run out of time at a certain depth, iter_move_gen mightnot match move_gen, but will have pot_best_move = 0
        if (iter_move_gen->get_piece_board() != move_gen->get_piece_board() && pot_best_move.get_move() != 0) {
            cout << "Moves not being made unmade correctly\n";
            cout << "Iterated: \n";
            iter_move_gen->print_piece_board();
            cout << "Correct: \n";
            move_gen->print_piece_board();
            exit(1);
        }
    }
    if (best_move.get_move() == 0) {
        exit(1);
    }
    return best_move;
}

void MovePick::set_piece_arr(int piece, int game, vector<int> squares) {
    for (int i = 0; i < 64; ++i) {
        piece_square[piece][game][i] = squares[i];
    }
}

void MovePick::init_piece_squares() {
    // TODO: Add in end game position evaluations
    // Initialize pawn moves:
    vector<int> pawn_pos_eval = {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };
    set_piece_arr(Move::WHITE_PAWN, 0, pawn_pos_eval);
    set_piece_arr(Move::WHITE_PAWN, 1, pawn_pos_eval);
    // Mirror and initialize for the black pawns:
    flip_chess_board(pawn_pos_eval);
    set_piece_arr(Move::BLACK_PAWN, 0, pawn_pos_eval);
    set_piece_arr(Move::BLACK_PAWN, 1, pawn_pos_eval);

    // Initialize knight moves:
    vector<int> knight_pos_eval = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    };
    set_piece_arr(Move::WHITE_KNIGHT, 0, knight_pos_eval);
    set_piece_arr(Move::WHITE_KNIGHT, 1, knight_pos_eval);
    // Mirror and initialize for the black knight:
    flip_chess_board(knight_pos_eval);
    set_piece_arr(Move::BLACK_KNIGHT, 0, knight_pos_eval);
    set_piece_arr(Move::BLACK_KNIGHT, 1, knight_pos_eval);

    // Initialize bishop moves:
    vector<int> bishop_pos_eval = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    };
    set_piece_arr(Move::WHITE_BISHOP, 0, bishop_pos_eval);
    set_piece_arr(Move::WHITE_BISHOP, 1, bishop_pos_eval);
    // Mirror and initialize for the black bishop:
    flip_chess_board(bishop_pos_eval);
    set_piece_arr(Move::BLACK_BISHOP, 0, bishop_pos_eval);
    set_piece_arr(Move::BLACK_BISHOP, 1, bishop_pos_eval);

    // Initialize rook moves:
    vector<int> rook_pos_eval = {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    };
    set_piece_arr(Move::WHITE_ROOK, 0, rook_pos_eval);
    set_piece_arr(Move::WHITE_ROOK, 1, rook_pos_eval);
    // Mirror and initialize for the black rook:
    flip_chess_board(rook_pos_eval);
    set_piece_arr(Move::BLACK_ROOK, 0, rook_pos_eval);
    set_piece_arr(Move::BLACK_ROOK, 1, rook_pos_eval);

    // Initialize queen moves:
    vector<int> queen_pos_eval = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };
    set_piece_arr(Move::WHITE_QUEEN, 0, queen_pos_eval);
    set_piece_arr(Move::WHITE_QUEEN, 1, queen_pos_eval);
    // Mirror and initialize for the black queen:
    flip_chess_board(queen_pos_eval);
    set_piece_arr(Move::BLACK_QUEEN, 0, queen_pos_eval);
    set_piece_arr(Move::BLACK_QUEEN, 1, queen_pos_eval);

    // Initialize king move for middle game:
    vector<int> king_pos_eval_middle = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    };
    set_piece_arr(Move::WHITE_KING, 0, king_pos_eval_middle);
    // Mirror and initialize for the black king:
    flip_chess_board(king_pos_eval_middle);
    set_piece_arr(Move::BLACK_KING, 0, king_pos_eval_middle);
    

    // Initialize the king move for the end game
    vector<int> king_pos_eval_end = {
        -50,-40,-30,-20,-20,-30,-40,-50,
        -30,-20,-10,  0,  0,-10,-20,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 30, 40, 40, 30,-10,-30,
        -30,-10, 20, 30, 30, 20,-10,-30,
        -30,-30,  0,  0,  0,  0,-30,-30,
        -50,-30,-30,-30,-30,-30,-30,-50
    };
    set_piece_arr(Move::WHITE_KING, 1, king_pos_eval_end);
    // Mirror and initialize for the black king:
    flip_chess_board(king_pos_eval_end);
    set_piece_arr(Move::BLACK_KING, 1, king_pos_eval_end);
}

void MovePick::init_row_masks() {
    row_masks[0] = 0xff00000000000000ULL;
    row_masks[1] = 0x00ff000000000000ULL;
    row_masks[2] = 0x0000ff0000000000ULL;
    row_masks[3] = 0x000000ff00000000ULL;
    row_masks[4] = 0x00000000ff000000ULL;
    row_masks[5] = 0x0000000000ff0000ULL;
    row_masks[6] = 0x000000000000ff00ULL;
    row_masks[7] = 0x00000000000000ffULL;
}

void MovePick::init_col_masks() {
    row_masks[0] = 0x8080808080808080ULL;
    row_masks[1] = 0x4040404040404040ULL;
    row_masks[2] = 0x2020202020202020ULL;
    row_masks[3] = 0x1010101010101010ULL;
    row_masks[4] = 0x0808080808080808ULL;
    row_masks[5] = 0x0404040404040404ULL;
    row_masks[6] = 0x0202020202020202ULL;
    row_masks[7] = 0x0101010101010101ULL;
}

void MovePick::print_path_scores() {
    stack<double> path_score_copy = path_scores;
    vector<int> scores = {};
    cout << "current path scores: ";
    while (path_scores.size()) {
        scores.push_back(path_scores.top());
        cout << path_scores.top() << " ";
        path_scores.pop();
    }
    cout << "\n";
    path_scores = path_score_copy;
}