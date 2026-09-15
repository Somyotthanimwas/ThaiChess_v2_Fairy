#include "Knight.h"
#include "Board.h"

#include <cstdlib>


Knight::Knight(Color color)
    : Piece("ม้า","♞",color)
{
    type = PieceType::Knight;
}



bool Knight::CanMove(
    Board& board,
    Position from,
    Position to
)
{
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);



    // ม้าเดินรูปตัว L
    if(
        (dx == 2 && dy == 1) ||
        (dx == 1 && dy == 2)
    )
    {
        Piece* target = board.GetPiece(to);



        // ช่องว่าง เดินได้
        if(target == nullptr)
        {
            return true;
        }



        // กินฝ่ายตรงข้าม
        if(target->GetColor() != GetColor())
        {
            return true;
        }
    }



    // เดินไม่ได้
    return false;
}