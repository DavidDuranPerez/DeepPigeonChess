/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef EVALUATION_H_INCLUDED
#define EVALUATION_H_INCLUDED

// Own libraries
#include "board.h"

// Constants
const int POS_INF=100000;
const int NEG_INF=-100000;

// Class for the evaluation
class Evaluation{
public:
    Evaluation(); // Default constructor
    int eval_pos(Board board); // Evaluate a position

private:
    // Functions
    int piece_square(); // Count pieces + Apply piece square table

    // Attributes
    Board board;
};

#endif