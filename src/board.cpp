/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "board.h"
#include <vector>

// Constructor for the square
Square::Square(int row, char column, char piece, bool is_valid){
    this->row = row;
    this->column = column;
    this->piece = piece;
    this->is_valid = is_valid;
};

// Constructor for the board --> default initial position
Board::Board(){
    // Starting position
    this->fen2pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

// Split a string
template <class Container>
void Board::split_str(const std::string& str, Container& cont, char delim){
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}

// Convert FEN to position
void Board::fen2pos(std::string fen_str){
    // Split the FEN
    std::vector<std::string> fen_splitted;
    this->split_str(fen_str, fen_splitted, ' ');

    // Get the position in a string
    std::string pos = fen_splitted[0];
    std::vector<std::string> pos_splitted;
    this->split_str(pos, pos_splitted, '/');

    // Mapping for the columns
    std::map<int, char> col_map = {
        { 2, 'a' },
        { 3, 'b' },
        { 4, 'c' },
        { 5, 'd' },
        { 6, 'e' },
        { 7, 'f' },
        { 8, 'g' },
        { 9, 'h' }
    };

    // Set the squares
    for(int i=sizeof(*this->squares)/sizeof(**this->squares)-1; i>=0; i--){ // row
        for(int j=0; j<sizeof(this->squares)/sizeof(*this->squares); j++){ // column
            if(i>=10 || i<=1 || j>=10 || j<=1){
                this->squares[i][j]=Square(0, 'x', ' ', false);
            }else{
                // Pieces
                std::string pieces = pos_splitted[8-(i-1)];
                this->squares[i][j]=Square(i-1, col_map[j], pieces[j-2], true);
            }
        }
    }
}