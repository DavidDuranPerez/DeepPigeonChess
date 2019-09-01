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
              // Rook moves
              moves_piece = this->rook_moves(i, j, false, true);
              break;
            case 'R':
              // Rook moves
              moves_piece = this->rook_moves(i, j, true, true);
              break;
            case 'b':
              // Bishop moves
              moves_piece = this->bishop_moves(i, j, false, true);
              break;
            case 'B':
              // Bishop moves
              moves_piece = this->bishop_moves(i, j, true, true);
              break;
            case 'q':
              // Queen moves
              moves_piece = this->queen_moves(i, j, false, true);
              break;
            case 'Q':
              // Queen moves
              moves_piece = this->queen_moves(i, j, true, true);
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

  if(true || show_moves){
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
    if(i==3 && this->board.get_piece(i+1, j)==' ' && this->board.get_piece(i+2, j)==' ' && this->board.is_valid(i+2, j) && this->board.get_push(is_white, i+2, j)){
      std::string target_sq=this->notate_square(i+2,j);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
    // One square diagonal (capture) --> king capture should never happen unless checkmate
    if((this->board.get_piece(i+1, j+1)=='p' || this->board.get_piece(i+1, j+1)=='r' || this->board.get_piece(i+1, j+1)=='n' || this->board.get_piece(i+1, j+1)=='b' || this->board.get_piece(i+1, j+1)=='q' || this->board.get_piece(i+1, j+1)=='k') || record_attack && this->board.is_valid(i+1, j+1) && this->board.get_capture(is_white, i+1, j+1)){
      std::string target_sq=this->notate_square(i+1,j+1);
      std::string move=orig_sq+target_sq;
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
    if(i==8 && this->board.get_piece(i-1, j)==' ' && this->board.get_piece(i-2, j)==' ' && this->board.is_valid(i-2, j) && this->board.get_push(is_white, i-2, j)){
      std::string target_sq=this->notate_square(i-2,j);
      std::string move=orig_sq+target_sq;
      moves_pawn.push_back(move);
    }
    // One square diagonal (capture) --> king capture should never happen unless checkmate
    if((this->board.get_piece(i-1, j+1)=='P' || this->board.get_piece(i-1, j+1)=='R' || this->board.get_piece(i-1, j+1)=='N' || this->board.get_piece(i-1, j+1)=='B' || this->board.get_piece(i-1, j+1)=='Q' || this->board.get_piece(i-1, j+1)=='K') || record_attack && this->board.is_valid(i-1, j+1) && this->board.get_capture(is_white, i-1, j+1)){
      std::string target_sq=this->notate_square(i-1,j+1);
      std::string move=orig_sq+target_sq;
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

std::vector<std::string> Mover::rook_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
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
    else if(decision=="capture"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j);
            
          // Checker
          if(this->board.get_piece(ind, j)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j);
          
          // Checker
          if(this->board.get_piece(ind, j)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j)=='k' && is_white) || (this->board.get_piece(ind, j)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j);
        else
          this->board.set_black_attack(ind, j);
      }
    }
  }

  // Up movements
  for(int ind=i+1; ind<=9; ind++){
    // Target square
    std::string target_sq=this->notate_square(ind,j);
    std::string move=orig_sq+target_sq;

    // Decision of what to do with the target square
    std::string decision=this->basic_move_capture(ind, j, is_white, record_attack);
    if(decision=="block" || decision=="invalid")
      break;
    else if(decision=="capture"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j);
            
          // Checker
          if(this->board.get_piece(ind, j)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j);
          
          // Checker
          if(this->board.get_piece(ind, j)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j)=='k' && is_white) || (this->board.get_piece(ind, j)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j);
        else
          this->board.set_black_attack(ind, j);
      }
    }
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
    else if(decision=="capture"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i, ind);
            
          // Checker
          if(this->board.get_piece(i, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i, ind);
          
          // Checker
          if(this->board.get_piece(i, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i, ind)=='k' && is_white) || (this->board.get_piece(i, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i, ind);
        else
          this->board.set_black_attack(i, ind);
      }
    }
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
    else if(decision=="capture"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i, ind);
            
          // Checker
          if(this->board.get_piece(i, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i, ind);
          
          // Checker
          if(this->board.get_piece(i, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i, ind)=='k' && is_white) || (this->board.get_piece(i, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_rook.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i, ind);
        else
          this->board.set_black_attack(i, ind);
      }
    }
  }

  return moves_rook;
}

