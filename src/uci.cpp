/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "uci.h"

// Constructor
UCI::UCI(){
  // Initialize the engine
  //this->engine = Engine();

  // Set the initial time
  this->initial_time = time(0);
  std::cout << this->initial_time << "\n";
}

// Get the information of the engine
void UCI::engine_info(){
  std::cout << this->engine.engine_name << "\n";
  std::cout << this->engine.engine_copyright << ", " << this->engine.engine_author << "\n\n";
}

// Get position
void UCI::get_position(std::stringstream& var_stream){
  std::string arg1, arg_next; // First argument and next one
  std::string fen_string; // FEN string
  
  // Consume the first parameter of position: fen or startpos
  var_stream >> arg1;

  // Get the position
  if(arg1=="startpos"){
    // Default position
    fen_string = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    // Consume "moves" token if any
    var_stream >> arg_next;
  }
  else if(arg1=="fen"){
    // FEN
    while(var_stream >> arg_next && arg_next != "moves")
      fen_string+=arg_next+" ";
  }
  else
    return;

  // Pass the position to the engine
  this->engine.set_board(fen_string);

  // Make the moves (if any)
  while(var_stream >> arg_next)
    this->engine.make_move(arg_next);
}

// The primary UCI communication loop
void UCI::comm_loop(){
  // Variable representing the command from the GUI
  // Implemented UCI commands:
  // GUI to engine: quit, uci, position fen|startpos [fen_str] moves [moves]
  // Engine to GUI: id name, id author, uciok
  std::string command;
  std::string line_command;

  // The communication loop
  do{
    // Receive the input from the GUI by lines
    getline(std::cin, line_command);

    // Get the first word --> main command
    std::stringstream var_stream(line_command); //  >> command;
    var_stream >> command;

    // Different commands
    if(command=="uci"){ // It tellsengine to use the uci communication
      // id
      std::cout << "id name " << this->engine.engine_name << " " << this->engine.engine_version << "\n";
      std::cout << "id author " << this->engine.engine_author << "\n";
      // For the moment, no option is available
      // uciok
      std::cout << "uciok" << "\n";
    }
    else if(command=="ucinewgame"){
      // It should clear any search
    }
    else if(command=="position"){
      this->get_position(var_stream);
    }
    else if(command=="go"){
      // Pass several parameters to search --> TBD!!!

      // Start computing
      this->engine.possible_moves();

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