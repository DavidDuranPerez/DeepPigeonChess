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
	UCI uci;
	
	// Give the information of the engine
	uci.engine_info();

	// For debugging
	system("pause");

	return 0;
}

