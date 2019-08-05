/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "engine.h"

// Default constructor
Engine::Engine(){
  this->debugging=true;
};

// Set a board
void Engine::set_board(std::string fen_str){
    this->board=Board(fen_str);
}

// Make a move in the board
void Engine::make_move(std::string move){
    this->board.move(move);
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
  // See the possible moves
  std::vector<std::string> legal_moves = this->possible_moves();
}

// Is the character in the array
bool Engine::is_in_array(char ch, char (&arr)[6]){
  bool found=false;
  for(int i=0; i<sizeof(arr)/sizeof(arr[0]); i++){
    if(ch==arr[i])
      return true;
  }
  return found;
}

// Get a list of all possible moves
std::vector<std::string> Engine::possible_moves(){
  // Initialize the vector
  std::vector<std::string> moves={};

  // Size of the board
  int board_size = this->board.get_size();
  // White to move?
  bool white2move = this->board.get_turn();

  // Pieces to look for
  char pieces2look[6];
  if(white2move){ // White
    pieces2look[0]='P';
    pieces2look[1]='R';
    pieces2look[2]='N';
    pieces2look[3]='B';
    pieces2look[4]='Q';
    pieces2look[5]='K';
  }
  else{ // Black
    pieces2look[0]='p';
    pieces2look[1]='r';
    pieces2look[2]='n';
    pieces2look[3]='b';
    pieces2look[4]='q';
    pieces2look[5]='k';
  }


  // Set the squares
  for (int i = 2; i < board_size-2; i++)
  { // row
      for (int j = 2; j < board_size-2; j++)
      { // column
        // See if it is piece that we have to compute the moves
        char orig_piece=this->board.get_piece(i,j);
        bool look4moves=this->is_in_array(orig_piece, pieces2look);

        // Switch between all pieces
        if(look4moves){
          std::vector<std::string> moves_piece;
          switch(orig_piece){
            case 'p':
            case 'P':
              // Pawn moves
              moves_piece = this->pawn_moves(i, j, white2move);
              break;
            case 'r':
            case 'R':
              // Rook moves
              moves_piece = this->rook_moves(i, j, white2move);
              break;
            case 'b':
            case 'B':
              // Bishop moves
              moves_piece = this->bishop_moves(i, j, white2move);
              break;
            case 'q':
            case 'Q':
              // Queen moves
              moves_piece = this->queen_moves(i, j, white2move);
              break;
            case 'k':
            case 'K':
              // King moves
              moves_piece = this->king_moves(i, j, white2move);
              break;
            case 'n':
            case 'N':
              // Knight moves
              moves_piece = this->knight_moves(i, j, white2move);
              break;
          }

          // Append this vector to the big one
          moves.insert(moves.end(), moves_piece.begin(), moves_piece.end());
        }
      }
  }

  if(this->debugging){
    std::cout << "There are " << moves.size() << " possible moves:" << "\n";
    for(std::size_t i=0; i<moves.size(); ++i) 
      std::cout << moves[i] << '\n'; 
  }

  // Return the list
  return moves;
};

// Notate a square
std::string Engine::notate_square(int i, int j){
  std::string orig_sq;
  orig_sq+=this->board.get_col(i,j);
  std::stringstream ss;
  ss << this->board.get_row(i,j);
  orig_sq+=ss.str();
  return orig_sq;
};

