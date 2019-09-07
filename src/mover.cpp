/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "mover.h"

// Default constructor
Mover::Mover(){
};

// Find the square of the king
std::string Mover::find_king(bool white2move){
  // Piece to find
  char piece2find;
  if(white2move)
    piece2find='K';
  else
    piece2find = 'k';

  // Size of the board
  int board_size = this->board.get_size();

  // Make the double loop to search the piece
  for (int i = 2; i < board_size-2; i++)
  { // row
      for (int j = 2; j < board_size-2; j++)
      { // column
        if(this->board.get_piece(i,j)==piece2find)
          return this->notate_square(i, j);
      }
  }

  return "error";
}

// Is the character in the array
bool Mover::is_in_array(char ch, char (&arr)[6]){
  bool found=false;
  for(int i=0; i<sizeof(arr)/sizeof(arr[0]); i++){
    if(ch==arr[i])
      return true;
  }
  return found;
}

// Is the character in the array
bool Mover::is_in_array2(char ch, char (&arr)[12]){
  bool found=false;
  for(int i=0; i<sizeof(arr)/sizeof(arr[0]); i++){
    if(ch==arr[i])
      return true;
  }
  return found;
}

// Is a string in a vector
bool Mover::is_in_vector(std::string str_val, std::vector<std::string> str_vec){
  bool found=false;
  for(int i=0; i<str_vec.size(); i++){
    if(str_val==str_vec[i])
      return true;
  }
  return found;
}

// Set attacked squares
void Mover::set_attacked_squares(){
  // For profiling purposes
  std::clock_t begin;
  if(PROFILING)
    begin = clock();

  // Reset to false all squares
  this->board.reset_attacked_squares();

  // Size of the board
  int board_size = this->board.get_size();

  // Pieces to look for
  char pieces2look[12]={'P','R','N','B','Q','K','p','r','n','b','q','k'};

  // Set the squares
  for (int i = 2; i < board_size-2; i++)
  { // row
      for (int j = 2; j < board_size-2; j++)
      { // column
        // See if it is piece that we have to compute the moves
        char orig_piece=this->board.get_piece(i,j);
        bool look4moves=this->is_in_array2(orig_piece, pieces2look);

        // Switch between all pieces
        if(look4moves){
          std::vector<std::string> moves_piece;
          switch(orig_piece){
            case 'p':
              // Pawn moves
              moves_piece = this->pawn_moves(i, j, false, true);
              break;
            case 'P':
              // Pawn moves
              moves_piece = this->pawn_moves(i, j, true, true);
              break;
            case 'r':
              {
                // Rook moves
                moves_piece = this->rook_moves(i, j, false, true);
                // Pieces pinned by the rook
                int grad_x[8]={0, 0, -1, 1, -99, -99, -99, -99};
                int grad_y[8]={1, -1, 0, 0, -99, -99, -99, -99};
                this->RBQ_pinned(i, j, false, grad_x, grad_y, 4); 
              }
              break;
            case 'R':
              {
                // Rook moves
                moves_piece = this->rook_moves(i, j, true, true);
                // Pieces pinned by the rook
                int grad_x[8]={0, 0, -1, 1, -99, -99, -99, -99};
                int grad_y[8]={1, -1, 0, 0, -99, -99, -99, -99};
                this->RBQ_pinned(i, j, true, grad_x, grad_y, 4); 
              }
              break;
            case 'b':
              {
                // Bishop moves
                moves_piece = this->bishop_moves(i, j, false, true);
                // Pieces pinned by the bishop
                int grad_x[8]={1, -1, -1, 1, -99, -99, -99, -99};
                int grad_y[8]={1, -1, 1, -1, -99, -99, -99, -99};
                this->RBQ_pinned(i, j, false, grad_x, grad_y, 4);
              }
              break;
            case 'B':
              {
                // Bishop moves
                moves_piece = this->bishop_moves(i, j, true, true);
                // Pieces pinned by the bishop
                int grad_x[8]={1, -1, -1, 1, -99, -99, -99, -99};
                int grad_y[8]={1, -1, 1, -1, -99, -99, -99, -99};
                this->RBQ_pinned(i, j, true, grad_x, grad_y, 4); 
              }
              break;
            case 'q':
              {
                // Queen moves
                moves_piece = this->queen_moves(i, j, false, true);
                // Pieces pinned by the queen
                int grad_x[8]={0, 0, -1, 1, 1, -1, -1, 1};
                int grad_y[8]={1, -1, 0, 0, 1, -1, 1, -1};
                this->RBQ_pinned(i, j, false, grad_x, grad_y, 8); 
              }
              break;
            case 'Q':
              {
                // Queen moves
                moves_piece = this->queen_moves(i, j, true, true);
                // Pieces pinned by the queen
                int grad_x[8]={0, 0, -1, 1, 1, -1, -1, 1};
                int grad_y[8]={1, -1, 0, 0, 1, -1, 1, -1};
                this->RBQ_pinned(i, j, true, grad_x, grad_y, 8); 
              }
              break;
            case 'k':
              // King moves
              moves_piece = this->king_moves(i, j, false, false, true);
              break;
            case 'K':
              // King moves
              moves_piece = this->king_moves(i, j, true, false, true);
              break;
            case 'n':
              // Knight moves
              moves_piece = this->knight_moves(i, j, false, true);
              break;
            case 'N':
              // Knight moves
              moves_piece = this->knight_moves(i, j, true, true);
              break;
          }
        }
      }
  }

  // For profiling purposes
  if(PROFILING){
    std::clock_t end = clock();
    double elapsed_ms = double(end - begin);
    ANALYZED_FUNCTION_TIME+=elapsed_ms;
    ANALYZED_FUNCTION_COUNTER++;
  }
}

