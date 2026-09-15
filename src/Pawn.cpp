#include "Pawn.h"
#include "Board.h"
#include "Position.h"

#include <cstdlib>

Pawn::Pawn(Color color)
    : Piece("เบี้ย", "●", color)
{
    type = PieceType::Pawn;
}

bool Pawn::CanMove(
    Board& board,
    Position from,
    Position to
)
{
    int dir;

    if (GetColor() == Color::White)
    {
        dir = -1;   // White อยู่ล่าง เดินขึ้น
    }
    else
    {
        dir = 1;    // Black อยู่บน เดินลง
    }

    int dx = to.x - from.x;
    int dy = to.y - from.y;

    // เดินตรง 1 ช่อง
    if (dx == 0 && dy == dir)
    {
        if (board.GetPiece(to) == nullptr)
        {
            return true;
        }

        return false;
    }

    // กินเฉียง 1 ช่อง
    if (abs(dx) == 1 && dy == dir)
    {
        Piece* target = board.GetPiece(to);

        if (target != nullptr &&
            target->GetColor() != GetColor())
        {
            return true;
        }
    }

    return false;
}