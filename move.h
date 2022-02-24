#ifndef MOVE_H
#define MOVE_H

#include "defs.h"

class Move {
    public:
        Move(unsigned int move) : move(move) {
            move_eval = 0.0;
        };

        unsigned int get_move();
        double get_move_eval();
        // Used to deter,ime the color and piece moving in the move member
        enum piece_color {
            EMPTY = 0,
            WHITE_PAWN = 1,
            WHITE_KNIGHT = 2,
            WHITE_BISHOP = 3,
            WHITE_ROOK = 4,
            WHITE_QUEEN = 5,
            WHITE_KING = 6,
            BLACK_PAWN = 7,
            BLACK_KNIGHT = 8,
            BLACK_BISHOP = 9,
            BLACK_ROOK = 10,
            BLACK_QUEEN = 11,
            BLACK_KING = 12,
        };

        bool operator== (Move rhs) {
            if (move == rhs.get_move()) {
                return true;
            }
            return false;
        }

        unsigned int get_from() {
            return move & 0x3f;
        }
        unsigned int get_to() {
            return (move & (0x3f << 6)) >> 6;
        }
        unsigned int get_moved() {
            return (move & (0xf << 12)) >> 12;
        }
        unsigned int get_captured() {
            return (move & (0xf << 16)) >> 16;
        }
        unsigned int get_castles() {
            return (move & (0xf << 20)) >> 20;
        }
        unsigned int get_en_passant() {
            return (move & (1 << 24)) >> 24;
        }
        unsigned int get_double_push() {
            return (move & (1 << 25)) >> 25;
        }
        unsigned int get_promotion() {
            return (move & (1 << 26)) >> 26;
        }
        unsigned int get_castle() {
            return (move & (1 << 27)) >> 27;
        }
        unsigned int get_flags() {
            // Includes enpassant, double push, promotion, and castle
            return (move & (0xf << 24)) >> 24;
        }

    private:
        /*
        move is an unsigned integer representing everything known about the move:

        Assume the move being made is always legal.

        fromSquare - 6 BITS (0-5) representing the square the piece is moving from
        toSquare - 6 BITS (6-11) representing the square the piece is moving to
        movedPiece - 4 BITS (12-15) representing the type of piece being moved (includes color)
        capturedPiece - 4 BITS (16-19) represents the type of piece being captured, if any
        legalCastles - 4 bits (20-23) represents the legal castles in the current position
        enPassant - 1 BIT (24) represents whether the move is an en passant
        doublePawnPush - 1 BIT (25) represents whether a pawn is being pushed two squares
        promotion - 1 BIT (26) represents whether a pawn is being promoted on the current move
        castling - 1 BIT (27) represents whether a castling move occured
        */

        unsigned int move;
        double move_eval;
};

#endif