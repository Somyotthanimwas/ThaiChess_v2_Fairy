#ifndef KING_H
#define KING_H

#include "Piece.h"


class King : public Piece
{

public:

    King(Color color);


    bool CanMove(
        Board& board,
        Position from,
        Position to
    ) override;


};


#endif