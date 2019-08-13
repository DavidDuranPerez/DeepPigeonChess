/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

// Own libraries
#include "uci.h"


// Constructor
UCI::UCI(){
  // Debugging or not
  this->debugging=false;

  // Set the initial time
  // this->initial_time = time(0);
  // sync_cout << this->initial_time << sync_endl;
}

// Get the information of the engine
void UCI::engine_info(){
  sync_cout << this->engine.engine_name << sync_endl;
  sync_cout << this->engine.engine_copyright << ", " << this->engine.engine_author << sync_endl;
  sync_cout << sync_endl;
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
    this->engine.make_move(arg_next, this->debugging);
}

// Get go commands
void UCI::get_go(std::stringstream& var_stream){
  std::string arg_next, arg_val; // First argument and next one
  
  // Initialize the parameters first
  this->engine.initialize_parameters();

  // Iterate all the commands
  while(var_stream >> arg_next)
  {
    if(arg_next=="searchmoves")
    {
      // The searchmoves are at the end
      while(var_stream >> arg_val)
        this->engine.set_searchmove(arg_val);
    }
    else if(arg_next=="infinite")
      this->engine.activate_infinite();
    else if(arg_next=="ponder")
      this->engine.activate_ponder();
    else if(arg_next =="wtime"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_wtime(std::atof(arg_val.c_str()));
    }
    else if(arg_next =="btime"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_btime(std::atof(arg_val.c_str()));
    }
    else if(arg_next =="winc"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_winc(std::atof(arg_val.c_str()));
    }
    else if(arg_next =="binc"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_binc(std::atof(arg_val.c_str()));
    }
    else if(arg_next =="movetime"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_movetime(std::atof(arg_val.c_str()));
    }
    else if(arg_next =="movestogo"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_movestogo(std::atoi(arg_val.c_str()));
    }
    else if(arg_next =="depth"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_depth(std::atoi(arg_val.c_str()));
    }
    else if(arg_next =="nodes"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_nodes(std::atoi(arg_val.c_str()));
    }
    else if(arg_next =="mate"){
      // Consume the value
      var_stream >> arg_val;
      this->engine.set_mate(std::atoi(arg_val.c_str()));
    }
  }
}

// The primary UCI communication loop
void UCI::comm_loop(){
  // Variable representing the command from the GUI
  // Implemented UCI commands:
  // GUI to engine: quit, uci, position fen|startpos [fen_str] moves [moves]
  // Engine to GUI: id name, id author, uciok
  std::string command;
  std::string line_command;
  std::thread calc_thread; // Thread to calculate the lines
  // Create a boolean that will be passed by reference to the thread in order to stop it
	//std::promise<void> exitSignal;
  std::atomic<bool> stop_flag;

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
      sync_cout << "id name " << this->engine.engine_name << " " << this->engine.engine_version << sync_endl;
      sync_cout << "id author " << this->engine.engine_author << sync_endl;
      // For the moment, no option is available
      // uciok
      sync_cout << "uciok" << sync_endl;
    }
    else if(command=="ucinewgame"){
      // It should clear any search
      // Stop searching
      stop_flag=true;

      // Wait for the thread to join
      if(calc_thread.joinable())
        calc_thread.join();
    }
    else if(command=="position"){
      // Get the position with all the arguments
      this->get_position(var_stream);
    }
    else if(command=="go"){
      // Pass several parameters to search
      this->get_go(var_stream);

      // Start computing
      // Since this is a long process, we put it in a thread, which we can stop
      //Fetch std::future object associated with promise
	    //std::future<void> futureObj = exitSignal.get_future();
      stop_flag=false;
      //this->engine.compute();
      //calc_thread=std::thread(&Engine::compute, this->engine, std::move(futureObj));
      calc_thread=std::thread(&Engine::compute, this->engine, std::ref(stop_flag));
    }
    else if(command=="stop" || command=="quit"){
      // Stop searching
      //Set the value in promise
	    //exitSignal.set_value();
      stop_flag=true;

      // Wait for the thread to join
      if(calc_thread.joinable())
        calc_thread.join();
      
      // Send the best move
      this->engine.get_bestmove();
    }
    else if(command=="isready"){
      // Answer back
      sync_cout << "readyok" << sync_endl;
    }
  } while (command != "quit");

  // Check that the thread has finished
  if(calc_thread.joinable())
    calc_thread.join();
}