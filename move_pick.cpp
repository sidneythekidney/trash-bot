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

    // Initialize masks as needed:
    init_piece_squares();
    init_col_masks();
    init_row_masks();
    init_passed_pawn_masks();
    init_king_safety_masks();
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

// double MovePick::checkmate_eval() {
//     // Determine if a side is in checkmate and evaluate accordingly:
//     if (iter_move_gen->in_check(castle::NONE)) {
//         // Create new MoveGen object and calculate to depth = 1:
//         MoveGen* temp_move_gen = new MoveGen(init, gen, 1, iter_move_gen->get_active_player(), iter_move_gen->get_piece_board(), 
//                                             iter_move_gen->get_en_passant(), iter_move_gen->get_castles());

//         temp_move_gen->calculate_moves();
        
//     }
//     return 0.0;
// }

double MovePick::eval_current_pos() {
    // Static position evaluator

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
    eval += king_safety_eval();

    // Remember the evaluation for this position:
    eval_pos[hash] = eval;
    // This evaluation is for when playing with white, negate if playing for black
    // return (move_gen->get_active_player() ? -1 : 1) * eval;
    return eval;
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

    eval += doubled_pawns_eval();
    eval += pawn_chain_eval();
    eval += isolated_pawns_eval();
    eval += passed_pawns_eval();

    return eval;
}

double MovePick::doubled_pawns_eval() {
    /*****************
     * DOUBLED PAWNS * 
     ****************/
    // Doubled pawns are bad, even worse in the endgame
    // Having two pawns still usually better than having just one pawn 
    double eval = 0;
    U64 white_pawns = iter_move_gen->get_white_pawns();
    U64 black_pawns = iter_move_gen->get_black_pawns();

    for (int i = 0; i < 8; ++i) {
        eval -= (endgame ? 75 : 50) * (count_bits(white_pawns & col_masks[i]) > 1);
        eval += (endgame ? 75 : 50) * (count_bits(black_pawns & col_masks[i]) > 1);
    }

    return eval;
}
        
