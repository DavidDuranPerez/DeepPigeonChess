/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019-2020, David Duran
*/

#include "timing.h"

// Default constructor
Timing::Timing(std::clock_t begin, double time_left, double inc, int move, int moves2go){
    this->begin=begin;
    this->time_left=time_left;
    this->inc=inc;
    this->move=move;
    this->moves2go=moves2go;
    this->time2move=false;
}

// Function deciding if it is the time to move or not
bool Timing::time_to_move(){
    if(this->time_left<0 || this->time2move) // This case means that we are not in a time control (we will have already lost otherwise). So no action from the manager
        return false;
    else{
        // Number of moves out of book
        int moves_out_book=this->move; // This shall be changed when using an opening/ending book!!!!!!!
        int n_moves = std::min(moves_out_book, 10); // During these movements, more time to think is reserved
        double factor = 2-n_moves/10; // This is the factor for more time to think

        // Moves to go
        int moves_to_go=this->moves2go;
        int expected_moves=60; // We expect to go to 60
        if(moves_to_go==0) // No other time control
            moves_to_go=expected_moves-this->move;
        if(moves_to_go<0)
            moves_to_go=10; // 10 extra moves
        double expected_inc=moves_to_go*this->inc; // Expected increment in time
        
        // Target time
        double target = (this->time_left+expected_inc)/moves_to_go;
        double time_allocated = factor*target; // In ms

        // See the current time
        std::clock_t curr_time = clock();

        // Difference with the beginning time
        double elapsed_ms = double(curr_time - this->begin);

        // 10 ms of margin
        double time_margin=10.0;
        if((time_allocated-elapsed_ms)<time_margin){
            this->time2move=true;
            return true;
        }
        else
            return false;
    }
}