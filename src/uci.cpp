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
  // Variable representing the command from the GUI
  // Implemented UCI commands:
  // GUI to engine: quit, uci
  // Engine to GUI: id name, id author, uciok
  std::string command;

  // The communication loop
  do{
    // Receive the input from the GUI
    std::cin >> command;

    // Different commands
    if(command=="uci"){ // It tellsengine to use the uci communication
      // id
      std::cout << "id name " << this->engine_name << " " << this->engine_version << "\n";
      std::cout << "id author " << this->engine_author << "\n";
      // For the moment, no option is available
      // uciok
      std::cout << "uciok" << "\n";
    }
    else if(command=="ucinewgame"){
      // It should clear any search
    }
    else if(command=="position"){
      // Pass the position to the engine
    }
    else if(command=="go"){
      // Pass several parameters to search
      // Send info commands --> JUST AN EXAMPLE!!
      std::cout << "info depth 1 seldepth 0" << "\n";
		  std::cout << "info score cp 13  depth 1 nodes 13 time 15 pv d2d4" << "\n"; 
    }
    else if(command=="stop"){
      // Stop searching
      // Send the best move --> JUST AN EXAMPLE!!
      std::cout << "bestmove d2d4" << "\n";
    }
    else if(command=="isready"){
      // Finish anything that is doing
      // Answer back
      std::cout << "readyok" << "\n";
    }
  } while (command != "quit");
}