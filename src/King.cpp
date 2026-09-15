#include "King.h"
#include "Board.h"
#include "Position.h"

#include <cstdlib>

King::King(Color color)
    : Piece("ขุน", "♚", color)
{
    type = PieceType::King;
}

bool King::CanMove(
    Board& board,
    Position from,
    Position to
)
{
    int dx = abs(to.x - from.x);
    int dy = abs(to.y - from.y);

    // ห้ามอยู่ที่เดิม
    if (dx == 0 && dy == 0)
    {
        return false;
    }

    if (dx <= 1 && dy <= 1)
    {
        // ห้ามเดินติดขุนฝ่ายตรงข้าม
        for (int y = 0; y < 8; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                Piece* piece = board.GetPiece(Position{x, y});

                if (piece != nullptr &&
                    piece->GetType() == PieceType::King &&
                    piece->GetColor() != color)
                {
                    int kx = abs(to.x - x);
                    int ky = abs(to.y - y);

                    if (kx <= 1 && ky <= 1)
                    {
                        return false;
                    }
                }
            }
        }

        return true;
    }

    return false;
}