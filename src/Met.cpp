#include "Met.h"
#include "Board.h"
#include "Position.h"

#include <cstdlib>

Met::Met(Color color)
    : Piece("เม็ด", "♛", color)
{
    type = PieceType::Met;
}

bool Met::CanMove(
    Board& board,
    Position from,
    Position to
)
{
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);

    // เม็ดเดินเฉียง 1 ช่อง
    if (dx == 1 && dy == 1)
    {
        Piece* target = board.GetPiece(to);

        // ช่องว่าง เดินได้
        if (target == nullptr)
        {
            return true;
        }

        // เจอฝ่ายตรงข้าม กินได้
        if (target->GetColor() != GetColor())
        {
            return true;
        }
    }

    return false;
}