#ifndef MOVE_H
#define MOVE_H

#include "Position.h"
#include "Piece.h"

#include <string>


struct MoveRecord
{
    Position from;
    Position to;

    Piece* movedPiece;
    Piece* capturedPiece;

    std::string movedName;
    std::string capturedName;


    // เพิ่มตัวนี้
    bool promoted = false;
};


struct LegalMove
{
    Position from;
    Position to;
};

#endif