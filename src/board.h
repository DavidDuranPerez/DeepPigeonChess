/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

// 3rd libraries
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

// Class for each square
class Square{
public:
    // Constructor
    Square(int row=1, char column='a', char piece='R', bool is_valid=true);

private:
    // Attributes
    int row; // 1, 2, 3...
    char column; // a, b, c...
    char piece; // Capital letters for white: P, R, N, B, Q, K; for black: p, r, n, b, q, k
    bool is_valid; // True for the valid squares, false for the auxiliary ones
};

// Class for the board
class Board{
public:
    // Constructor
    Board();

private:
    // Squares
    Square squares[12][12];

    // Convert FEN to position
    void fen2pos(std::string fen_str);

    // Split a string by a delimiter
    template <class Container>
    void split_str(const std::string&, Container&, char delim = ' ');
};

#endif