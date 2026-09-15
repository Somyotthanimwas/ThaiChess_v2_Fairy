#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece
{

public:

    Rook(Color color);


    bool CanMove(
    const Board& board,
    Position from,
    Position to
) const override;

};

#endif