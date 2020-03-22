/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019-2020, David Duran
*/

// Own libraries
#include "utilities.h"

// Used to serialize access to std::cout to avoid multiple threads writing at
// the same time.
std::ostream& operator<<(std::ostream& os, SyncCout sc) {

  static std::mutex m;

  if (sc == IO_LOCK)
      m.lock();

  if (sc == IO_UNLOCK)
      m.unlock();

  return os;
}