std::vector<std::string> Engine::pawn_moves(int i, int j, bool is_white){
  // En-passant is not yet implemented!!!!!

  // Initialize the vector
  std::vector<std::string> moves_pawn={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Advance depends on the color
  if(is_white){
    // One square up
    if(this->board.get_piece(i+1, j)==' ' && this->board.is_valid(i+1, j)){
      std::string target_sq=this->notate_square(i+1,j);
      std::string move=orig_sq+target_sq;
      // Make the promotion
      if(i+1==9){
        moves_pawn.push_back(move+'q'); // Queen promotion
        moves_pawn.push_back(move+'r'); // Rook promotion
        moves_pawn.push_back(move+'n'); // Knight promotion
        moves_pawn.push_back(move+'b'); // Bishop promotion
      }
      else{
        moves_pawn.push_back(move);
      }
    }
    // Two squares up
    if(i==3 && this->board.get_piece(i+1, j)==' ' && this->board.get_piece(i+2, j)==' ' && this->board.is_valid(i+2, j)){
      std::string target_sq=this->notate_square(i+2,j);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
    // One square diagonal (capture)
    if((this->board.get_piece(i+1, j+1)=='p' || this->board.get_piece(i+1, j+1)=='r' || this->board.get_piece(i+1, j+1)=='n' || this->board.get_piece(i+1, j+1)=='b' || this->board.get_piece(i+1, j+1)=='q') && this->board.is_valid(i+1, j+1)){
      std::string target_sq=this->notate_square(i+1,j+1);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
    // One square diagonal (capture)
    if((this->board.get_piece(i+1, j-1)=='p' || this->board.get_piece(i+1, j-1)=='r' || this->board.get_piece(i+1, j-1)=='n' || this->board.get_piece(i+1, j-1)=='b' || this->board.get_piece(i+1, j-1)=='q') && this->board.is_valid(i+1, j-1)){
      std::string target_sq=this->notate_square(i+1,j-1);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
  }
  else{
    // One square up
    if(this->board.get_piece(i-1, j)==' ' && this->board.is_valid(i-1, j)){
      std::string target_sq=this->notate_square(i-1,j);
      std::string move=orig_sq+target_sq;
      // Make the promotion
      if(i-1==2){
        moves_pawn.push_back(move+'q'); // Queen promotion
        moves_pawn.push_back(move+'r'); // Rook promotion
        moves_pawn.push_back(move+'n'); // Knight promotion
        moves_pawn.push_back(move+'b'); // Bishop promotion
      }
      else{
        moves_pawn.push_back(move);
      }
    }
    // Two squares up
    if(i==8 && this->board.get_piece(i-1, j)==' ' && this->board.get_piece(i-2, j)==' ' && this->board.is_valid(i-2, j)){
      std::string target_sq=this->notate_square(i-2,j);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
    // One square diagonal (capture)
    if((this->board.get_piece(i-1, j+1)=='P' || this->board.get_piece(i-1, j+1)=='R' || this->board.get_piece(i-1, j+1)=='N' || this->board.get_piece(i-1, j+1)=='B' || this->board.get_piece(i-1, j+1)=='Q') && this->board.is_valid(i-1, j+1)){
      std::string target_sq=this->notate_square(i+1,j+1);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
    // One square diagonal (capture)
    if((this->board.get_piece(i-1, j-1)=='P' || this->board.get_piece(i-1, j-1)=='R' || this->board.get_piece(i-1, j-1)=='N' || this->board.get_piece(i-1, j-1)=='B' || this->board.get_piece(i-1, j-1)=='Q') && this->board.is_valid(i-1, j-1)){
      std::string target_sq=this->notate_square(i-1,j-1);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
  }

  return moves_pawn;
}

std::string Engine::basic_move_capture(int i_target, int j_target, bool is_white){
    // We do not event look if it is not valid
    if(!this->board.is_valid(i_target, j_target))
      return "invalid";

    // No capture
    if(this->board.get_piece(i_target, j_target)==' ' && this->board.is_valid(i_target, j_target)){
      return "move";
    }

    // Capture
    if(is_white){
      if((this->board.get_piece(i_target, j_target)=='p' || this->board.get_piece(i_target, j_target)=='r' || this->board.get_piece(i_target, j_target)=='n' || this->board.get_piece(i_target, j_target)=='b' || this->board.get_piece(i_target, j_target)=='q') && this->board.is_valid(i_target, j_target)){
        return "capture";
      }
    }else{
      if((this->board.get_piece(i_target, j_target)=='P' || this->board.get_piece(i_target, j_target)=='R' || this->board.get_piece(i_target, j_target)=='N' || this->board.get_piece(i_target, j_target)=='B' || this->board.get_piece(i_target, j_target)=='Q') && this->board.is_valid(i_target, j_target)){
        return "capture";
      }
    }

    // Block
    if(is_white){
      if((this->board.get_piece(i_target, j_target)=='P' || this->board.get_piece(i_target, j_target)=='R' || this->board.get_piece(i_target, j_target)=='N' || this->board.get_piece(i_target, j_target)=='B' || this->board.get_piece(i_target, j_target)=='Q' || this->board.get_piece(i_target, j_target)=='K') && this->board.is_valid(i_target, j_target)){
        return "block";
      }
    }else{
      if((this->board.get_piece(i_target, j_target)=='p' || this->board.get_piece(i_target, j_target)=='r' || this->board.get_piece(i_target, j_target)=='n' || this->board.get_piece(i_target, j_target)=='b' || this->board.get_piece(i_target, j_target)=='q' || this->board.get_piece(i_target, j_target)=='k') && this->board.is_valid(i_target, j_target)){
        return "block";
      }        
    }

    // Default
    return "default";
}

std::vector<std::string> Engine::rook_moves(int i, int j, bool is_white){
  // Initialize the vector
  std::vector<std::string> moves_rook={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Down movements
  for(int ind=i-1; ind>=2; ind--){
    // Target square
    std::string target_sq=this->notate_square(ind,j);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_rook.push_back(move);
  }

  // Up movements
  for(int ind=i+1; ind<=9; ind++){
    // Target square
    std::string target_sq=this->notate_square(ind,j);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_rook.push_back(move);
  }

  // Left movements
  for(int ind=j-1; ind>=2; ind--){
    // Target square
    std::string target_sq=this->notate_square(i,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_rook.push_back(move);
  }

  // Right movements
  for(int ind=j+1; ind<=9; ind++){
    // Target square
    std::string target_sq=this->notate_square(i,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_rook.push_back(move);
  }

  return moves_rook;
}

std::vector<std::string> Engine::bishop_moves(int i, int j, bool is_white){
  // Initialize the vector
  std::vector<std::string> moves_bishop={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Left-down movements
  for(int ind=i-1; ind>=2; ind--){
    // Difference of squares
    int diff_squares = i-ind;
    // Target square
    std::string target_sq=this->notate_square(ind, j-diff_squares);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j-diff_squares, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_bishop.push_back(move);
  }

  // Right-up movements
  for(int ind=i+1; ind<=9; ind++){
    // Difference of squares
    int diff_squares = ind-i;
    // Target square
    std::string target_sq=this->notate_square(ind, j+diff_squares);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j+diff_squares, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_bishop.push_back(move);
  }

  // Left-up movements
  for(int ind=j-1; ind>=2; ind--){
    // Difference of squares
    int diff_squares = j-ind;
    // Target square
    std::string target_sq=this->notate_square(i+diff_squares,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i+diff_squares, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_bishop.push_back(move);
  }

  // Right-down movements
  for(int ind=j+1; ind<=9; ind++){
    // Difference of squares
    int diff_squares = ind-j;
    // Target square
    std::string target_sq=this->notate_square(i-diff_squares,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i-diff_squares, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_bishop.push_back(move);
  }

  return moves_bishop;
}

std::vector<std::string> Engine::queen_moves(int i, int j, bool is_white){
  // Initialize the vector
  std::vector<std::string> moves_queen={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Down movements
  for(int ind=i-1; ind>=2; ind--){
    // Target square
    std::string target_sq=this->notate_square(ind,j);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Up movements
  for(int ind=i+1; ind<=9; ind++){
    // Target square
    std::string target_sq=this->notate_square(ind,j);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Left movements
  for(int ind=j-1; ind>=2; ind--){
    // Target square
    std::string target_sq=this->notate_square(i,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Right movements
  for(int ind=j+1; ind<=9; ind++){
    // Target square
    std::string target_sq=this->notate_square(i,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Left-down movements
  for(int ind=i-1; ind>=2; ind--){
    // Difference of squares
    int diff_squares = i-ind;
    // Target square
    std::string target_sq=this->notate_square(ind, j-diff_squares);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j-diff_squares, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Right-up movements
  for(int ind=i+1; ind<=9; ind++){
    // Difference of squares
    int diff_squares = ind-i;
    // Target square
    std::string target_sq=this->notate_square(ind, j+diff_squares);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j+diff_squares, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Left-up movements
  for(int ind=j-1; ind>=2; ind--){
    // Difference of squares
    int diff_squares = j-ind;
    // Target square
    std::string target_sq=this->notate_square(i+diff_squares,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i+diff_squares, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  // Right-down movements
  for(int ind=j+1; ind<=9; ind++){
    // Difference of squares
    int diff_squares = ind-j;
    // Target square
    std::string target_sq=this->notate_square(i-diff_squares,ind);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(i-diff_squares, ind, is_white);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="move" || decision=="capture")
      moves_queen.push_back(move);
  }

  return moves_queen;
}

std::vector<std::string> Engine::king_moves(int i, int j, bool is_white){
  // Castling not yet implemented!!!!!!!!!!!

  // Initialize the vector
  std::vector<std::string> moves_king={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Down movement
  // Target square
  std::string target_sq=this->notate_square(i-1,j);
  std::string move=orig_sq+target_sq;
  // Decision of what to do with the target square
  std::string decision=this->basic_move_capture(i-1, j, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Up movement
  // Target square
  target_sq=this->notate_square(i+1,j);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1, j, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Left movements
  // Target square
  target_sq=this->notate_square(i,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i, j-1, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Right movements
  // Target square
  target_sq=this->notate_square(i,j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i, j+1, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Left-down movements
  // Target square
  target_sq=this->notate_square(i-1, j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1, j-1, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Right-up movements
  // Target square
  target_sq=this->notate_square(i+1, j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1, j+1, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Left-up movements
  // Target square
  target_sq=this->notate_square(i+1,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1, j-1, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  // Right-down movements
  // Target square
  target_sq=this->notate_square(i-1,j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1,j+1, is_white);
  if(decision=="move" || decision=="capture")
    moves_king.push_back(move);

  return moves_king;
}

std::vector<std::string> Engine::knight_moves(int i, int j, bool is_white){
  // Initialize the vector
  std::vector<std::string> moves_knight={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Vertical up-right L
  // Target square
  std::string target_sq=this->notate_square(i+2,j+1);
  std::string move=orig_sq+target_sq;
  // Decision of what to do with the target square
  std::string decision=this->basic_move_capture(i+2,j+1, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Vertical up-left L
  // Target square
  target_sq=this->notate_square(i+2,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+2,j-1, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Horizontal up-right L
  // Target square
  target_sq=this->notate_square(i+1,j+2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1,j+2, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Horizontal up-left L
  // Target square
  target_sq=this->notate_square(i+1,j-2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1,j-2, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Vertical down-right L
  // Target square
  target_sq=this->notate_square(i-2,j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-2,j+1, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Vertical down-left L
  // Target square
  target_sq=this->notate_square(i-2,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-2,j-1, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Horizontal down-right L
  // Target square
  target_sq=this->notate_square(i-1,j+2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1,j+2, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  // Horizontal down-left L
  // Target square
  target_sq=this->notate_square(i-1,j-2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1,j-2, is_white);
  if(decision=="move" || decision=="capture")
    moves_knight.push_back(move);

  return moves_knight;
}