/*
  Deep Pigeon, a chess engine
  Copyright (C) 2019, David Duran
*/

#include "board.h"
#include <vector>

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
std::map<int, char> col_map_reverse = {
    {'a', 2},
    {'b', 3},
    {'c', 4},
    {'d', 5},
    {'e', 6},
    {'f', 7},
    {'g', 8},
    {'h', 9}};

// Constructor for the square
Square::Square(int row, char column, char piece, bool is_valid)
{
    this->row = row;
    this->column = column;
    this->piece = piece;
    this->is_valid = is_valid;
};

// Get the piece
char Square::get_piece()
{
    return this->piece;
}

// Set the piece
void Square::set_piece(char piece)
{
    this->piece = piece;
}

// Default constructor
Board::Board(){
    this->debugging = true;
}

// Constructor for the board --> default initial position
Board::Board(std::string fen_string)
{
    // Debugging
    this->debugging = true;

    // Starting position
    this->fen2pos(fen_string);
}

// Subtract a character from a string
std::string Board::subtract_character(std::string orig_str, char ch2remove){
    std::string mod_str="";
    for(int i=0; i<orig_str.length(); i++){
        if(orig_str[i]!=ch2remove)
            mod_str+=orig_str[i];
    }
    return mod_str;
}

// See if a type of castle is allowed
bool Board::is_castle_allowed(char type_castle){
    for(int i=0; i<this->allowed_castles.length(); i++){
        if(this->allowed_castles[i]==type_castle)
            return true;
    }
    return false;
}

// Move
// This function assumes that the move is legal. It is not the responsibility of this function to check if it is legal
void Board::move(std::string fromto)
{
    // It is always four characters
    char from[2];
    from[0] = fromto[0];
    from[1] = fromto[1];
    char to[2];
    to[0] = fromto[2];
    to[1] = fromto[3];

    // Transform from algebraic to internal notation
    int col_from = col_map_reverse[from[0]];
    int row_from = (int)from[1] - '0' + 1;
    int col_to = col_map_reverse[to[0]];
    int row_to = (int)to[1] - '0' + 1;

    // Get the piece from
    char piece_from = this->squares[row_from][col_from].get_piece();
    char piece_to = this->squares[row_to][col_to].get_piece();

    // Set an empty piece in the from and set the from piece in the to
    this->squares[row_from][col_from].set_piece(' ');
    this->squares[row_to][col_to].set_piece(piece_from);

    // Castling
    if(fromto=="e1g1"){ // Short castling for white
        // Change the piece
        this->squares[2][9].set_piece(' ');
        this->squares[2][7].set_piece('R');
        // Modify the allowed castles
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'K');
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'Q');
    }
    else if(fromto=="e1c1"){ // Long castling for white
        // Change the piece
        this->squares[2][2].set_piece(' ');
        this->squares[2][5].set_piece('R');
        // Modify the allowed castles
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'K');
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'Q');
    }
    else if(fromto=="e8g8"){ // Short castling for black
        // Change the piece
        this->squares[9][9].set_piece(' ');
        this->squares[9][7].set_piece('R');
        // Modify the allowed castles
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'k');
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'q');
    }
    else if(fromto=="e8c8"){ // Long castling for black
        // Change the piece
        this->squares[9][2].set_piece(' ');
        this->squares[9][5].set_piece('R');
        // Modify the allowed castles
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'k');
        this->allowed_castles=this->subtract_character(this->allowed_castles, 'q');
    }
    else{
        // Cases where the castle is not allowed anymore (a check is not included since it is allowed if afterwards not in check)
        if(piece_from=='K'){ // White king moves
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'K');
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'Q');
        }
        else if(piece_from=='k'){ // Black king moves
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'k');
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'q');  
        }
        else if(piece_from=='R' && col_from==2 && row_from==2){ // White long rook
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'Q');
        }
        else if(piece_from=='R' && col_from==9 && row_from==2){ // White short rook
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'K');
        }
        else if(piece_from=='r' && col_from==2 && row_from==9){ // Black long rook
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'q');
        }
        else if(piece_from=='r' && col_from==9 && row_from==9){ // Black short rook
            this->allowed_castles=this->subtract_character(this->allowed_castles, 'k');
        }
    }

    // Promotion (if any)
    if(fromto.length()>4){
        if(this->white_moves){
            this->squares[row_to][col_to].set_piece(toupper(fromto[4]));
        }
        else{
            this->squares[row_to][col_to].set_piece(fromto[4]);
        }
    }

    // Change the turn
    this->white_moves = !this->white_moves;

    // En passant --> TBD!!!

    // Full moves
    if (this->white_moves)
        this->fullmoves += 1;

    // Half moves
    if (piece_to == ' ' && piece_from != 'p' && piece_from != 'P')
        this->halfmoves += 1;
    else
        this->halfmoves = 0;

    // Print the board if debugging
    if (this->debugging)
        this->print_board();
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
std::string Board::convert_fenrow(std::string fen_row)
{
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
            for (int ind = 0; ind < (int)pieces_raw[count] - 48; ind++)
            {
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
        }
    }

    // See who has to move
    if (fen_splitted[1] == "w")
        this->white_moves = true;
    else
        this->white_moves = false;

    // Get the allowed castles
    this->allowed_castles = fen_splitted[2];

    // En passant
    this->en_passant_target = fen_splitted[3];

    // Get halfmoves
    this->halfmoves = std::stoi(fen_splitted[4]);

    // Get fullmoves
    this->fullmoves = std::stoi(fen_splitted[5]);

    // Print the board if debugging
    if (this->debugging)
        this->print_board();
}

// Print the board (for debugging purposes)
void Board::print_board()
{
    for (int i = sizeof(*this->squares) / sizeof(**this->squares) - 1; i >= 0; i--)
    { // row
        // Get the pieces
        std::string pieces;
        if (i < 10 && i > 1)
            std::cout << "+---+---+---+---+---+---+---+---+"
                      << "\n|";

        for (int j = 0; j < sizeof(this->squares) / sizeof(*this->squares); j++)
        { // column
            if (i < 10 && i > 1 && j < 10 && j > 1)
                std::cout << " " << this->squares[i][j].get_piece() << " |";
        }
        std::cout << "\n";
        if (i == 2){
            std::cout << "+---+---+---+---+---+---+---+---+" << "\n";
            std::cout << "White to move?: " << std::boolalpha << this->white_moves << "; Allowed castles: " << this->allowed_castles << "; En-passant targets: " << this->en_passant_target << "; Halfmoves: " << this->halfmoves << "; Fullmoves:" << this->fullmoves;
        }
    }
}