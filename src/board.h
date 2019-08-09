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
    // Gettters
    char get_piece();
    char get_col(){return this->column;};
    int get_row(){return this->row;};
    bool get_valid(){return this->is_valid;};
    // Set piece
    void set_piece(char piece);

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
    Board(std::string fen_string);
    Board(); // Default one
    // Move
    void move(std::string fromto, bool print_board);
    // Getters
    int get_size(){return sizeof(this->squares) / sizeof(*this->squares);}; // Size
    bool get_turn(){return this->white_moves;}; // White to move?
    char get_piece(int i, int j){return this->squares[i][j].get_piece();}; // Get a piece of a square
    int get_row(int i, int j){return this->squares[i][j].get_row();}; // Get a row of a square
    char get_col(int i, int j){return this->squares[i][j].get_col();}; // Get a column of a square
    bool is_valid(int i, int j){return this->squares[i][j].get_valid();}; // See if a square is valid
    bool is_castle_allowed(char type_castle); // See if a type of castle is allowed
    bool is_enpassant(int i, int j); // Check if a square is a target en-passant square
    bool is_checkmated(){return this->checkmated;};

    // Setter
    void set_checkmate(bool checkmated){this->checkmated=checkmated;}; // Set a checkmate

    // Print board
    void print_board();

private:
    // Attributes
    Square squares[12][12]; // All 12x12 squares (the external 2 rows and columns are auxiliary only)
    bool debugging; // For debugging purposes
    bool white_moves; // Does white has to move?
    std::string allowed_castles; // KQkq are the maximum
    std::string en_passant_target; // En passant target square in algebraic notation (regardless of whether there is a pawn in position to make the capture)
    int halfmoves; // Number of halfmoves since last capture of pawn move
    int fullmoves; // Number of fullmoves (starting at 1 and increased after black move)
    bool checkmated=false; // To indicate that it is checkmated

    // Convert FEN to position
    void fen2pos(std::string fen_str);
    std::string subtract_character(std::string orig_str, char ch2remove);

    // Convert FEN row to fixed 8
    std::string convert_fenrow(std::string fen_row);

    // Split a string by a delimiter
    template <class Container>
    void split_str(const std::string&, Container&, char delim = ' ');
};

#endif