#ifndef KING_H
#define KING_H

#include "Piece.h"
#include "Board.h"

class King : public Piece
{
public:

    King(Color color);

    bool CanMove(
        const Board& board,
        Position from,
        Position to
    ) const override;
};

#endif