/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

// 3rd libraries
#include <vector>

// Own libraries
#include "board.h"


// Class for the engine
class Engine{
public:
    Engine(); // Constructor
    void set_board(std::string fen_str); // Set a board
    void make_move(std::string move); // Make a move to the board
    std::vector<std::string> possible_moves(); // Get a list of all possible moves

    // Public parameters
    std::string engine_name = "Deep Pigeon";
    std::string engine_author = "David Duran";
    std::string engine_copyright = "Copyright (C) 2019";
    std::string engine_version = "0.1";

private:
    // Functions
    bool is_in_array(char ch, char (&arr)[6]); // Is the character in the array
    std::vector<std::string> pawn_moves(int i, int j, bool is_white); // Pawn moves from the position i,j
    std::vector<std::string> rook_moves(int i, int j, bool is_white); // Rook moves from the position i,j
    std::string rook_one_move(int i_target, int j_target, bool is_white); // Rook basic move
    std::vector<std::string> knight_moves(int i, int j, bool is_white); // Knight moves from the position i,j
    std::vector<std::string> bishop_moves(int i, int j, bool is_white); // Bishop moves from the position i,j
    std::vector<std::string> queen_moves(int i, int j, bool is_white); // Queen moves from the position i,j
    std::vector<std::string> king_moves(int i, int j, bool is_white); // King moves from the position i,j
    std::string notate_square(int i, int j);

    // Parameters
    Board board;
    bool debugging; // For debugging purposes
};

#endif