cd ../src

g++ -g3 -gdwarf-2 main.cpp uci.cpp board.cpp utilities.cpp timing.cpp engine.cpp mover.cpp evaluation.cpp -std=c++11 -pthread -O0 -o ../dist/DeepPigeon_debug.exe

pause