/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "engine.h"

// Default constructor
Engine::Engine(){
};

// Set a board
void Engine::set_board(std::string fen_str){
    this->board=Board(fen_str);
}

// Make a move in the board
void Engine::make_move(std::string move){
    this->board.move(move);
}