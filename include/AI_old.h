#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Move.h"


class AI
{
public:

    static int level;

    static void SetLevel(int value);

    static LegalMove FindBestMove(
        Board& board,
        Color color
    );

    static int Minimax(
        Board& board,
        Color color,
        int depth,
        int alpha,
        int beta
    );

    static int MoveScore(
        Board& board,
        const LegalMove& move
    );

    static int Quiescence(
        Board& board,
        Color color,
        int alpha,
        int beta
    );

private:

    static LegalMove killerMove[20];

};

#endif