/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019-2020, David Duran
*/
#ifndef AUX_H_INCLUDED
#define AUX_H_INCLUDED

// 3rd libraries
#include <ostream>
#include <mutex>

// To correctly write with multiple threads
enum SyncCout { IO_LOCK, IO_UNLOCK };
std::ostream& operator<<(std::ostream&, SyncCout);

#define sync_cout std::cout << IO_LOCK
#define sync_endl std::endl << IO_UNLOCK

#endif