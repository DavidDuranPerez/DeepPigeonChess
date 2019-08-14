/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef MOVER_H_INCLUDED
#define MOVER_H_INCLUDED

// 3rd libraries
#include <vector>

// Own libraries
#include "board.h"
#include "utilities.h"

// Class in charge of providing all possible moves
class Mover{
public:
    // Functions called by other classes
    Mover(); // Default constructor
    std::vector<std::string> possible_moves(Board &board, bool white2move, bool show_moves, bool review_checks, bool check_castling=true); // Get a list of all possible moves
    bool is_checked(bool white2move); // See if the king is checked
    bool is_in_check(Board &board, bool white2move, bool show_moves, std::string target_square); // It tells if there is any piece checking a target square in the given position

private:
    // Attributes
    Board board;

    // Functions
    bool is_in_array(char ch, char (&arr)[6]); // Is the character in the array
    std::vector<std::string> moves_in_vector(std::string target, std::vector<std::string> v); // Moves that contain the target
    std::vector<std::string> pawn_moves(int i, int j, bool is_white); // Pawn moves from the position i,j
    bool pawn_checks(int i, int j, bool is_white, std::string target_square); // Return if a pawn from the position i,j gives a check
    std::string basic_move_capture(int i_target, int j_target, bool is_white); // It tells which kind of move it is (normal, capture, block, invalid...)
    std::vector<std::string> rook_moves(int i, int j, bool is_white); // Rook moves from the position i,j
    bool rook_checks(int i, int j, bool is_white, std::string target_square); // Return if a rook from the position i,j gives a check
    std::vector<std::string> knight_moves(int i, int j, bool is_white); // Knight moves from the position i,j
    bool knight_checks(int i, int j, bool is_white, std::string target_square); // Return if a knight from the position i,j gives a check
    std::vector<std::string> bishop_moves(int i, int j, bool is_white); // Bishop moves from the position i,j
    bool bishop_checks(int i, int j, bool is_white, std::string target_square); // Return if a bishop from the position i,j gives a check
    std::vector<std::string> queen_moves(int i, int j, bool is_white); // Queen moves from the position i,j
    bool queen_checks(int i, int j, bool is_white, std::string target_square); // Return if a queen from the position i,j gives a check
    std::vector<std::string> king_moves(int i, int j, bool is_white, bool check_castling=true); // King moves from the position i,j
    bool king_checks(int i, int j, bool is_white, std::string target_square); // Return if a king from the position i,j gives a check
    std::string notate_square(int i, int j); // Notate a square
    std::string find_king(bool white2move); // Find the square of the king
    bool aim_castle(bool white2move, std::vector<std::string> key_squares); // Moves from black that prevent a castle (aiming at key squares)
};

#endif