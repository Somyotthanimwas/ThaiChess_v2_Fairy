#include "Rook.h"
#include "Board.h"

#include <cstdlib>


Rook::Rook(Color color)
    : Piece("เรือ","♜",color)
{
    type = PieceType::Rook;
}



bool Rook::CanMove(
    Board& board,
    Position from,
    Position to
)
{
    // ห้ามอยู่ที่เดิม
    if(from.x == to.x &&
       from.y == to.y)
    {
        return false;
    }


    // ต้องเดินแนวตรงเท่านั้น
    if(from.x != to.x &&
       from.y != to.y)
    {
        return false;
    }



    // เดินแนวนอน
    if(from.y == to.y)
    {
        int step;

        if(to.x > from.x)
            step = 1;
        else
            step = -1;


        for(
            int x = from.x + step;
            x != to.x;
            x += step
        )
        {
            if(board.GetPiece(Position{x, from.y}) != nullptr)
            {
                return false;
            }
        }
    }



    // เดินแนวตั้ง
    if(from.x == to.x)
    {
        int step;

        if(to.y > from.y)
            step = 1;
        else
            step = -1;


        for(
            int y = from.y + step;
            y != to.y;
            y += step
        )
        {
            if(board.GetPiece(Position{from.x, y}) != nullptr)
            {
                return false;
            }
        }
    }



    // ตรวจช่องปลายทาง
    Piece* target = board.GetPiece(to);



    // เดินไปช่องว่าง
    if(target == nullptr)
    {
        return true;
    }



    // กินฝ่ายตรงข้าม
    if(target->GetColor() != GetColor())
    {
        return true;
    }



    // กินฝ่ายเดียวกันไม่ได้
    return false;
}