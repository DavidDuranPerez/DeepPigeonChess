/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "board.h"
#include <vector>

// Constructor for the square
Square::Square(int row, char column, char piece, bool is_valid)
{
    this->row = row;
    this->column = column;
    this->piece = piece;
    this->is_valid = is_valid;
};

// Get the piece
char Square::get_piece(){
    return this->piece;
}

// Constructor for the board --> default initial position
Board::Board(std::string fen_string)
{
    // Debugging
    this->debugging=true;

    // Starting position
    this->fen2pos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

// Split a string
template <class Container>
void Board::split_str(const std::string &str, Container &cont, char delim)
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim))
    {
        cont.push_back(token);
    }
}

// Convert a FEN row to a fixed 8 row
std::string Board::convert_fenrow(std::string fen_row){
    // Raw pieces (in FEN format)
    std::string pieces_raw = fen_row;
    
    // Fixed 8 pieces
    std::string pieces;
    
    // Loop it
    int count_clean = 0;
    for (int count = 0; count < pieces_raw.length(); count++)
    {
        if (pieces_raw[count] == 'p' || pieces_raw[count] == 'r' || pieces_raw[count] == 'n' || pieces_raw[count] == 'b' || pieces_raw[count] == 'q' || pieces_raw[count] == 'k' || pieces_raw[count] == 'P' || pieces_raw[count] == 'R' || pieces_raw[count] == 'N' || pieces_raw[count] == 'B' || pieces_raw[count] == 'Q' || pieces_raw[count] == 'K')
        {
            pieces += pieces_raw[count];
            count_clean++;
        }
        else
        {
            for(int ind=0; ind<(int)pieces_raw[count]; ind++){
                pieces += ' ';
                count_clean++;
            }
        }
    }

    // Return the pieces
    return pieces;
}

// Convert FEN to position
void Board::fen2pos(std::string fen_str)
{
    // Split the FEN
    std::vector<std::string> fen_splitted;
    this->split_str(fen_str, fen_splitted, ' ');

    // Get the position in a string
    std::string pos = fen_splitted[0];
    std::vector<std::string> pos_splitted;
    this->split_str(pos, pos_splitted, '/');

    // Mapping for the columns
    std::map<int, char> col_map = {
        {2, 'a'},
        {3, 'b'},
        {4, 'c'},
        {5, 'd'},
        {6, 'e'},
        {7, 'f'},
        {8, 'g'},
        {9, 'h'}};

    // Set the squares
    for (int i = sizeof(*this->squares) / sizeof(**this->squares) - 1; i >= 0; i--)
    { // row
        // Get the pieces
        std::string pieces;
        if (i >= 10 || i <= 1)
            pieces = ' ';
        else
        {
            // Convert the raw pieces to 8 fixed format
            pieces = this->convert_fenrow(pos_splitted[8 - (i - 1)]);
        }

        for (int j = 0; j < sizeof(this->squares) / sizeof(*this->squares); j++)
        { // column
            if (i >= 10 || i <= 1 || j >= 10 || j <= 1)
            {
                this->squares[i][j] = Square(0, 'x', ' ', false);
            }
            else
            {
                this->squares[i][j] = Square(i - 1, col_map[j], pieces[j - 2], true);
            }
            if(this->debugging)
                std::cout << this->squares[i][j].get_piece();
        }
        if(this->debugging)
            std::cout << "\n";
    }

    // See who has to move
    if(fen_splitted[1]=="w")
        this->white_moves=true;
    else
        this->white_moves=false;

    // Get the allowed castles
    this->allowed_castles = fen_splitted[2];

    // En passant
    this->en_passant_target = fen_splitted[3];

    // Get halfmoves
    this->halfmoves=std::stoi(fen_splitted[4]);

    // Get fullmoves
    this->fullmoves=std::stoi(fen_splitted[5]);
}