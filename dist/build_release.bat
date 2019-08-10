cd ../src

g++ -g main.cpp uci.cpp board.cpp engine.cpp mover.cpp evaluation.cpp -std=c++11 -O3 -o ../dist/DeepPigeon_release.exe

pause