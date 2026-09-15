#ifndef MET_H
#define MET_H

#include "Piece.h"


class Met : public Piece
{

public:

    Met(Color color);


    bool CanMove(
        Board& board,
        Position from,
        Position to
    ) override;


};


#endif