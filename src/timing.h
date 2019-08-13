/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef TIMING_H_INCLUDED
#define TIMING_H_INCLUDED

// 3rd libraries
#include <ctime>
#include <algorithm>

// Class representing the time management
class Timing{
public:
    Timing(std::clock_t begin, double time_left, double inc, int move, int moves2go); // Default constructor
    bool time_to_move(); // Function deciding if it is the time to move or not

private:
    // Main attributes
    std::clock_t begin; // Time when we started counting
    double time_left; // Time left to play
    double inc; // Increment
    int move; // Move
    int moves2go; // Moves to go to the next time control
    bool time2move;
};

#endif