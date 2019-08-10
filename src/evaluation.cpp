/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "evaluation.h"

// Mapping for the points assigned to each piece
std::map<char, int> map_pieces = {
    {'p', -100},
    {'P', 100},
    {'b', -300},
    {'B', 300},
    {'n', -300},
    {'N', 300},
    {'r', -500},
    {'R', 500},
    {'q', -900},
    {'Q', 900}};

// Default constructor
Evaluation::Evaluation(){
};

// Evaluate a position (black is negative, white is positive) in cp
int Evaluation::eval_pos(Board board){
    // Set the board position
    this->board=board;

    // Variable to store the result
    int result=0.0;

    // First, see if it is checkmate
    if(this->board.is_checkmated()){
        if(this->board.get_turn())
            return NEG_INF; // White is checkmated
        else
            return POS_INF; // Black is checkmated
    }
    else{
        // 1. Count pieces + 2. Assign a value to each piece depending on the square
        result+=this->piece_square();

        // Return the result
        return result;
    }
}

// Piece-square table
int Evaluation::piece_square(){
    // Table for King
    int king_table[8][8] = {
        {20, 30, 10, 0, 0, 10, 30, 20}, // row 0
        {20, 20, 0, 0, 0, 0, 20, 20}, // row 1
        {-10, -20, -20, -20, -20, -20, -20, -10}, // row 2
        {-20, -30, -30, -40, -40, -30, -30, -20}, // row 3
        {-30, -40, -40, -50, -50, -40, -40, -30}, // row 4
        {-30, -40, -40, -50, -50, -40, -40, -30}, // row 5
        {-30, -40, -40, -50, -50, -40, -40, -30}, // row 6
        {-30, -40, -40, -50, -50, -40, -40, -30} // row 7
    };
    // Table for queen
    int queen_table[8][8] = {
        {-20, -10, -10, -5, -5, -10, -10, -20}, // row 0
        {-10, 0, 5, 0, 0, 0, 0, -10}, // row 1
        {-10, 5, 5, 5, 5, 5, 0, -10}, // row 2
        {0, 0, 5, 5, 5, 5, 0, -5}, // row 3
        {-5, 0, 5, 5, 5, 5, 0, -5}, // row 4
        {-10, 0, 5, 5, 5, 5, 0, -10}, // row 5
        {-10, 0, 0, 0, 0, 0, 0, -10}, // row 6
        {-20, -10, -10, -5, -5, -10, -10, -20} // row 7
    };
    // Table for rook
    int rook_table[8][8] = {
        {0, 0, 0, 5, 5, 0, 0, 0}, // row 0
        {-5, 0, 0, 0, 0, 0, 0, -5}, // row 1
        {-5, 0, 0, 0, 0, 0, 0, -5}, // row 2
        {-5, 0, 0, 0, 0, 0, 0, -5}, // row 3
        {-5, 0, 0, 0, 0, 0, 0, -5}, // row 4
        {-5, 0, 0, 0, 0, 0, 0, -5}, // row 5
        {5, 10, 10, 10, 10, 10, 10, 5}, // row 6
        {0, 0, 0, 0, 0, 0, 0, 0} // row 7
    };
    // Table for bishop
    int bishop_table[8][8] = {
        {-20, -10, -10, -10, -10, -10, -10, -20}, // row 0
        {-10, 5, 0, 0, 0, 0, 5, -10}, // row 1
        {-10, 10, 10, 10, 10, 10, 10, -10}, // row 2
        {-10, 0, 10, 10, 10, 10, 0, -10}, // row 3
        {-10, 5, 5, 10, 10, 5, 5, -10}, // row 4
        {-10, 0, 5, 10, 10, 5, 0, -10}, // row 5
        {-10, 0, 0, 0, 0, 0, 0, -10}, // row 6
        {-20, -10, -10, -10, -10, -10, -10, -20} // row 7
    };
    // Table for knight
    int knight_table[8][8] = {
        {-50, -40, -30, -30, -30, -30, -40, -50}, // row 0
        {-40, -20, 0, 5, 5, 0, -20, -40}, // row 1
        {-30, 5, 10, 15, 15, 10, 5, -30}, // row 2
        {-30, 0, 15, 20, 20, 15, 0, -30}, // row 3
        {-30, 5, 15, 20, 20, 15, 5, -30}, // row 4
        {-30, 0, 10, 15, 15, 10, 0, -30}, // row 5
        {-40, -20, 0, 0, 0, 0, -20, -40}, // row 6
        {-50, -40, -30, -30, -30, -30, -40, -50} // row 7
    };
    // Table for pawn
    int pawn_table[8][8] = {
        {0, 0, 0, 0, 0, 0, 0, 0}, // row 0
        {5, 10, 10, -20, -20, 10, 10, 5}, // row 1
        {5, -5, -10, 0, 0, -10, -5, 5}, // row 2
        {0, 0, 0, 20, 20, 0, 0, 0}, // row 3
        {5, 5, 10, 25, 25, 10, 5, 5}, // row 4
        {10, 10, 20, 30, 30, 20, 10, 10}, // row 5
        {50, 50, 50, 50, 50, 50, 50, 50}, // row 6
        {0, 0, 0, 0, 0, 0, 0, 0} // row 7
    };

    // Variable to store the value
    int result=0;

    // Size of the board
    int board_size = this->board.get_size();

    // Make the double loop to search the piece
    for (size_t i = 2; i < board_size-2; i++)
    { // row
        for (size_t j = 2; j < board_size-2; j++)
        { // column
            // Get the piece
            char piece=this->board.get_piece(i,j);

            // Assign the values for each piece
            if(piece!='K' && piece!='k' && piece!=' ')
                result+=map_pieces[piece];

            // Switch
            switch(piece){
                case 'K': // White King
                    result+=king_table[i-2][j-2];
                    break;
                case 'k': // Black King
                    result+=king_table[8-(i-2)][8-(j-2)];
                    break;
                case 'Q': // White Queen
                    result+=queen_table[i-2][j-2];
                    break;
                case 'q': // Black Queen
                    result+=queen_table[8-(i-2)][8-(j-2)];
                    break;
                case 'R': // White Rook
                    result+=rook_table[i-2][j-2];
                    break;
                case 'r': // Black Rook
                    result+=rook_table[8-(i-2)][8-(j-2)];
                    break;
                case 'B': // White Bishop
                    result+=bishop_table[i-2][j-2];
                    break;
                case 'b': // Black Bishop
                    result+=bishop_table[8-(i-2)][8-(j-2)];
                    break;
                case 'N': // White Knight
                    result+=knight_table[i-2][j-2];
                    break;
                case 'n': // Black Knight
                    result+=knight_table[8-(i-2)][8-(j-2)];
                    break;
                case 'P': // White pawns
                    result+=pawn_table[i-2][j-2];
                    break;
                case 'p': // Black pawns
                    result+=pawn_table[8-(i-2)][8-(j-2)];
                    break;
                default: // No piece
                    break;
            }
        }
    }

    // Return the value
    return result;
}