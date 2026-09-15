#ifndef PIECE_H
#define PIECE_H


#include "Color.h"
#include "Position.h"
#include "PieceType.h"

#include <string>


class Board;


class Piece
{

protected:

    std::string name;
    std::string symbol;
    Color color;
    PieceType type;


public:

    Piece(
        const std::string& name,
        const std::string& symbol,
        Color color
    );


    virtual ~Piece();


    std::string GetName() const;

    std::string GetSymbol() const;

    std::string GetColoredSymbol() const;   // เพิ่มบรรทัดนี้

    Color GetColor() const;

    PieceType GetType() const;


    virtual bool CanMove(
        const Board& board,
        Position from,
        Position to
    ) const = 0;

};


#endif