double MovePick::pawn_chain_eval() {
    /***************
     * PAWN CHAINS * 
     **************/
    /*
        - Pawns are stronger when they support each other
    */
    double eval = 0;
    U64 white_pawns = iter_move_gen->get_white_pawns();
    U64 black_pawns = iter_move_gen->get_black_pawns();
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

double MovePick::isolated_pawns_eval() {
    /******************
     * ISOLATED PAWNS *
     *****************/

    /*
     - Isolated pawns are pawns that have no friendly pawns on adjacent ranks
     - Isolated pawns are generally bad as they can be hard to defend
    */
    U64 white_pawns = iter_move_gen->get_white_pawns();
    U64 black_pawns = iter_move_gen->get_black_pawns();
    vector<int> w_pawns_per_col;
    vector<int> b_pawns_per_col;
    for (int i = 0; i < 8; ++i) {
        w_pawns_per_col.push_back(count_bits(white_pawns & col_masks[i]));
        b_pawns_per_col.push_back(count_bits(black_pawns & col_masks[i]));
    }
    int num_w_isolated = 0;
    int num_b_isolated = 0;
    for (int i = 0; i < 8; ++i) {
        bool w_isolated  = true;
        bool b_isolated = true;
        
        if (i != 0) {
            // check left:
            if (w_pawns_per_col[i-1]) {
                w_isolated = false;
            }
            if (b_pawns_per_col[i-1]) {
                b_isolated = false;
            }
        }
        if (i != 7) {
            // check right:
            if (w_pawns_per_col[i+1]) {
                w_isolated = false;
            }
            if (b_pawns_per_col[i+1]) {
                b_isolated = false;
            }
        }
        if (w_isolated) {
            num_w_isolated += w_pawns_per_col[i];
        }
        if (b_isolated) {
            num_b_isolated += b_pawns_per_col[i];
        }
        // Assume isolated pawns to be slightly bad, slightly worse in the endgame
    }
    return (num_w_isolated - num_b_isolated) * (endgame ? -30.0 : -25.0);
}

double MovePick::passed_pawns_eval() {
    /****************
     * PASSED PAWNS *
     ***************/
    /* 
     - Passed pawns are pawns that make it past the opponents pawns of same and adjacent col
    */
    double eval = 0;
    U64 white_pawns = iter_move_gen->get_white_pawns();
    U64 black_pawns = iter_move_gen->get_black_pawns();

    U64 white_pawns_cp = white_pawns;
    U64 black_pawns_cp = black_pawns;
    while (white_pawns_cp) {
        int pos = get_ls1b(white_pawns_cp);
        eval += (black_pawns & w_passed_pawn_mask[pos]) ? 0 : 75;
        white_pawns_cp = pop_bit(white_pawns_cp, pos);
    }
    while (black_pawns_cp) {
        int pos = get_ls1b(black_pawns_cp);
        eval -= (white_pawns & b_passed_pawn_mask[pos]) ? 0 : 75;
        black_pawns_cp = pop_bit(black_pawns_cp, pos);
    }

    return eval;
}

double MovePick::king_safety_eval() {
    // Get king position and piece masks
    int w_king_pos = get_ls1b(iter_move_gen->get_white_king());
    int b_king_pos = get_ls1b(iter_move_gen->get_black_king());
    U64 white_pieces = iter_move_gen->get_white_pieces();
    U64 black_pieces = iter_move_gen->get_black_pieces();

    // Perform evaluation
    int num_w_protectors = count_bits(white_pieces & w_king_safety_masks[w_king_pos]);
    int num_b_protectors = count_bits(black_pieces & b_king_safety_masks[b_king_pos]);

    return (double)(num_w_protectors - num_b_protectors) * 30.0;
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
    // double best_score = -numeric_limits<double>::infinity();
    // Keep track of the actual move to be played
    Move curr_move = Move(0);
    // Keep track of the time when this function started running
    auto start_time = clock();

    // Initialize path_scores:
    for (int i = 0; i < iter_move_gen->get_depth(); ++i) {
        path_scores.push_back(((i + move_gen->get_active_player()) % 2) ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity());
    }

    while (iter_move_gen->make_move()) {
        // If the depth == 2 the current move is a move the CPU can play:
        if (iter_move_gen->get_curr_depth() == 2) {
            curr_move = iter_move_gen->get_curr_move();
        }
        // Update path scores based on NegaMax algorithm
        for (int d = iter_move_gen->get_curr_depth(); d > iter_move_gen->get_depth() || (iter_move_gen->get_num_sibling_moves_left(d) == 0); --d) {
            // Travel up and evaluate:
            if (iter_move_gen->get_checkmate_status() > checkmate::STALEMATE) {
                // cout << "Found checkmate lol\n";

                double eval = (iter_move_gen->get_checkmate_status() == checkmate::WHITE_CHECKMATE ? 1 : -1) * numeric_limits<double>::infinity();
                // cout << "eval: " << eval << "\n";
                // Inject the evaluation up a level since the checkmate indication occurs after making the move:
                // ISSUE: Checkmates cannot be identified at a depth = 1
                // print_path_scores();
                path_scores[iter_move_gen->get_curr_depth()-2] = eval;
                // update best move if we are at a depth of 1
                if (iter_move_gen->get_curr_depth()-2 == 0) {
                    best_move = curr_move;
                }
                // cout << "depth: " << iter_move_gen->get_curr_depth() << "\n";
                // print_path_scores();
            }
            if (iter_move_gen->get_checkmate_status() == checkmate::STALEMATE) {
                // Handle stalemate evaluation
                // print_path_scores();
                double old_score = path_scores[iter_move_gen->get_curr_depth()-2];
                double mult = (iter_move_gen->get_active_player() == color::WHITE) ? -1.0 : 1.0;
                path_scores[iter_move_gen->get_curr_depth()-2] = mult * max(0.0, mult * old_score);
                // Update best score if needed
                if (iter_move_gen->get_curr_depth()-2 == 0 && path_scores[iter_move_gen->get_curr_depth()-2] == 0.0) {
                    best_move = curr_move;
                }
                // print_path_scores();
                continue;
            }
            if (d > iter_move_gen->get_depth()) {
                // Evaluate and add position
                double eval = eval_current_pos();
                path_scores.push_back(eval);
            }
            if (path_scores.size() == 1) {
                // Make final move (undo_move) and return
                iter_move_gen->make_move();
                // Return the best move:
                return best_move;
            }
            double new_score = path_scores.back();
            path_scores.pop_back();
            double old_score = path_scores.back();
            path_scores.pop_back();

            // cout << "calculated score: " << new_score << "\n";
            // cout << "best score: " << old_score << "\n";
            
            double mult = ((d + move_gen->get_active_player()) % 2) ? -1 : 1;
            path_scores.push_back(mult * max(mult * old_score, mult * new_score));
            // Update best move as needed:
            if (path_scores.size() == 1 && path_scores.back() != old_score) {
                // best_score = path_scores.top();
                best_move = curr_move;
                // cout << "new best score: " << path_scores.back() << "\n\n\n\n";
            }
        }

        // Add placeholder values back in to path_scores needed:
        for (int i = path_scores.size(); i < iter_move_gen->get_depth(); ++i) {
            path_scores.push_back(((i + move_gen->get_active_player()) % 2) ? numeric_limits<double>::infinity() : -numeric_limits<double>::infinity());
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
        cout << "calculating at depth = " << depth << "\n";
        // if (depth == 3) {
        //     break;
        // }
        iter_depth = depth;
        // Clear the path score stack:
        while (!path_scores.empty()) {
            path_scores.pop_back();
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
        cout << "Returning invalid best move\n";
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
    row_masks = {
        0x00000000000000ffULL,
        0x000000000000ff00ULL,
        0x0000000000ff0000ULL,
        0x00000000ff000000ULL,
        0x000000ff00000000ULL,
        0x0000ff0000000000ULL,
        0x00ff000000000000ULL,
        0xff00000000000000ULL
    };
}

void MovePick::init_col_masks() {
    col_masks = {
        0x0101010101010101ULL,
        0x0202020202020202ULL,
        0x0404040404040404ULL,
        0x0808080808080808ULL,
        0x1010101010101010ULL,
        0x2020202020202020ULL,
        0x4040404040404040ULL,
        0x8080808080808080ULL
    };
}

void MovePick::init_passed_pawn_masks() {
    b_passed_pawn_mask = {
        0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
        0x0303030303030000ULL, 0x0707070707070000ULL, 0x0e0e0e0e0e0e0000ULL, 0x1c1c1c1c1c1c0000ULL, 0x3838383838380000ULL, 0x7070707070700000ULL, 0xe0e0e0e0e0e00000ULL, 0xc0c0c0c0c0c00000ULL,
        0x0303030303000000ULL, 0x0707070707000000ULL, 0x0e0e0e0e0e000000ULL, 0x1c1c1c1c1c000000ULL, 0x3838383838000000ULL, 0x7070707070000000ULL, 0xe0e0e0e0e0000000ULL, 0xc0c0c0c0c0000000ULL,
        0x0303030300000000ULL, 0x0707070700000000ULL, 0x0e0e0e0e00000000ULL, 0x1c1c1c1c00000000ULL, 0x3838383800000000ULL, 0x7070707000000000ULL, 0xe0e0e0e000000000ULL, 0xc0c0c0c000000000ULL,
        0x0303030000000000ULL, 0x0707070000000000ULL, 0x0e0e0e0000000000ULL, 0x1c1c1c0000000000ULL, 0x3838380000000000ULL, 0x7070700000000000ULL, 0xe0e0e00000000000ULL, 0xc0c0c00000000000ULL,
        0x0303000000000000ULL, 0x0707000000000000ULL, 0x0e0e000000000000ULL, 0x1c1c000000000000ULL, 0x3838000000000000ULL, 0x7070000000000000ULL, 0xe0e0000000000000ULL, 0xc0c0000000000000ULL,
        0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
        0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
    };

    w_passed_pawn_mask = {
        0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
        0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL,
        0x0000000000000303ULL, 0x0000000000000707ULL, 0x0000000000000e0eULL, 0x0000000000001c1cULL, 0x0000000000003838ULL, 0x0000000000007070ULL, 0x000000000000e0e0ULL, 0x000000000000c0c0ULL,
        0x0000000000030303ULL, 0x0000000000070707ULL, 0x00000000000e0e0eULL, 0x00000000001c1c1cULL, 0x0000000000383838ULL, 0x0000000000707070ULL, 0x0000000000e0e0e0ULL, 0x0000000000c0c0c0ULL,
        0x0000000003030303ULL, 0x0000000007070707ULL, 0x000000000e0e0e0eULL, 0x000000001c1c1c1cULL, 0x0000000038383838ULL, 0x0000000070707070ULL, 0x00000000e0e0e0e0ULL, 0x00000000c0c0c0c0ULL,
        0x0000000303030303ULL, 0x0000000707070707ULL, 0x0000000e0e0e0e0eULL, 0x0000001c1c1c1c1cULL, 0x0000003838383838ULL, 0x0000007070707070ULL, 0x000000e0e0e0e0e0ULL, 0x000000c0c0c0c0c0ULL,
        0x0000030303030303ULL, 0x0000070707070707ULL, 0x00000e0e0e0e0e0eULL, 0x00001c1c1c1c1c1cULL, 0x0000383838383838ULL, 0x0000707070707070ULL, 0x0000e0e0e0e0e0e0ULL, 0x0000c0c0c0c0c0c0ULL,
        0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL, 0x0000000000000000ULL
    };
}

void MovePick::print_path_scores() {
    deque<double> path_score_copy = path_scores;
    vector<int> scores = {};
    cout << "current path scores: ";
    while (path_scores.size()) {
        scores.push_back(path_scores.back());
        cout << path_scores.back() << " ";
        path_scores.pop_back();
    }
    cout << "\n";
    path_scores = path_score_copy;
}

void MovePick::init_king_safety_masks() {
    // Initialize white king safety masks:
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0ULL;
        if (i % 8) {
            // Add left square:
            mask |= 1ULL << (i-1);
            if (i / 8) {
                // Add upper left square
                mask |= 1ULL << (i-9);
            }
        }
        if (i / 8) {
            // Add upper square:
            mask |= 1ULL << (i-8);
        }
        if (i % 8 != 7) {
            // Add right square:
            mask |= 1ULL << (i+1);
            if (i / 8) {
                // Add upper right square
                mask |= 1ULL << (i-7);
            }
        }
        w_king_safety_masks.push_back(mask);
    }   
    // Initialize black king safety masks:
    for (int i = 0; i < 64; ++i) {
        U64 mask = 0ULL;
        if (i % 8) {
            // Add left square:
            mask |= 1ULL << (i-1);
            if (i / 8 != 7) {
                // Add lower left square
                mask |= 1ULL << (i+7);
            }
        }
        if (i / 8 != 7) {
            // Add lower square:
            mask |= 1ULL << (i+8);
        }
        if (i % 8 != 7) {
            // Add right square:
            mask |= 1ULL << (i+1);
            if (i / 8 != 7) {
                // Add upper right square
                mask |= 1ULL << (i+9);
            }
        }
        b_king_safety_masks.push_back(mask);
    }
}