std::vector<std::string> Mover::bishop_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
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
    else if(decision=="capture"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j-diff_squares);
            
          // Checker
          if(this->board.get_piece(ind, j-diff_squares)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j-diff_squares);
          
          // Checker
          if(this->board.get_piece(ind, j-diff_squares)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j-diff_squares)=='k' && is_white) || (this->board.get_piece(ind, j-diff_squares)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j-diff_squares);
        else
          this->board.set_black_attack(ind, j-diff_squares);
      }
    }
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
    else if(decision=="capture"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j+diff_squares);
            
          // Checker
          if(this->board.get_piece(ind, j+diff_squares)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j+diff_squares);
          
          // Checker
          if(this->board.get_piece(ind, j+diff_squares)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j+diff_squares)=='k' && is_white) || (this->board.get_piece(ind, j+diff_squares)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j+diff_squares);
        else
          this->board.set_black_attack(ind, j+diff_squares);
      }
    }
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
    else if(decision=="capture"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i+diff_squares, ind);
            
          // Checker
          if(this->board.get_piece(i+diff_squares, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i+diff_squares, ind);
          
          // Checker
          if(this->board.get_piece(i+diff_squares, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i+diff_squares, ind)=='k' && is_white) || (this->board.get_piece(i+diff_squares, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i+diff_squares,ind);
        else
          this->board.set_black_attack(i+diff_squares,ind);
      }
    }
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
    else if(decision=="capture"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i-diff_squares, ind);
            
          // Checker
          if(this->board.get_piece(i-diff_squares, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i-diff_squares, ind);
          
          // Checker
          if(this->board.get_piece(i-diff_squares, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i-diff_squares, ind)=='k' && is_white) || (this->board.get_piece(i-diff_squares, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_bishop.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i-diff_squares,ind);
        else
          this->board.set_black_attack(i-diff_squares,ind);
      }
    }
  }

  return moves_bishop;
}

std::vector<std::string> Mover::queen_moves(int i, int j, bool is_white, bool record_attack/*=false*/){
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j);
            
          // Checker
          if(this->board.get_piece(ind, j)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j);
          
          // Checker
          if(this->board.get_piece(ind, j)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j)=='k' && is_white) || (this->board.get_piece(ind, j)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j);
        else
          this->board.set_black_attack(ind, j);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j);
            
          // Checker
          if(this->board.get_piece(ind, j)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j);
          
          // Checker
          if(this->board.get_piece(ind, j)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j)=='k' && is_white) || (this->board.get_piece(ind, j)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j);
        else
          this->board.set_black_attack(ind, j);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i, ind);
            
          // Checker
          if(this->board.get_piece(i, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i, ind);
          
          // Checker
          if(this->board.get_piece(i, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i, ind)=='k' && is_white) || (this->board.get_piece(i, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i, ind);
        else
          this->board.set_black_attack(i, ind);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i, ind);
            
          // Checker
          if(this->board.get_piece(i, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i, ind);
          
          // Checker
          if(this->board.get_piece(i, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i, ind)=='k' && is_white) || (this->board.get_piece(i, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i, ind);
        else
          this->board.set_black_attack(i, ind);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j-diff_squares);
            
          // Checker
          if(this->board.get_piece(ind, j-diff_squares)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j-diff_squares);
          
          // Checker
          if(this->board.get_piece(ind, j-diff_squares)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j-diff_squares)=='k' && is_white) || (this->board.get_piece(ind, j-diff_squares)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j-diff_squares);
        else
          this->board.set_black_attack(ind, j-diff_squares);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(ind, j+diff_squares);
            
          // Checker
          if(this->board.get_piece(ind, j+diff_squares)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(ind, j+diff_squares);
          
          // Checker
          if(this->board.get_piece(ind, j+diff_squares)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(ind, j+diff_squares)=='k' && is_white) || (this->board.get_piece(ind, j+diff_squares)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(ind, j+diff_squares);
        else
          this->board.set_black_attack(ind, j+diff_squares);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i+diff_squares, ind);
            
          // Checker
          if(this->board.get_piece(i+diff_squares, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i+diff_squares, ind);
          
          // Checker
          if(this->board.get_piece(i+diff_squares, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i+diff_squares, ind)=='k' && is_white) || (this->board.get_piece(i+diff_squares, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i+diff_squares,ind);
        else
          this->board.set_black_attack(i+diff_squares,ind);
      }
    }
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
    else if(decision=="capture"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white){
          this->board.set_white_attack(i-diff_squares, ind);
            
          // Checker
          if(this->board.get_piece(i-diff_squares, ind)=='k'){
            this->board.set_checker_white(i,j);
          }
        }
        else{
          this->board.set_black_attack(i-diff_squares, ind);
          
          // Checker
          if(this->board.get_piece(i-diff_squares, ind)=='K'){
            this->board.set_checker_black(i,j);
          }
        }
      }
      if(!(record_attack && ((this->board.get_piece(i-diff_squares, ind)=='k' && is_white) || (this->board.get_piece(i-diff_squares, ind)=='K' && !is_white))))
        break;
    }
    else if(decision=="move"){
      moves_queen.push_back(move);
      // Record an attacked squared
      if(record_attack){
        if(is_white)
          this->board.set_white_attack(i-diff_squares,ind);
        else
          this->board.set_black_attack(i-diff_squares,ind);
      }
    }
  }

  return moves_queen;
}

