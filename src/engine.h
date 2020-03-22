/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019-2020, David Duran
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

// 3rd libraries
#include <vector>
#include <algorithm>
#include <future>
#include <chrono>

// Own libraries
#include "board.h"
#include "evaluation.h"
#include "mover.h"
#include "utilities.h"
#include "timing.h"

// Class representing a node (every time you make a possible move and evaluate it)
class Node{
public:
  Node(); // Default constructor

  // Main attributes
  std::string move=""; // String representing the move
  std::string bestmove=""; // String representing the best move of the children
  int score=0; // Score
  int depth=1; // Number of halfmoves
  std::vector<Node> children={}; // Children
};


// Class for the engine
class Engine{
public:
    // Main functions
    Engine(); // Constructor
    void set_board(std::string fen_str); // Set a board
    void make_move(std::string move, bool print_board); // Make a move to the board
    void compute(std::atomic<bool> &stop_flag, std::clock_t begin); // Compute the best move

    // Public parameters
    std::string engine_name = "Deep Pigeon";
    std::string engine_author = "David Duran";
    std::string engine_copyright = "Copyright (C) 2019-2020";
    std::string engine_version = "0.1";

    // Setters
    void set_searchmove(std::string smove){this->searchmoves.push_back(smove);};
    void activate_ponder(){this->ponder=true;};
    void activate_infinite(){this->infinite=true;};
    void set_wtime(double wtime){this->wtime=wtime;};
    void set_btime(double btime){this->btime=btime;};
    void set_winc(double winc){this->winc=winc;};
    void set_binc(double binc){this->binc=binc;};
    void set_movetime(double movetime){this->movetime=movetime;};
    void set_movestogo(int movestogo){this->movestogo=movestogo;};
    void set_depth(int depth){this->depth=depth;};
    void set_nodes(int nodes){this->nodes=nodes;};
    void set_mate(int mate){this->mate=mate;};

    // Initialize parameters
    void initialize_parameters();

    // Get best move + ponder
    void get_bestmove(){sync_cout << "bestmove " << this->bestmove << " ponder " << this->bestponder << sync_endl;};

private:
    // Minimax function
    int minimax(Node &node, int depth, bool maximizingPlayer, Evaluation eval, Mover mover, std::atomic<bool> &stop_flag, Timing timing);
    int alphabeta(Node &node, int depth, int alpha, int beta, bool maximizingPlayer, Evaluation eval, Mover mover, std::atomic<bool> &stop_flag, Timing timing);

    // Display info
    void display_depth(int depth); // Display only the depth
    void display_score(int depth, int score, int nodes, int nps, int time, std::string best_line); // Display the score
    void display_nps(int nps); // Display the nodes per second 
    std::string get_pv(int depth); // Get the best line

    // Parameters
    Board board;
    bool debugging; // For debugging purposes
    bool use_alphabeta; // Choosing between alphabeta or minimax

    // Options/Limits
    std::vector<std::string> searchmoves; // Not used yet!!!!!
    bool ponder=false; // Ponder mode. Not used yet!!!!!!!!
    double wtime=-1.0; // Time left for white (msec).
    double btime=-1.0; // Time left for black (msec).
    double winc=0.0; // White increment per move (msec).
    double binc=0.0; // Black increment per move (msec).
    int movestogo=0; // Moves to go to the next time control. If you only receive wtime, there is no other time control.
    int depth=0; // Search x plies (halfmoves) only.
    int nodes=0; // Search x nodes only. Not used yet!!!!!!!!!!
    int mate=0; // Search for a mate in x moves. Not used yet!!!!!!!!
    double movetime=-1.0; // Search exactly during x msec. Not used yet!!!!!!!!!
    bool infinite=false; // Search until the "stop" command.

    // Results
    Node searched_tree; // List of nodes with their score
    std::string bestmove; // Best immediate move
    std::string bestponder; // Best ponder move
    std::string bestline; // Best line
    int nodes_searched=0;
};

#endif