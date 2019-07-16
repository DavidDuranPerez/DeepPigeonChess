/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

// 3rd libraries
#include <iostream>
#include <ctime>
#include <string>

// Main class
class UCI
{
public:
  // Constructor
  UCI();

  // Functions
  void engine_info(); // Print engine information on the console
  void comm_loop(); // The primary UCI communication loop
  void get_position(std::stringstream& var_stream); // Get position from the UCI protocol

private:
  // Parameters
  std::string engine_name = "Deep Pigeon";
  std::string engine_author = "David Duran";
  std::string engine_copyright = "Copyright (C) 2019";
  std::string engine_version = "0.1";

  std::time_t initial_time;
};

#endif