std::vector<std::string> Mover::king_moves(int i, int j, bool is_white, bool check_castling/*=true*/, bool record_attack/*=false*/){
  // Initialize the vector
  std::vector<std::string> moves_king={};

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Down movement
  // Target square
  std::string target_sq=this->notate_square(i-1,j);
  std::string move=orig_sq+target_sq;
  // Decision of what to do with the target square
  std::string decision=this->basic_move_capture(i-1, j, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i-1, j)) || (!is_white && !this->board.is_square_attacked_by_white(i-1, j))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i-1,j);
      else
        this->board.set_black_attack(i-1,j);
    }
  }

  // Up movement
  // Target square
  target_sq=this->notate_square(i+1,j);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1, j, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i+1,j)) || (!is_white && !this->board.is_square_attacked_by_white(i+1,j))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i+1, j);
      else
        this->board.set_black_attack(i+1, j);
    }
  }

  // Left movements
  // Target square
  target_sq=this->notate_square(i,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i, j-1, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i,j-1)) || (!is_white && !this->board.is_square_attacked_by_white(i,j-1))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i,j-1);
      else
        this->board.set_black_attack(i,j-1);
    }
  }

  // Right movements
  // Target square
  target_sq=this->notate_square(i,j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i, j+1, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i, j+1)) || (!is_white && !this->board.is_square_attacked_by_white(i, j+1))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i,j+1);
      else
        this->board.set_black_attack(i,j+1);
    }
  }

  // Left-down movements
  // Target square
  target_sq=this->notate_square(i-1, j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1, j-1, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i-1, j-1)) || (!is_white && !this->board.is_square_attacked_by_white(i-1, j-1))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i-1, j-1);
      else
        this->board.set_black_attack(i-1, j-1);
    }
  }

  // Right-up movements
  // Target square
  target_sq=this->notate_square(i+1, j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1, j+1, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i+1, j+1)) || (!is_white && !this->board.is_square_attacked_by_white(i+1, j+1))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i+1, j+1);
      else
        this->board.set_black_attack(i+1, j+1);
    }
  }

  // Left-up movements
  // Target square
  target_sq=this->notate_square(i+1,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1, j-1, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i+1,j-1)) || (!is_white && !this->board.is_square_attacked_by_white(i+1,j-1))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i+1,j-1);
      else
        this->board.set_black_attack(i+1,j-1);
    }
  }

  // Right-down movements
  // Target square
  target_sq=this->notate_square(i-1,j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1,j+1, is_white, true);
  if(decision=="move" || decision=="capture")
  {
    // Avoid entering in an attacked square
    if((is_white && !this->board.is_square_attacked_by_black(i-1,j+1)) || (!is_white && !this->board.is_square_attacked_by_white(i-1,j+1))) 
      moves_king.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white)
        this->board.set_white_attack(i-1,j+1);
      else
        this->board.set_black_attack(i-1,j+1);
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

  // Original square
  std::string orig_sq=this->notate_square(i,j);

  // Vertical up-right L
  // Target square
  std::string target_sq=this->notate_square(i+2,j+1);
  std::string move=orig_sq+target_sq;
  // Decision of what to do with the target square
  std::string decision=this->basic_move_capture(i+2,j+1, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i+2,j+1);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i+2,j+1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i+2,j+1);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i+2,j+1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Vertical up-left L
  // Target square
  target_sq=this->notate_square(i+2,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+2,j-1, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i+2,j-1);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i+2,j-1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i+2,j-1);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i+2,j-1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Horizontal up-right L
  // Target square
  target_sq=this->notate_square(i+1,j+2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1,j+2, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i+1,j+2);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i+1,j+2)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i+1,j+2);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i+1,j+2)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Horizontal up-left L
  // Target square
  target_sq=this->notate_square(i+1,j-2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i+1,j-2, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i+1,j-2);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i+1,j-2)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i+1,j-2);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i+1,j-2)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Vertical down-right L
  // Target square
  target_sq=this->notate_square(i-2,j+1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-2,j+1, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i-2,j+1);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i-2,j+1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i-2,j+1);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i-2,j+1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Vertical down-left L
  // Target square
  target_sq=this->notate_square(i-2,j-1);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-2,j-1, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i-2,j-1);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i-2,j-1)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i-2,j-1);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i-2,j-1)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Horizontal down-right L
  // Target square
  target_sq=this->notate_square(i-1,j+2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1,j+2, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i-1,j+2);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i-1,j+2)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i-1,j+2);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i-1,j+2)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  // Horizontal down-left L
  // Target square
  target_sq=this->notate_square(i-1,j-2);
  move=orig_sq+target_sq;
  // Decision of what to do with the target square
  decision=this->basic_move_capture(i-1,j-2, is_white);
  if(decision=="move" || decision=="capture"){
    moves_knight.push_back(move);
    // Record an attacked squared
    if(record_attack){
      if(is_white){
        this->board.set_white_attack(i-1,j-2);
          
        // Checker
        if(decision=="capture" && this->board.get_piece(i-1,j-2)=='k'){
          this->board.set_checker_white(i,j);
        }
      }
      else{
        this->board.set_black_attack(i-1,j-2);
        
        // Checker
        if(decision=="capture" && this->board.get_piece(i-1,j-2)=='K'){
          this->board.set_checker_black(i,j);
        }
      }
    }
  }

  return moves_knight;
}