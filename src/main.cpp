/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

// For profiling purposes
double ANALYZED_FUNCTION_TIME=0.0;
int ANALYZED_FUNCTION_COUNTER=0;

// Own libraries
#include "uci.h"

// 3rd party libraries
#include <iostream>

int main()
{
	// For profiling purposes
	std::clock_t begin;
	if(PROFILING)
		begin = clock();

	// Initialize the communication
	UCI uci = UCI();

	// Give the information of the engine
	uci.engine_info();

	// Communication loop
	//std::thread comm_loop(&UCI::comm_loop, uci);
	uci.comm_loop();

	// For profiling purposes
	if(PROFILING){
		std::clock_t end = clock();
    	double elapsed_ms = double(end - begin);
		std::cout << "Total Elapsed Time: " << elapsed_ms << " ms" << std::endl;
		std::cout << "Function " << ANALYZED_FUNCTION_NAME << " Elapsed Time (sum): " << ANALYZED_FUNCTION_TIME << " ms" << std::endl;
		std::cout << "Function " << ANALYZED_FUNCTION_NAME << " # calls: " << ANALYZED_FUNCTION_COUNTER << std::endl; 
		system("pause");
	}

	return 0;
}
