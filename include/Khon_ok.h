#ifndef KHON_H
#define KHON_H

#include "Piece.h"


class Khon : public Piece
{

public:

    Khon(Color color);


   bool CanMove(
    const Board& board,
    Position from,
    Position to
) const override;

};


#endif