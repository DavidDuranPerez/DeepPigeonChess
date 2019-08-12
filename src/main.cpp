/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

// Own libraries
#include "uci.h"

// 3rd party libraries
#include <iostream>

int main()
{
	// Initialize the communication
	UCI uci = UCI();

	// Give the information of the engine
	uci.engine_info();

	// Communication loop
	//std::thread comm_loop(&UCI::comm_loop, uci);
	uci.comm_loop();

	// For debugging
	//system("pause");

	return 0;
}
