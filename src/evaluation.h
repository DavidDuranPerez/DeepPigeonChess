/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef EVALUATION_H_INCLUDED
#define EVALUATION_H_INCLUDED

// Own libraries
#include "board.h"

// Class for the evaluation
class Evaluation{
public:
    Evaluation(); // Default constructor
    double eval_pos(Board board); // Evaluate a position

private:
    // Functions
    double count_pieces(); // Count just the pieces

    // Attributes
    Board board;
};

#endif