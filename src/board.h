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
#include <vector>
#include <tuple>

// Class for each square
class Square{
public:
    // Constructor
    Square(int row=1, char column='a', char piece='R', bool is_valid=true, bool attacked_by_white=false, bool attacked_by_black=false);
    // Gettters
    char get_piece();
    char get_col(){return this->column;};
    int get_row(){return this->row;};
    bool get_valid(){return this->is_valid;};
    bool is_attacked_by_white(){return this->attacked_by_white;};
    bool is_attacked_by_black(){return this->attacked_by_black;};
    bool get_white_capture(){return this->capture_mask_white;};
    bool get_black_capture(){return this->capture_mask_black;};
    bool get_white_push(){return this->push_mask_white;};
    bool get_black_push(){return this->push_mask_black;};
    bool is_pinned_piece(){return this->is_pinned;};
    std::vector<std::string> get_x_ray(){return this->x_ray_pinned;};
    // Set piece
    void set_piece(char piece);
    // Set attacked squares
    void set_white_attack(bool is_attacked){this->attacked_by_white=is_attacked;};
    void set_black_attack(bool is_attacked){this->attacked_by_black=is_attacked;};
    // Set capture squares
    void set_white_capture(bool val_bool){this->capture_mask_white=val_bool;};
    void set_black_capture(bool val_bool){this->capture_mask_black=val_bool;};
    // Set block squares
    void set_white_push(bool val_bool){this->push_mask_white=val_bool;};
    void set_black_push(bool val_bool){this->push_mask_black=val_bool;};
    // Set a pinned piece
    void set_pinned(bool val_bool){this->is_pinned=val_bool;};
    void set_x_ray(std::vector<std::string> x_ray){this->x_ray_pinned=x_ray;};

private:
    // Attributes
    int row; // 1, 2, 3...
    char column; // a, b, c...
    char piece; // Capital letters for white: P, R, N, B, Q, K; for black: p, r, n, b, q, k
    bool is_valid; // True for the valid squares, false for the auxiliary ones
    bool attacked_by_white; // True if the square is attacked by white, false otherwise
    bool attacked_by_black; // True if the square is attacked by black, false otherwise
    bool capture_mask_white; // If not in check, true by default. Otherwise, only true where we can capture by white
    bool capture_mask_black; // If not in check, true by default. Otherwise, only true where we can capture by black
    bool push_mask_white; // If not in check, true by default. Otherwise, only true where we can block by white
    bool push_mask_black; // If not in check, true by default. Otherwise, only true where we can block by black
    bool is_pinned; // It tells if a piece is pinned or not
    std::vector<std::string> x_ray_pinned; // The possible moves for the pinned pieces
};

// Class for the board
class Board{
public:
    // Constructor
    Board(std::string fen_string);
    Board(); // Default one
    // Move
    void move(std::string fromto, bool print_board);
    // Reset attacked squares
    void reset_attacked_squares();
    // Set attacked squares
    void set_white_attack(int i, int j){this->squares[i][j].set_white_attack(true);};
    void set_black_attack(int i, int j){this->squares[i][j].set_black_attack(true);};
    void set_color_attack(bool is_white, int i, int j){(is_white) ? this->squares[i][j].set_white_attack(true) : this->squares[i][j].set_black_attack(true);};
    // Set capture mask
    void set_capture_mask(bool is_white, int i, int j);
    // Set push masks
    void empty_push_mask(bool is_white);
    void set_push_mask(bool is_white, int i_check, int j_check);
    // Set block squares
    void set_white_push(int i, int j){this->squares[i][j].set_white_push(true);};
    void set_black_push(int i, int j){this->squares[i][j].set_black_push(true);};
    // Set pinned piece
    void set_pinned_piece(int i, int j){this->squares[i][j].set_pinned(true);};
    void set_x_ray_pinned(int i, int j, std::vector<std::string> x_ray_pinned){this->squares[i][j].set_x_ray(x_ray_pinned);};
    // Getters
    int get_size(){return sizeof(this->squares) / sizeof(*this->squares);}; // Size
    bool get_turn(){return this->white_moves;}; // White to move?
    char get_piece(int i, int j){return this->squares[i][j].get_piece();}; // Get a piece of a square
    int get_row(int i, int j){return this->squares[i][j].get_row();}; // Get a row of a square
    char get_col(int i, int j){return this->squares[i][j].get_col();}; // Get a column of a square
    bool is_valid(int i, int j){return this->squares[i][j].get_valid();}; // See if a square is valid
    bool is_castle_allowed(char type_castle); // See if a type of castle is allowed
    bool is_enpassant(int i, int j); // Check if a square is a target en-passant square
    bool is_checkmated(){return this->checkmated;}; // See if checkmated
    int get_fullmoves(){return this->fullmoves;}; // Get the fullmoves
    bool is_square_attacked_by_white(int i, int j){return this->squares[i][j].is_attacked_by_white();};
    bool is_square_attacked_by_black(int i, int j){return this->squares[i][j].is_attacked_by_black();};
    bool get_capture(bool is_white, int i, int j){return (is_white) ? this->squares[i][j].get_white_capture() : this->squares[i][j].get_black_capture();};
    bool get_push(bool is_white, int i, int j){return (is_white) ? this->squares[i][j].get_white_push() : this->squares[i][j].get_black_push();};
    bool is_pinned_piece(int i, int j){return this->squares[i][j].is_pinned_piece();};
    std::vector<std::string> get_x_ray_pinned(int i, int j){return this->squares[i][j].get_x_ray();};

    // Setter
    void set_checkmate(bool checkmated){this->checkmated=checkmated;}; // Set a checkmate

    // Setter for checkers
    void set_checker_white(int i, int j){this->checkers_white.push_back(std::make_tuple(i, j));};
    void set_checker_black(int i, int j){this->checkers_black.push_back(std::make_tuple(i, j));};
    void set_checker_color(bool is_white, int i, int j){(is_white) ? this->checkers_white.push_back(std::make_tuple(i, j)) : this->checkers_black.push_back(std::make_tuple(i, j));};

    // Get number of checkers
    int get_num_checkers(bool is_white){return is_white ? this->checkers_black.size() : this->checkers_white.size();};
    std::tuple<int, int> get_checker(bool is_white){return is_white ? this->checkers_black[0] : this->checkers_white[0];}; // Only the first one

    // Print board
    void print_board();
    void print_attacked_by_white();
    void print_attacked_by_black();

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
    std::vector<std::tuple<int, int>> checkers_white; // A list of white pieces checking the black king
    std::vector<std::tuple<int, int>> checkers_black; // A list of black pieces checking the white king

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