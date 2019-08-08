/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/
#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

// 3rd libraries
#include <vector>
#include <algorithm>
#include <ctime>

// Own libraries
#include "board.h"
#include "evaluation.h"

// Structure representing a node (every time you make a possible move and evaluate it)
struct Node{
  std::string move=""; // String representing the move
  double score=0.0; // Score
  int depth=1; // Number of halfmoves
  std::vector<Node> children={}; // Children
};


// Class for the engine
class Engine{
public:
    Engine(); // Constructor
    void set_board(std::string fen_str); // Set a board
    void make_move(std::string move, bool print_board); // Make a move to the board
    std::vector<std::string> check_moves(bool white2move); // Get a list of all check moves against you
    std::vector<std::string> possible_moves(bool white2move, bool show_moves, bool review_checks, bool check_castling=true); // Get a list of all possible moves
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
    std::vector<std::string> moves_in_vector(std::string target, std::vector<std::string> v); // Moves that contain the target
    std::vector<std::string> pawn_moves(int i, int j, bool is_white); // Pawn moves from the position i,j
    std::string basic_move_capture(int i_target, int j_target, bool is_white); // It tells which kind of move it is (normal, capture, block, invalid...)
    std::vector<std::string> rook_moves(int i, int j, bool is_white); // Rook moves from the position i,j
    std::vector<std::string> knight_moves(int i, int j, bool is_white); // Knight moves from the position i,j
    std::vector<std::string> bishop_moves(int i, int j, bool is_white); // Bishop moves from the position i,j
    std::vector<std::string> queen_moves(int i, int j, bool is_white); // Queen moves from the position i,j
    std::vector<std::string> king_moves(int i, int j, bool is_white, bool check_castling=true); // King moves from the position i,j
    std::string notate_square(int i, int j); // Notate a square
    std::string find_king(bool white2move); // Find the square of the king
    bool aim_castle(bool white2move, std::vector<std::string> key_squares); // Moves from black that prevent a castle (aiming at key squares)

    // Minimax function
    double minimax(Node &node, int depth, bool maximizingPlayer, Evaluation eval);
    double alphabeta(Node &node, int depth, double alpha, double beta, bool maximizingPlayer, Evaluation eval);

    // Display info
    void display_depth(int depth); // Display only the depth
    void display_score(int score, int depth, int nodes, int time, std::string best_line); // Display the score
    void display_nps(int nps); // Display the nodes per second 
    std::string get_best_line(int depth); // Get best line

    // Parameters
    Board board;
    bool debugging; // For debugging purposes
    bool use_alphabeta;

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
    Node searched_tree; // List of nodes with their score
    std::string bestmove; // Best immediate move
    std::vector<std::string> bestline; // Best line
    bool checkmated=false;
    int nodes_searched=0;
};

#endif