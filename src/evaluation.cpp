/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "evaluation.h"

// Mapping for the points assigned to each piece
std::map<char, double> map_pieces = {
    {'p', -100.0},
    {'P', 100.0},
    {'b', -300.0},
    {'B', 300.0},
    {'n', -300.0},
    {'N', 300.0},
    {'r', -500.0},
    {'R', 500.0},
    {'q', -900.0},
    {'Q', 900.0}};

// Default constructor
Evaluation::Evaluation(){
};

// Evaluate a position (black is negative, white is positive) in cp
double Evaluation::eval_pos(Board board){
    // Set the board position
    this->board=board;

    // Variable to store the result
    double result=0.0;

    // First, see if it is checkmate
    if(this->board.is_checkmated()){
        if(this->board.get_turn())
            return NEG_INF; // White is checkmated
        else
            return POS_INF; // Black is checkmated
    }
    else{
        // 1. Count pieces
        result+=this->count_pieces();

        // Return the result
        return result;
    }
}

// Count just the pieces
double Evaluation::count_pieces(){
    // Variable to store the value
    double result=0.0;

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
        }
    }

    // Return the value
    return result;
}