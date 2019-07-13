/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "uci.h"

// Constructor
UCI::UCI(){
  // Set the initial time
  this->initial_time = time(0);
  std::cout << this->initial_time << "\n";
}

// Get the information of the engine
void UCI::engine_info(){
  std::cout << this->engine_name << "\n";
  std::cout << this->engine_copyright << ", " << this->engine_author << "\n\n";
}

// The primary UCI communication loop
void UCI::comm_loop(){
  // Variable representing the input from the user
  // Implemented UCI commands:
  // GUI to engine: quit
  // Engine to GUI: id name, id author, uciok
  std::string input; 

  // The communication loop
  do{
    // Receive the input
    std::cin >> input;

    // Different commands
    if(input=="uci"){ // It tellsengine to use the uci communication
      // id
      std::cout << "id name " << this->engine_name << " " << this->engine_version << "\n";
      std::cout << "id author " << this->engine_author << "\n";
      // For the moment, no option is available
      // uciok
      std::cout << "uciok" << "\n";
    }
  } while (input != "quit");
}