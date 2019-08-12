/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "engine.h"

// Default constructor for node
Node::Node(){
  this->move=""; // String representing the move
  this->bestmove=""; // String representing the best move of the children
  this->score=0.0; // Score
  this->depth=1; // Number of halfmoves
  this->children={}; // Children
}

// Default constructor for engine
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
//void Engine::compute(std::future<void> futureObj){
void Engine::compute(std::atomic<bool> &stop_flag){
  // Create the fundamentals classes
  Mover mover=Mover();
  Evaluation eval=Evaluation();

  // See the possible moves
  std::vector<std::string> legal_moves = mover.possible_moves(this->board, this->board.get_turn(), this->debugging, true);

  // Decide the depth
  int depth_max=this->depth;
  if(this->infinite || depth_max==0)
    depth_max=1000; // Go infinite

  // Get original board so that we do not loss it
  Board board_original=this->board;

  // Start first with small depths and increase it. This repeats unnecessary loops though they are the faster ones!!!!!!
  int curr_depth=1;
  while(curr_depth<=depth_max && !stop_flag){ // It can be stopped from outside
    // Initialize the nodes
    sync_cout << stop_flag << sync_endl;
    Node node=Node();
    this->searched_tree=node;

    // Display info
    //this->display_depth(curr_depth);

    // Begin counting time for that line
    this->nodes_searched=0; // Initialize the nodes at 0
    std::clock_t begin = clock();

    // Call the alphabeta/minimax function
    int best_score;
    if(this->use_alphabeta){
      int alpha=2*NEG_INF;
      int beta=2*POS_INF;
      best_score=this->alphabeta(this->searched_tree, curr_depth, alpha, beta, this->board.get_turn(), eval, mover);
    }
    else
      best_score=this->minimax(this->searched_tree, curr_depth, this->board.get_turn(), eval, mover);
    
    // Get best move and best line
    this->bestmove = this->searched_tree.bestmove; // The outer most
    this->bestline=this->get_pv(curr_depth);

    // End counting time
    std::clock_t end = clock();
    double elapsed_ms = double(end - begin);

    // Display info
    if(elapsed_ms<1)
      elapsed_ms=1;
    this->display_score(curr_depth, best_score, this->nodes_searched, (int)(this->nodes_searched*CLOCKS_PER_SEC/elapsed_ms), elapsed_ms, this->bestline);
    //this->display_nps((int)(this->nodes_searched*CLOCKS_PER_SEC/elapsed_ms));

    // Increase the depth
    curr_depth++;
  }

  // Get best move
  this->get_bestmove();

}

// Alphabeta function
int Engine::alphabeta(Node &node, int depth, int alpha, int beta, bool maximizingPlayer, Evaluation eval, Mover mover){
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
    int value=2*NEG_INF;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child=Node();
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      int value_int=std::max(value, this->alphabeta(child, depth-1, alpha, beta, !maximizingPlayer, eval, mover));
      if(value_int>value){
        node.bestmove=legal_moves[i];
        node.score=value_int;
      }
      node.children.push_back(child); // Append the child to the parent
      value=value_int;
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;

      // Beta cut-off
      alpha = std::max(alpha, value_int);
      if(alpha>=beta)
        break;
    }
    return value;
  }
  else { // Minimize
    int value=2*POS_INF;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child=Node();
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      int value_int=std::min(value, this->alphabeta(child, depth-1, alpha, beta, !maximizingPlayer, eval, mover));
      if(value_int<value){
        node.bestmove=legal_moves[i];
        node.score=value_int;
      }
      node.children.push_back(child); // Append the child to the parent
      value=value_int;
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;

      // Alpha cut-off
      beta = std::min(beta, value_int);
      if(alpha>=beta)
        break;
    }
    return value;
  }
}

// Get the best lien
std::string Engine::get_pv(int depth){
  // The string representing the pv
  std::string pv="";

  // Node to look into
  Node node_selected=this->searched_tree;

  // Move to search
  std::string move2search=node_selected.bestmove;
  pv = move2search;

  // Define the current depth
  int curr_depth=depth;

  // Iterate the depths
  while(depth>0){
    // Search the move
    for(size_t i=0; i<node_selected.children.size(); i++)
    {
      Node node_int=node_selected.children[i];
      if(move2search==node_int.move){
        node_selected=node_int; // Select the child now
        if(depth>1){
          move2search=node_selected.bestmove; // New move to search
          if(move2search=="") // Due to a checkmate
            depth=1; // Force ending of the line
          else
            pv+=" "+move2search;
        }
        depth--; // Decrease the depth
        break;
      }
    }
  }

  return pv;
}

// Display info
void Engine::display_depth(int depth){
  std::stringstream ss;
  ss << depth;
  std::cout << "info depth " << ss.str() << "\n";
}
void Engine::display_score(int depth, int score, int nodes, int nps, int time, std::string best_line){
  // Convert to strings
  std::stringstream ss5;
  ss5 << depth;
  std::stringstream ss;
  ss << score;
  std::stringstream ss3;
  ss3 << nodes;
  std::stringstream ss6;
  ss6 << nps;
  std::stringstream ss4;
  ss4 << time;

  // Message
  sync_cout << "info depth " << ss5.str() << " score cp " << ss.str() << " nodes " << ss3.str() << " nps " << ss6.str() << " time " << ss4.str() << " pv " << best_line << sync_endl;
}
void Engine::display_nps(int nps){
  std::stringstream ss;
  ss << nps;
  std::cout << "info nps " << ss.str() << "\n"; 
}

// Minimax function
int Engine::minimax(Node &node, int depth, bool maximizingPlayer, Evaluation eval, Mover mover){
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
    int value=2*NEG_INF;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child=Node();
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      int value_int=std::max(value, this->minimax(child, depth-1, !maximizingPlayer, eval, mover));
      if(value_int>value){
        node.move=legal_moves[i];
        node.score=value_int;
      }
      value=value_int;
      node.children.push_back(child); // Append the child to the parent
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;
    }
    return value;
  }
  else { // Minimize
    int value=2*POS_INF;
    for(size_t i=0; i<legal_moves.size(); i++){
      // Make the move
      this->make_move(legal_moves[i], false);
      
      // Evaluate the position
      Node child=Node();
      child.move=legal_moves[i];
      child.score=eval.eval_pos(this->board);

      // Compute the maximum
      int value_int=std::min(value, this->minimax(child, depth-1, !maximizingPlayer, eval, mover));
      if(value_int<value){
        node.move=legal_moves[i];
        node.score=value_int;
      }
      node.children.push_back(child); // Append the child to the parent
      value=value_int;
    
      // Get back to the original board
      this->nodes_searched++;
      this->board=board_original;
    }
    return value;
  }
}