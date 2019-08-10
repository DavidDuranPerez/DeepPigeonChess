/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "engine.h"

// Default constructor
Engine::Engine(){
  this->debugging=false;
  this->use_alphabeta=true;
};

// Set a board
void Engine::set_board(std::string fen_str){
    this->board=Board(fen_str);
}

// Make a move in the board
void Engine::make_move(std::string move, bool print_board){
    this->board.move(move, print_board);
}

// Initialize parameters
void Engine::initialize_parameters(){
  this->searchmoves.clear();
  this->ponder=false;
  this->wtime=-1.0;
  this->btime=-1.0;
  this->winc=-1.0;
  this->binc=-1.0;
  this->movestogo=0;
  this->depth=0;
  this->nodes=0;
  this->mate=0;
  this->movetime=-1.0;
  this->infinite=false;
}

// Compute the best move
void Engine::compute(){
  // Create the fundamentals classes
  Mover mover=Mover();
  Evaluation eval=Evaluation();

  // See the possible moves
  std::vector<std::string> legal_moves = mover.possible_moves(this->board, this->board.get_turn(), this->debugging, true);

  // Initialize the nodes
  Node node;
  this->searched_tree=node;

  // Decide the depth
  int depth_max=this->depth;
  if(depth_max==0)
    depth_max=3;

  // Get original board so that we do not loss it
  Board board_original=this->board;

  // Start first with small depths and increase it. This repeats unnecessary loops though they are the faster ones!!!!!!
  int curr_depth=1;
  while(curr_depth<=depth_max){
    // Display info
    this->display_depth(curr_depth);

    // Create the element in the vector of bestline if it does not exist
    this->bestline={};
    while(this->bestline.size()<curr_depth)
      this->bestline.push_back(" ");

    // Begin counting time for that line
    this->nodes_searched=0; // Initialize the nodes at 0
    std::clock_t begin = clock();

    // Call the alphabeta/minimax function
    double best_score;
    if(this->use_alphabeta){
      double alpha=2*NEG_INF;
      double beta=2*POS_INF;
      best_score=this->alphabeta(this->searched_tree, curr_depth, alpha, beta, this->board.get_turn(), eval, mover);
    }
    else
      best_score=this->minimax(this->searched_tree, curr_depth, this->board.get_turn(), eval, mover);
    this->bestmove = this->searched_tree.move; // The outer most

    // End counting time
    std::clock_t end = clock();
    double elapsed_ms = double(end - begin);

    // Display info
    this->display_score((int)best_score, curr_depth, this->nodes_searched, elapsed_ms, this->get_best_line(curr_depth));
    this->display_nps((int)(this->nodes_searched*CLOCKS_PER_SEC/elapsed_ms));

    // Increase the depth
    curr_depth++;
  }

  // Get best move
  this->get_bestmove();

}

// Alphabeta function
double Engine::alphabeta(Node &node, int depth, double alpha, double beta, bool maximizingPlayer, Evaluation eval, Mover mover){
  // If end of depth
  if(depth==0)
    return node.score;

  // Get original board so that we do not loss it
  Board board_original=this->board;

  // Get all possible moves
  std::vector<std::string> legal_moves = mover.possible_moves(this->board, maximizingPlayer, this->debugging, true);

  // See if it is checkmate (terminal node)
  if(this->board.is_checkmated()) // If checkmate, we still haven't evaluate it
      return eval.eval_pos(this->board); // No need to make a move for the evaluation function since it is in checkmate

  // Maximize
  if(maximizingPlayer){
    double value=2*NEG_INF;
    bool tochange=false;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Initialize it
      tochange=false;

      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child;
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      double value_int=std::max(value, this->alphabeta(child, depth-1, alpha, beta, !maximizingPlayer, eval, mover));
      if(value_int>value){
        node.move=legal_moves[i];
        node.score=value_int;
        tochange=true;
      }
      value=value_int;
      
      // Save the node
      this->searched_tree.children.push_back(child);
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;

      // Beta cut-off
      alpha = std::max(alpha, value_int);
      if(alpha>=beta)
        break;
      else{
        if(tochange)
          this->bestline[this->bestline.size()-depth]=legal_moves[i];
      }
    }
    return value;
  }
  else { // Minimize
    double value=2*POS_INF;
    bool tochange=false;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Initialize it
      tochange=false;

      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child;
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      double value_int=std::min(value, this->alphabeta(child, depth-1, alpha, beta, !maximizingPlayer, eval, mover));
      if(value_int<value){
        node.move=legal_moves[i];
        node.score=value_int;
        tochange=true;
      }
      value=value_int;

      // Save the node
      this->searched_tree.children.push_back(child);
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;

      // Alpha cut-off
      beta = std::min(beta, value_int);
      if(alpha>=beta)
        break;
      else{
        if(tochange)
          this->bestline[this->bestline.size()-depth]=legal_moves[i];
      }
    }
    return value;
  }
}

// Get best line
std::string Engine::get_best_line(int depth){
  std::string best_line=this->bestline[0];
  for(size_t i=1; i<this->bestline.size(); i++){
    best_line+=" "+this->bestline[i];
  }
  return best_line;
}

// Display info
void Engine::display_depth(int depth){
  std::stringstream ss;
  ss << depth;
  std::cout << "info depth " << ss.str() << "\n";
}
void Engine::display_score(int score, int depth, int nodes, int time, std::string best_line){
  std::stringstream ss;
  ss << score;
  std::cout << "info score cp " << ss.str();
  std::stringstream ss2;
  ss2 << depth;
  std::cout << " depth " << ss2.str();
  std::stringstream ss3;
  ss3 << nodes;
  std::cout << " nodes " << ss3.str();
  std::stringstream ss4;
  ss4 << time;
  std::cout << " time " << ss4.str() << " pv " << best_line << "\n";
}
void Engine::display_nps(int nps){
  std::stringstream ss;
  ss << nps;
  std::cout << "info nps " << ss.str() << "\n"; 
}

// Minimax function
double Engine::minimax(Node &node, int depth, bool maximizingPlayer, Evaluation eval, Mover mover){
  // If end of depth
  if(depth==0)
    return node.score;

  // Get original board so that we do not loss it
  Board board_original=this->board;

  // Get all possible moves
  std::vector<std::string> legal_moves = mover.possible_moves(this->board, maximizingPlayer, this->debugging, true);

  // See if it is checkmate (terminal node)
  if(this->board.is_checkmated()) // If checkmate, we still haven't evaluate it
      return eval.eval_pos(this->board); // No need to make a move for the evaluation function since it is in checkmate

  // Maximize
  if(maximizingPlayer){
    double value=2*NEG_INF;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child;
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      double value_int=std::max(value, this->minimax(child, depth-1, !maximizingPlayer, eval, mover));
      if(value_int>value){
        node.move=legal_moves[i];
        node.score=value_int;
        this->bestline[this->bestline.size()-depth]=legal_moves[i]; // Wrong best line at second level!!!!
      }
      value=value_int;
      
      // Save the node
      this->searched_tree.children.push_back(child);
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;
    }
    return value;
  }
  else { // Minimize
    double value=2*POS_INF;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child;
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      double value_int=std::min(value, this->minimax(child, depth-1, !maximizingPlayer, eval, mover));
      if(value_int<value){
        node.move=legal_moves[i];
        node.score=value_int;
        this->bestline[this->bestline.size()-depth]=legal_moves[i]; // Wrong best line at second level!!!!
      }
      value=value_int;

      // Save the node
      this->searched_tree.children.push_back(child);
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;
    }
    return value;
  }
}