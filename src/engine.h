/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

// 3rd libraries
#include <vector>

// Own libraries
#include "board.h"


// Class for the engine
class Engine{
public:
    Engine(); // Constructor
    void set_board(std::string fen_str); // Set a board
    void make_move(std::string move); // Make a move to the board
    std::vector<std::string> possible_moves(); // Get a list of all possible moves
    void compute(); // Compute the best move

    // Public parameters
    std::string engine_name = "Deep Pigeon";
    std::string engine_author = "David Duran";
    std::string engine_copyright = "Copyright (C) 2019";
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

    // Get best move
    void get_bestmove(){std::cout << "bestmove " << this->bestmove << "\n";};

private:
    // Functions
    bool is_in_array(char ch, char (&arr)[6]); // Is the character in the array
    std::vector<std::string> pawn_moves(int i, int j, bool is_white); // Pawn moves from the position i,j
    std::string basic_move_capture(int i_target, int j_target, bool is_white); // It tells which kind of move it is (normal, capture, block, invalid...)
    std::vector<std::string> rook_moves(int i, int j, bool is_white); // Rook moves from the position i,j
    std::vector<std::string> knight_moves(int i, int j, bool is_white); // Knight moves from the position i,j
    std::vector<std::string> bishop_moves(int i, int j, bool is_white); // Bishop moves from the position i,j
    std::vector<std::string> queen_moves(int i, int j, bool is_white); // Queen moves from the position i,j
    std::vector<std::string> king_moves(int i, int j, bool is_white); // King moves from the position i,j
    std::string notate_square(int i, int j);

    // Parameters
    Board board;
    bool debugging; // For debugging purposes

    // Options/Limits
    std::vector<std::string> searchmoves; // Not used yet!!!!!
    bool ponder=false; // Ponder mode. Not used yet!!!!!!!!
    double wtime=-1.0; // Time left for white (msec). Not used yet!!!!!!!!
    double btime=-1.0; // Time left for black (msec). Not used yet!!!!!!!!
    double winc=-1.0; // White increment per move (msec). Not used yet!!!!!!
    double binc=-1.0; // Black increment per move (msec). Not used yet!!!!!!
    int movestogo=0; // Moves to go to the next time control. If you only receive wtime, there is no other time control. Not used yet!!!!!!!
    int depth=0; // Search x plies (halfmoves) only. Not used yet!!!!!!!
    int nodes=0; // Search x nodes only. Not used yet!!!!!!!!!!
    int mate=0; // Search for a mate in x moves. Not used yet!!!!!!!!
    double movetime=-1.0; // Search exactly during x msec. Not used yet!!!!!!!!!
    bool infinite=false; // Search until the "stop" command. Not used yet!!!!!!!!

    // Results
    std::string bestmove;
};

#endif