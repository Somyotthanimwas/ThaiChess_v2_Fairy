#include "Piece.h"
#include <cstdlib>

#ifdef __linux__
#include <iostream>
#endif


// ============================
// Constructor
// ============================

Piece::Piece(
    const std::string& name,
    const std::string& symbol,
    Color color
)
:
name(name),
symbol(symbol),
color(color)
{

}


// ============================
// Destructor
// ============================

Piece::~Piece()
{

}


// ============================
// Get Thai Name
// ============================

std::string Piece::GetName() const
{
    return name;
}


// ============================
// Get English Name (Debug)
// ============================

std::string Piece::GetEnglishName() const
{
    if(name == "ม้า")
        return "Knight";

    if(name == "เรือ")
        return "Rook";

    if(name == "โคน")
        return "Khon";

    if(name == "เม็ด")
        return "Met";

    if(name == "ขุน")
        return "King";

    if(name == "เบี้ย")
        return "Pawn";


    return "Unknown";
}


// ============================
// Get Symbol
// ============================

std::string Piece::GetSymbol() const
{
    return symbol;
}


// ============================
// Get Colored Symbol
// ============================

std::string Piece::GetColoredSymbol() const
{

#ifdef __linux__

    if(color == Color::White)
    {
        return "\033[97m" + symbol + "\033[0m";
    }
    else
    {
        return "\033[91m" + symbol + "\033[0m";
    }

#else

    return symbol;

#endif

}


// ============================
// Get Color
// ============================

Color Piece::GetColor() const
{
    return color;
}


// ============================
// Get Type
// ============================

PieceType Piece::GetType() const
{
    return type;
}