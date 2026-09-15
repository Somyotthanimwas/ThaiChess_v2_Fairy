#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"


class Pawn : public Piece
{

public:

    Pawn(Color color);


    bool CanMove(
    const Board& board,
    Position from,
    Position to
) const override;

};

#endif