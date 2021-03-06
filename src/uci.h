/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019-2020, David Duran
*/
#ifndef UCI_H_INCLUDED
#define UCI_H_INCLUDED

// 3rd libraries
#include <iostream>
#include <ctime>
#include <string>
#include <thread>

// Own libraries
#include "engine.h"

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
  void get_go(std::stringstream& var_stream); // Get go commands

private:
  // Parameters
  Engine engine=Engine();
  std::time_t initial_time;
  bool debugging;
};

#endif