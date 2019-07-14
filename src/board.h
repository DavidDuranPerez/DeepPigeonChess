/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

// 3rd libraries
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <map>

// Class for each square
class Square{
public:
    // Constructor
    Square(int row=1, char column='a', char piece='R', bool is_valid=true);
    // Get piece
    char get_piece();

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
    // Attributes
    Square squares[12][12]; // All 12x12 squares (the external 2 rows and columns are auxiliary only)
    bool debugging; // For debugging purposes
    bool white_moves; // Does white has to move?
    std::string allowed_castles; // KQkq are the maximum
    std::string en_passant_target; // En passant target square in algebraic notation (regardless of whether there is a pawn in position to make the capture)
    int halfmoves; // Number of halfmoves since last capture of pawn move
    int fullmoves; // Number of fullmoves (starting at 1 and increased after black move)

    // Convert FEN to position
    void fen2pos(std::string fen_str);

    // Convert FEN row to fixed 8
    std::string convert_fenrow(std::string fen_row);

    // Split a string by a delimiter
    template <class Container>
    void split_str(const std::string&, Container&, char delim = ' ');
};

#endif