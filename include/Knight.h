#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece
{

public:

    Knight(Color color);


    bool CanMove(
        Board& board,
        Position from,
        Position to
    ) override;


};

#endif