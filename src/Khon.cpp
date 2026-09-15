#include "Khon.h"
#include "Board.h"
#include "Position.h"

#include <cstdlib>

Khon::Khon(Color color)
    : Piece("โคน", "♝", color)
{
    type = PieceType::Khon;
}

bool Khon::CanMove(
    Board& board,
    Position from,
    Position to
)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;

    bool canMove = false;

    if (GetColor() == Color::White)
    {
        // ขาวอยู่ล่าง เดินขึ้น
        if (dx == 0 && dy == -1)
            canMove = true;

        if (abs(dx) == 1 && dy == -1)
            canMove = true;

        if (abs(dx) == 1 && dy == 1)
            canMove = true;
    }
    else
    {
        // ดำอยู่บน เดินลง
        if (dx == 0 && dy == 1)
            canMove = true;

        if (abs(dx) == 1 && dy == 1)
            canMove = true;

        if (abs(dx) == 1 && dy == -1)
            canMove = true;
    }

    if (canMove)
    {
        Piece* target = board.GetPiece(to);

        // ช่องว่าง
        if (target == nullptr)
        {
            return true;
        }

        // กินฝ่ายตรงข้าม
        if (target->GetColor() != GetColor())
        {
            return true;
        }
    }

    return false;
}