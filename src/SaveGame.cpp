#include "SaveGame.h"
#include "Rook.h"
#include "King.h"
#include "Pawn.h"
#include "Knight.h"
#include "Met.h"
#include "Khon.h"

#include <fstream>
#include <iostream>


bool SaveGame::Save(
    const Board& board,
    const std::string& filename
)
{
    std::ofstream file(filename);


    if(!file)
    {
        return false;
    }


    for(int y = 0; y < 8; y++)
{
    for(int x = 0; x < 8; x++)
    {
        Piece* piece = board.GetPiece(
            Position{x,y}
        );


        if(piece == nullptr)
{
    file << ".";
}
else
if(piece->GetColor() == Color::White)
{
    switch(piece->GetType())
    {
        case PieceType::King:   file << "K"; break;
        case PieceType::Rook:   file << "R"; break;
        case PieceType::Knight: file << "N"; break;
        case PieceType::Khon:   file << "C"; break;
        case PieceType::Met:    file << "M"; break;
        case PieceType::Pawn:   file << "P"; break;
    }
}
else
{
    switch(piece->GetType())
    {
        case PieceType::King:   file << "k"; break;
        case PieceType::Rook:   file << "r"; break;
        case PieceType::Knight: file << "n"; break;
        case PieceType::Khon:   file << "c"; break;
        case PieceType::Met:    file << "m"; break;
        case PieceType::Pawn:   file << "p"; break;
    }
}
    }

    file << "\n";
}


std::cout << "Save file: "
          << filename
          << "\n";


// Save Turn
if(board.GetTurn() == Color::White)
{
    file << "TURN W\n";
}
else
{
    file << "TURN B\n";
}


file.close();

return true;

}



bool SaveGame::Load(
    Board& board,
    const std::string& filename
)
{
    std::ifstream file(filename);


    if(!file)
    {
        return false;
    }


   std::string line;


board.Clear();


for(int y = 0; y < 8; y++)
{
    getline(file,line);


    int x = 0;

    for(size_t i = 0; i < line.size(); )
    {

        std::string symbol;


        symbol = line.substr(i,1);
i++;

if(symbol == "K")
{
    board.SetPiece(Position{x,y}, new King(Color::White));
}

if(symbol == "R")
{
    board.SetPiece(Position{x,y}, new Rook(Color::White));
}

if(symbol == "N")
{
    board.SetPiece(Position{x,y}, new Knight(Color::White));
}

if(symbol == "C")
{
    board.SetPiece(Position{x,y}, new Khon(Color::White));
}

if(symbol == "M")
{
    board.SetPiece(Position{x,y}, new Met(Color::White));
}

if(symbol == "P")
{
    board.SetPiece(Position{x,y}, new Pawn(Color::White));
}

if(symbol == "k")
{
    board.SetPiece(Position{x,y}, new King(Color::Black));
}

if(symbol == "r")
{
    board.SetPiece(Position{x,y}, new Rook(Color::Black));
}

if(symbol == "n")
{
    board.SetPiece(Position{x,y}, new Knight(Color::Black));
}

if(symbol == "c")
{
    board.SetPiece(Position{x,y}, new Khon(Color::Black));
}

if(symbol == "m")
{
    board.SetPiece(Position{x,y}, new Met(Color::Black));
}

if(symbol == "p")
{
    board.SetPiece(Position{x,y}, new Pawn(Color::Black));
}
       
        x++;
    }
}
 
// Load Turn
getline(file,line);

if(line == "TURN W")
{
    board.SetTurn(Color::White);
}

if(line == "TURN B")
{
    board.SetTurn(Color::Black);
}

std::cout << "Load file: "
          << filename
          << "\n";



file.close();

return true;

}