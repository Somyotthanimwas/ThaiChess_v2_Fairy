#ifndef KHON_H
#define KHON_H

#include "Piece.h"


class Khon : public Piece
{

public:

    Khon(Color color);


    bool CanMove(
        Board& board,
        Position from,
        Position to
    ) override;


};


#endif