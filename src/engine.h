/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

// Own libraries
#include "board.h"

// Class for the engine
class Engine{
public:
    Engine(); // Constructor
    void set_board(std::string fen_str); // Set a board
    void make_move(std::string move); // Make a move to the board

    // Public parameters
    std::string engine_name = "Deep Pigeon";
    std::string engine_author = "David Duran";
    std::string engine_copyright = "Copyright (C) 2019";
    std::string engine_version = "0.1";

private:
    // Parameters
    Board board;
};

#endif