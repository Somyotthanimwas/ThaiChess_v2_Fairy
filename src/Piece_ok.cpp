#include "Piece.h"
#include <cstdlib>

#ifdef __linux__
#include <iostream>
#endif

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


Piece::~Piece()
{

}


std::string Piece::GetName() const
{
    return name;
}


std::string Piece::GetSymbol() const
{
    return symbol;
}


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
Color Piece::GetColor() const
{
    return color;
}


PieceType Piece::GetType() const
{
    return type;
}