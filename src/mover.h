/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019-2020, David Duran
*/
#ifndef MOVER_H_INCLUDED
#define MOVER_H_INCLUDED

// 3rd libraries
#include <vector>
#include <ctime>

// Own libraries
#include "board.h"
#include "utilities.h"

// Variable for profiling
bool const PROFILING=false;
extern double ANALYZED_FUNCTION_TIME;
extern int ANALYZED_FUNCTION_COUNTER;
std::string const ANALYZED_FUNCTION_NAME="is_checked";

// Class in charge of providing all possible moves
class Mover{
public:
    // Functions called by other classes
    Mover(); // Default constructor
    std::vector<std::string> possible_moves(Board &board, bool white2move, bool show_moves, bool check_castling=true); // Get a list of all possible moves

private:
    // Attributes
    Board board;

    // Auxiliary Functions
    bool is_in_array(char ch, char (&arr)[6]); // Is the character in the array
    bool is_in_array2(char ch, char (&arr)[12]); // Is the character in the array
    bool is_in_vector(std::string str_val, std::vector<std::string> str_vec); // Is a string in a vector
    std::string notate_square(int i, int j); // Notate a square
    std::tuple<int, int> find_king(bool white2move); // Find the square of the king

    // Movement Functions
    bool is_enpassant_discovered_check(bool is_white, int j_left_pawn, int j_right_pawn, int i_pawns); // Check en-passant discovered check
    std::vector<std::string> pawn_moves(int i, int j, bool is_white, bool record_attack=false); // Pawn moves from the position i,j
    std::string basic_move_capture(int i_target, int j_target, bool is_white, bool is_king=false); // It tells which kind of move it is (normal, capture, block, invalid...)
    void RBQ_pinned(int i, int j, bool is_white, int (&grad_x)[8], int (&grad_y)[8], int N_grad); // To identify the pinned pieces by rooks, bishops or queens
    std::vector<std::string> RBQ_moves(int i, int j, bool is_white, int (&grad_x)[8], int (&grad_y)[8], int N_grad, bool record_attack/*=false*/); // Common part of moves for rooks, bishops and queens
    std::vector<std::string> rook_moves(int i, int j, bool is_white, bool record_attack=false); // Rook moves from the position i,j
    std::vector<std::string> knight_moves(int i, int j, bool is_white, bool record_attack=false); // Knight moves from the position i,j
    std::vector<std::string> bishop_moves(int i, int j, bool is_white, bool record_attack=false); // Bishop moves from the position i,j
    std::vector<std::string> queen_moves(int i, int j, bool is_white, bool record_attack=false); // Queen moves from the position i,j
    std::vector<std::string> king_moves(int i, int j, bool is_white, bool check_castling=true, bool record_attack=false); // King moves from the position i,j

    // Function to set attacked squares
    void set_attacked_squares();
};

#endif