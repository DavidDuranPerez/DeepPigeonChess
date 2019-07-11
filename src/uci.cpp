/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "uci.h"

// Constructor
UCI::UCI(){
    // Set the initial time
    initial_time = time(0);
    std::cout << initial_time << "\n";
}

// Get the information of the engine
void UCI::engine_info(){
    std::cout << engine_name << "\n";
    std::cout << engine_copyright << ", " << engine_author << "\n";
}