// Get a list of all possible moves
std::vector<std::string> Mover::possible_moves(Board &board, bool white2move, bool show_moves, bool check_castling/*=true*/){
  // Set the board position
  this->board=board;

  // Find the attacked squares for the current position
  this->set_attacked_squares();
  if(show_moves){
    this->board.print_attacked_by_white();
    this->board.print_attacked_by_black();
  }

  // Initialize the vector
  std::vector<std::string> moves={};
  std::vector<std::string> moves_definitive={};

  // Size of the board
  int board_size = this->board.get_size();

  // Get the number of checkers
  int num_checkers = this->board.get_num_checkers(white2move);

  // Pieces to look for
  char pieces2look[6];
  if(num_checkers<=1){
    // If we have one piece checking
    if(num_checkers==1){
      // Get the checker
      std::tuple<int, int> ij_checker=this->board.get_checker(white2move);
      int i_check=std::get<0>(ij_checker);
      int j_check=std::get<1>(ij_checker);

      // Set the capture mask
      this->board.set_capture_mask(white2move, i_check, j_check);

      // Set the push mask (only if not knight)
      if(this->board.get_piece(i_check, j_check)!='N' && this->board.get_piece(i_check, j_check)!='n'){
        // Set the push mask between the checker and the king
        this->board.set_push_mask(white2move, i_check, j_check);
      }
      else{
        // Empty push mask
        this->board.empty_push_mask(white2move);
      }
    }

    // Pieces to look
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
  }
  else{
    // For mulitple checkers, only king moves are possible
    if(white2move){ // White
      pieces2look[5]='K';
    }
    else{ // Black
      pieces2look[5]='k';
    }
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
              moves_piece = this->king_moves(i, j, white2move, check_castling);
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

  // We are in checkmate
  if(moves.size()==0){
    this->board.set_checkmate(true);
  }

  if(show_moves){
    sync_cout << "There are " << moves.size() << " possible moves:" << sync_endl;
    for(std::size_t i=0; i<moves.size(); ++i) 
      sync_cout << moves[i] << sync_endl; 
  }

  // Get back the board position
  board=this->board;

  // Return the list
  return moves;
};

// Notate a square
std::string Mover::notate_square(int i, int j){
  std::string orig_sq;
  orig_sq+=this->board.get_col(i,j);
  std::stringstream ss;
  ss << this->board.get_row(i,j);
  orig_sq+=ss.str();
  return orig_sq;
};

std::vector<std::string> Mover::pawn_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
  // Initialize the vector
  std::vector<std::string> moves_pawn={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Advance depends on the color
  if(is_white){
    // One square up
    if(this->board.get_piece(i+1, j)==' ' && this->board.is_valid(i+1, j) && this->board.get_push(is_white, i+1, j)){
      std::string target_sq=this->notate_square(i+1,j);
      std::string move=orig_sq+target_sq;
      // Make the promotion
      if(i+1==9){
        if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        {
          moves_pawn.push_back(move+'q'); // Queen promotion
          moves_pawn.push_back(move+'r'); // Rook promotion
          moves_pawn.push_back(move+'n'); // Knight promotion
          moves_pawn.push_back(move+'b'); // Bishop promotion
        }
      }
      else{
        if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
          moves_pawn.push_back(move);
      }
    }
    // Two squares up
    if(i==3 && this->board.get_piece(i+1, j)==' ' && this->board.get_piece(i+2, j)==' ' && this->board.is_valid(i+2, j) && this->board.get_push(is_white, i+2, j)){
      std::string target_sq=this->notate_square(i+2,j);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
    }
    // One square diagonal (capture) --> king capture should never happen unless checkmate
    if((this->board.get_piece(i+1, j+1)=='p' || this->board.get_piece(i+1, j+1)=='r' || this->board.get_piece(i+1, j+1)=='n' || this->board.get_piece(i+1, j+1)=='b' || this->board.get_piece(i+1, j+1)=='q' || this->board.get_piece(i+1, j+1)=='k') || record_attack && this->board.is_valid(i+1, j+1) && this->board.get_capture(is_white, i+1, j+1)){
      std::string target_sq=this->notate_square(i+1,j+1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i+1,j+1);

        // Checker
        if(this->board.get_piece(i+1, j+1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
        
    }
    // One square diagonal (en passant)
    if(this->board.get_piece(i+1, j+1)==' ' && this->board.is_enpassant(i+1, j+1) && this->board.is_valid(i+1, j+1) && (this->board.get_capture(is_white, i, j+1) || this->board.get_push(is_white, i+1, j+1))){
      std::string target_sq=this->notate_square(i+1,j+1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i+1,j+1);

        // Checker
        if(this->board.get_piece(i+1, j+1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
    }
    // One square diagonal (capture) --> king capture should never happen unless checkmate
    if((this->board.get_piece(i+1, j-1)=='p' || this->board.get_piece(i+1, j-1)=='r' || this->board.get_piece(i+1, j-1)=='n' || this->board.get_piece(i+1, j-1)=='b' || this->board.get_piece(i+1, j-1)=='q' || this->board.get_piece(i+1, j-1)=='k') || record_attack && this->board.is_valid(i+1, j-1) && this->board.get_capture(is_white, i+1, j-1)){
      std::string target_sq=this->notate_square(i+1,j-1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i+1,j-1);

        // Checker
        if(this->board.get_piece(i+1, j-1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
    }
    // One square diagonal (en passant)
    if(this->board.get_piece(i+1, j-1)==' ' && this->board.is_enpassant(i+1, j-1) && this->board.is_valid(i+1, j-1) && (this->board.get_capture(is_white, i, j-1) || this->board.get_push(is_white, i+1, j-1))){
      std::string target_sq=this->notate_square(i+1,j-1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i+1,j-1);

        // Checker
        if(this->board.get_piece(i+1, j-1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
    }
  }
  else{
    // One square up
    if(this->board.get_piece(i-1, j)==' ' && this->board.is_valid(i-1, j) && this->board.get_push(is_white, i-1, j)){
      std::string target_sq=this->notate_square(i-1,j);
      std::string move=orig_sq+target_sq;
      // Make the promotion
      if(i-1==2){
        if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j)))){
          moves_pawn.push_back(move+'q'); // Queen promotion
          moves_pawn.push_back(move+'r'); // Rook promotion
          moves_pawn.push_back(move+'n'); // Knight promotion
          moves_pawn.push_back(move+'b'); // Bishop promotion
        }
      }
      else{
        if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
          moves_pawn.push_back(move);
      }
    }
    // Two squares up
    if(i==8 && this->board.get_piece(i-1, j)==' ' && this->board.get_piece(i-2, j)==' ' && this->board.is_valid(i-2, j) && this->board.get_push(is_white, i-2, j)){
      std::string target_sq=this->notate_square(i-2,j);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
    }
    // One square diagonal (capture) --> king capture should never happen unless checkmate
    if((this->board.get_piece(i-1, j+1)=='P' || this->board.get_piece(i-1, j+1)=='R' || this->board.get_piece(i-1, j+1)=='N' || this->board.get_piece(i-1, j+1)=='B' || this->board.get_piece(i-1, j+1)=='Q' || this->board.get_piece(i-1, j+1)=='K') || record_attack && this->board.is_valid(i-1, j+1) && this->board.get_capture(is_white, i-1, j+1)){
      std::string target_sq=this->notate_square(i-1,j+1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i-1,j+1);

        // Checker
        if(this->board.get_piece(i-1, j+1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
    // One square diagonal (en passant)
    if(this->board.get_piece(i-1, j+1)==' ' && this->board.is_enpassant(i-1, j+1) && this->board.is_valid(i-1, j+1) && (this->board.get_capture(is_white, i, j+1) || this->board.get_push(is_white, i-1, j+1))){
      std::string target_sq=this->notate_square(i-1,j+1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i-1,j+1);

        // Checker
        if(this->board.get_piece(i-1, j+1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
    // One square diagonal (capture) --> king capture should never happen unless checkmate
    if((this->board.get_piece(i-1, j-1)=='P' || this->board.get_piece(i-1, j-1)=='R' || this->board.get_piece(i-1, j-1)=='N' || this->board.get_piece(i-1, j-1)=='B' || this->board.get_piece(i-1, j-1)=='Q' || this->board.get_piece(i-1, j-1)=='K') || record_attack && this->board.is_valid(i-1, j-1) && this->board.get_capture(is_white, i-1, j-1)){
      std::string target_sq=this->notate_square(i-1,j-1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i-1,j-1);

        // Checker
        if(this->board.get_piece(i-1, j-1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
    // One square diagonal (en passant)
    if(this->board.get_piece(i-1, j-1)==' ' && this->board.is_enpassant(i-1, j-1) && this->board.is_valid(i-1, j-1) && (this->board.get_capture(is_white, i, j-1) || this->board.get_push(is_white, i-1, j-1))){
      std::string target_sq=this->notate_square(i-1,j-1);
      std::string move=orig_sq+target_sq;
      if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
        moves_pawn.push_back(move);
      // Record an attacked squared
      if(record_attack){
        this->board.set_white_attack(i-1,j-1);

        // Checker
        if(this->board.get_piece(i-1, j-1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  return moves_pawn;
}

std::string Mover::basic_move_capture(int i_target, int j_target, bool is_white, bool is_king/*=false*/){
    // We do not event look if it is not valid
    if(!this->board.is_valid(i_target, j_target))
      return "invalid";

    // No capture
    if(this->board.get_piece(i_target, j_target)==' ' && this->board.is_valid(i_target, j_target) && (is_king || this->board.get_push(is_white, i_target, j_target))){
      return "move";
    }

    // Capture --> king capture should never happen unless checkmate
    if(is_white){
      if((this->board.get_piece(i_target, j_target)=='p' || this->board.get_piece(i_target, j_target)=='r' || this->board.get_piece(i_target, j_target)=='n' || this->board.get_piece(i_target, j_target)=='b' || this->board.get_piece(i_target, j_target)=='q' || this->board.get_piece(i_target, j_target)=='k') && this->board.is_valid(i_target, j_target) && (is_king || this->board.get_capture(is_white, i_target, j_target))){
        return "capture";
      }
    }else{
      if((this->board.get_piece(i_target, j_target)=='P' || this->board.get_piece(i_target, j_target)=='R' || this->board.get_piece(i_target, j_target)=='N' || this->board.get_piece(i_target, j_target)=='B' || this->board.get_piece(i_target, j_target)=='Q' || this->board.get_piece(i_target, j_target)=='K') && this->board.is_valid(i_target, j_target) && (is_king || this->board.get_capture(is_white, i_target, j_target))){
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

void Mover::RBQ_pinned(int i, int j, bool is_white, int (&grad_x)[8], int (&grad_y)[8], int N_grad){
  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Loop all the gradients
  for(int k=0; k<N_grad; k++){
    // Start the gradient
    int x=j+grad_x[k];
    int y=i+grad_y[k];

    // X-ray for pinned pieces
    std::vector<std::string> x_ray={};
    int i_pinned=-1; // Possible pinned position
    int j_pinned=-1; // Possible pinned position
    int n_pinned=0;
    x_ray.push_back(orig_sq); // Add the attacking piece to the x_ray too

    // Ends when x or y are outside the limits
    while((x>=2 || x<=9) && (y>=2 || y<=9)){
      // Target square
      std::string target_sq=this->notate_square(y, x);
      std::string move=orig_sq+target_sq;

      // Decision of what to do with the target square
      std::string decision=this->basic_move_capture(y, x, is_white);
      // Differente cases
      if(decision=="block" || decision=="invalid")
        break;
      else if(decision=="capture"){
        // Record an attacked squared
        if(is_white){
          // Checker
          if(this->board.get_piece(y, x)=='k'){
            // Add the pinned piece
            if(n_pinned==1){
              // Mark the square as pinned
              this->board.set_pinned_piece(i_pinned, j_pinned);

              // Add the possible moves for the pinned piece
              this->board.set_x_ray_pinned(i_pinned, j_pinned, x_ray);
            }
          }
          else{
            // Possible pinned piece (last one)
            i_pinned=y;
            j_pinned=x;
            n_pinned++;
          }
        }
        else{
          // Checker
          if(this->board.get_piece(y, x)=='K'){
            // Add the pinned piece
            if(n_pinned==1){
              // Mark the square as pinned
              this->board.set_pinned_piece(i_pinned, j_pinned);

              // Add the possible moves for the pinned piece
              this->board.set_x_ray_pinned(i_pinned, j_pinned, x_ray);
            }
          }
          else{
            // Possible pinned piece (last one)
            i_pinned=y;
            j_pinned=x;
            n_pinned++;
          }
        }
        if((this->board.get_piece(y, x)=='k' && is_white) || (this->board.get_piece(y, x)=='K' && !is_white))
          break;
      }
      else if(decision=="move"){
        // Add to x-ray
        x_ray.push_back(target_sq);
      }
    
      // Increase the gradient
      x+=grad_x[k];
      y+=grad_y[k];
    }
  }
}

std::vector<std::string> Mover::RBQ_moves(int i, int j, bool is_white, int (&grad_x)[8], int (&grad_y)[8], int N_grad, bool record_attack/*=false*/){
  // Initialize the vector
  std::vector<std::string> moves={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Loop all the gradients
  for(int k=0; k<N_grad; k++){
    // Start the gradient
    int x=j+grad_x[k];
    int y=i+grad_y[k];

    // Ends when x or y are outside the limits
    while((x>=2 || x<=9) && (y>=2 || y<=9)){
      // Target square
      std::string target_sq=this->notate_square(y, x);
      std::string move=orig_sq+target_sq;

      // Decision of what to do with the target square
      std::string decision=this->basic_move_capture(y, x, is_white);
      // Differente cases
      if(decision=="block" || decision=="invalid")
        break;
      else if(decision=="capture"){
        // Add the move
        if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
          moves.push_back(move);

        // Record an attacked squared
        if(record_attack){
          if(is_white){
            this->board.set_white_attack(y, x);
              
            // Checker
            if(this->board.get_piece(y, x)=='k'){
              this->board.set_checker_white(i,j);
            }
          }
          else{
            this->board.set_black_attack(y, x);
            
            // Checker
            if(this->board.get_piece(y, x)=='K'){
              this->board.set_checker_black(i,j);
            }
          }
        }
        if(!(record_attack && ((this->board.get_piece(y, x)=='k' && is_white) || (this->board.get_piece(y, x)=='K' && !is_white))))
          break;
      }
      else if(decision=="move"){
        // Add the move
        if(!(this->board.is_pinned_piece(i, j) && !this->is_in_vector(target_sq, this->board.get_x_ray_pinned(i, j))))
          moves.push_back(move);

        // Record an attacked squared
        if(record_attack){
          if(is_white)
            this->board.set_white_attack(y, x);
          else
            this->board.set_black_attack(y, x);
        }
      }
      
      // Increase the gradient
      x+=grad_x[k];
      y+=grad_y[k];
    }
  }

  return moves;
}


std::vector<std::string> Mover::rook_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
  // Gradients for the 4 directions
  int grad_x[8]={0, 0, -1, 1, -99, -99, -99, -99};
  int grad_y[8]={1, -1, 0, 0, -99, -99, -99, -99};

  // Get the moves
  return this->RBQ_moves(i, j, is_white, grad_x, grad_y, 4, record_attack);
}

std::vector<std::string> Mover::bishop_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
  // Gradients for the 4 directions
  int grad_x[8]={-1, 1, -1, 1, -99, -99, -99, -99};
  int grad_y[8]={-1, 1, 1, -1, -99, -99, -99, -99};

  // Get the moves
  return this->RBQ_moves(i, j, is_white, grad_x, grad_y, 4, record_attack);
}

std::vector<std::string> Mover::queen_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
  // Gradients for the 8 directions
  int grad_x[8]={0, 0, -1, 1, -1, 1, -1, 1};
  int grad_y[8]={1, -1, 0, 0, -1, 1, 1, -1};

  // Get the moves
  return this->RBQ_moves(i, j, is_white, grad_x, grad_y, 8, record_attack);
}

std::vector<std::string> Mover::king_moves(int i, int j, bool is_white, bool check_castling/*=true*/, bool record_attack/*=false*/){
  // Initialize the vector
  std::vector<std::string> moves_king={};

  // Gradients for the 8 directions
  int grad_x[8]={0, 0, -1, 1, -1, 1, -1, 1};
  int grad_y[8]={1, -1, 0, 0, -1, 1, 1, -1};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Loop all the gradients
  for(int k=0; k<sizeof(grad_x)/sizeof(grad_x[0]); k++){
    // Start the gradient
    int x=j+grad_x[k];
    int y=i+grad_y[k];

    // Target square
    std::string target_sq=this->notate_square(y, x);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(y, x, is_white, true);
    if(decision=="move" || decision=="capture")
    {
      // Avoid entering in an attacked square
      if((is_white && !this->board.is_square_attacked_by_black(y, x)) || (!is_white && !this->board.is_square_attacked_by_white(y, x))) 
        moves_king.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(y, x);
        else
          this->board.set_black_attack(y, x);
      }
    }
  }

  // Castling can be done if: allowed (king and rook not moved before), empty squares between king and rook, and no enemy piece aiming at key squares
  if(check_castling){
    // White castles
    if(is_white){
      // Short castle
      if(this->board.is_castle_allowed('K') && this->board.get_piece(2, 7)==' ' && this->board.get_piece(2, 8)==' ' && (!this->board.is_square_attacked_by_black(2, 6) && !this->board.is_square_attacked_by_black(2, 7) && !this->board.is_square_attacked_by_black(2, 8)))
        moves_king.push_back("e1g1");

      // Long castle
      if(this->board.is_castle_allowed('Q') && this->board.get_piece(2, 3)==' ' && this->board.get_piece(2, 4)==' ' && this->board.get_piece(2, 5)==' ' && (!this->board.is_square_attacked_by_black(2, 6) && !this->board.is_square_attacked_by_black(2, 5) && !this->board.is_square_attacked_by_black(2, 4)))
        moves_king.push_back("e1c1");
    }
    else{ // Black castles
      // Short castle
      if(this->board.is_castle_allowed('k') && this->board.get_piece(9, 7)==' ' && this->board.get_piece(9, 8)==' ' && (!this->board.is_square_attacked_by_white(9, 6) && !this->board.is_square_attacked_by_white(9, 7) && !this->board.is_square_attacked_by_white(9, 8)))
        moves_king.push_back("e8g8");

      // Long castle
      if(this->board.is_castle_allowed('q') && this->board.get_piece(9, 3)==' ' && this->board.get_piece(9, 4)==' ' && this->board.get_piece(9, 5)==' ' && (!this->board.is_square_attacked_by_white(9, 6) && !this->board.is_square_attacked_by_white(9, 5) && !this->board.is_square_attacked_by_white(9, 4)))
        moves_king.push_back("e8c8");
    }
  }

  return moves_king;
}

std::vector<std::string> Mover::knight_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
  // Initialize the vector
  std::vector<std::string> moves_knight={};

  // Gradients for the 8 directions
  int grad_x[8]={2, 1, -1, -2, -2, -1, 1, 2};
  int grad_y[8]={1, 2, 2, 1, -1, -2, -2, -1};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Loop all the gradients
  for(int k=0; k<sizeof(grad_x)/sizeof(grad_x[0]); k++){
    // Start the gradient
    int x=j+grad_x[k];
    int y=i+grad_y[k];

    // Target square
    std::string target_sq=this->notate_square(y, x);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision;
    // Knights can never move along a pin ray
    if(this->board.is_pinned_piece(i, j))
      decision = "invalid";
    else
      decision=this->basic_move_capture(y, x, is_white);
    // Differente cases
    if(decision=="move" || decision=="capture"){
      // Add the move
      moves_knight.push_back(move);

      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(y, x);
            
          // Checker
          if(decision=="capture" && this->board.get_piece(y, x)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(y, x);
          
          // Checker
          if(decision=="capture" && this->board.get_piece(y, x)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
    }
  }

  return moves_knight;
}