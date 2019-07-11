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

class UCI {
    public:
        // Constructor
        UCI();

        // Functions
        void engine_info();

    private:
        // Parameters
        std::string engine_name = "Deep Pigeon";
        std::string engine_author = "David Duran";
        std::string engine_copyright = "Copyright (C) 2019";

        std::time_t initial_time;
};

#endif