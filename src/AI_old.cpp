#include "AI.h"
#include <algorithm>
#include <iostream>

LegalMove AI::killerMove[20];

// ============================
// AI Level
// ============================

int AI::level = 3;

// ============================
// Set AI Level
// ============================

void AI::SetLevel(int value)
{
    if(value < 1)
    {
        value = 1;
    }


    if(value > 5)
    {
        value = 5;
    }


    level = value;


    std::cout
        << "AI Level = "
        << level
        << std::endl;
}


// ============================
// Move Score (MVV-LVA)
// ============================

int AI::MoveScore(
    Board& board,
    const LegalMove& move)
{
    Piece* attacker =
        board.GetPiece(move.from);

    Piece* victim =
        board.GetPiece(move.to);


    // เดินธรรมดา
    if(victim == nullptr)
    {
        return 0;
    }


    int attackerValue = 0;
    int victimValue = 0;


    // ผู้โจมตี
    switch(attacker->GetType())
    {
        case PieceType::King:
            attackerValue = 10000;
            break;

        case PieceType::Rook:
            attackerValue = 500;
            break;

        case PieceType::Knight:
            attackerValue = 350;
            break;

        case PieceType::Khon:
            attackerValue = 300;
            break;

        case PieceType::Met:
            attackerValue = 150;
            break;

        case PieceType::Pawn:
            attackerValue = 100;
            break;

        default:
            break;
    }


    // ตัวที่ถูกกิน
    switch(victim->GetType())
    {
        case PieceType::King:
            victimValue = 10000;
            break;

        case PieceType::Rook:
            victimValue = 500;
            break;

        case PieceType::Knight:
            victimValue = 350;
            break;

        case PieceType::Khon:
            victimValue = 300;
            break;

        case PieceType::Met:
            victimValue = 150;
            break;

        case PieceType::Pawn:
            victimValue = 100;
            break;

        default:
            break;
    }


    // MVV-LVA
    return (victimValue * 10) - attackerValue;
}
// ============================
// Quiescence Search
// ============================

int AI::Quiescence(
    Board& board,
    Color color,
    int alpha,
    int beta)
{
    int standPat = board.Evaluate();


    // ฝั่ง White ต้องการคะแนนสูง
    if(color == Color::White)
    {
        if(standPat >= beta)
            return beta;

        if(standPat > alpha)
            alpha = standPat;
    }
    else
    {
        if(standPat <= alpha)
            return alpha;

        if(standPat < beta)
            beta = standPat;
    }



    auto moves =
        board.GenerateCaptureMoves(color);



    // เรียงการกินก่อน
    std::sort(
        moves.begin(),
        moves.end(),
        [&](const LegalMove& a,
            const LegalMove& b)
        {
            return MoveScore(board,a) >
                   MoveScore(board,b);
        }
    );



    for(auto move : moves)
    {
        Piece* captured =
            board.MakeTestMove(move);



        int score =
            Quiescence(
                board,
                color == Color::White
                    ? Color::Black
                    : Color::White,
                alpha,
                beta
            );



        board.UndoTestMove(
            move,
            captured
        );



        if(color == Color::White)
        {
            if(score > alpha)
                alpha = score;
        }
        else
        {
            if(score < beta)
                beta = score;
        }



        if(beta <= alpha)
            break;
    }


    return color == Color::White
        ? alpha
        : beta;
}

// ============================
// Find Best Move
// ============================

LegalMove AI::FindBestMove(
    Board& board,
    Color color)
{
    LegalMove bestMove;

    bestMove.from = Position{-1,-1};
    bestMove.to   = Position{-1,-1};


    auto moves =
        board.GenerateLegalMoves(color);


    if(moves.empty())
    {
        std::cout
            << "AI: No legal moves\n";

        return bestMove;
    }


    // เอาการกินขึ้นมาก่อน
    std::sort(
    moves.begin(),
    moves.end(),
    [&](const LegalMove& a,
        const LegalMove& b)
    {
        return MoveScore(board, a) >
               MoveScore(board, b);
    }
);



    int bestScore;

    if(color == Color::White)
    {
        bestScore = -999999;
    }
    else
    {
        bestScore = 999999;
    }



    bool first = true;



  for(auto move : moves)
{

    Piece* captured =
    board.MakeTestMove(move);

int depthTable[5] =
{
    1, // Level 1 เร็ว
    2, // Level 2
    3, // Level 3
    4, // Level 4
    5  // Level 5 ช้า เก่ง
};


int depth =
    depthTable[level-1];


int score =
    Minimax(
        board,
        color == Color::White
            ? Color::Black
            : Color::White,
        depth,
        -999999,
        999999
    );
board.UndoTestMove(
    move,
    captured
);

  
        std::cout
            << "Move "
            << move.from.x << ","
            << move.from.y
            << " -> "
            << move.to.x << ","
            << move.to.y
            << " score = "
            << score
            << std::endl;



        if(first)
        {
            bestScore = score;
            bestMove = move;
            first = false;
        }
        else
        {

            if(color == Color::White)
            {
                if(score > bestScore)
                {
                    bestScore = score;
                    bestMove = move;
                }
            }
            else
            {
                if(score < bestScore)
                {
                    bestScore = score;
                    bestMove = move;
                }
            }

        }

    }



    std::cout
        << "AI Best Score = "
        << bestScore
        << std::endl;



    return bestMove;
}





// ============================
// Minimax
// ============================


int AI::Minimax(
    Board& board,
    Color color,
    int depth,
    int alpha,
    int beta)
{


    if(board.IsCheckmate(color))
    {

        if(color == Color::White)
        {
            return -999999;
        }
        else
        {
            return 999999;
        }

    }



  if(depth == 0)
{
    return board.Evaluate();
}


  std::vector<LegalMove> moves;

if(depth <= 1)
{
    moves =
        board.GenerateCaptureMoves(color);
}
else
{
    moves =
        board.GenerateLegalMoves(color);
}

if(depth >= 2)
{
    std::sort(
        moves.begin(),
        moves.end(),
        [&](const LegalMove& a,
            const LegalMove& b)
        {
            return MoveScore(board, a) >
                   MoveScore(board, b);
        }
    );
}  
  if(moves.empty())
    {
        return board.Evaluate();
    }



    int bestScore;



    if(color == Color::White)
    {

        bestScore = -999999;


        for(auto move : moves)
        {

            Piece* captured =
                board.MakeTestMove(move);



            int score =
                Minimax(
                    board,
                    Color::Black,
                    depth - 1,
                    alpha,
                    beta
                );



            board.UndoTestMove(
                move,
                captured
            );



            if(score > bestScore)
            {
                bestScore = score;
            }



            if(bestScore > alpha)
            {
                alpha = bestScore;
            }



            if(beta <= alpha)
            {
                break;
            }

        }

    }
    else
    {

        bestScore = 999999;


        for(auto move : moves)
        {

            Piece* captured =
                board.MakeTestMove(move);



            int score =
                Minimax(
                    board,
                    Color::White,
                    depth - 1,
                    alpha,
                    beta
                );



            board.UndoTestMove(
                move,
                captured
            );



            if(score < bestScore)
            {
                bestScore = score;
            }



            if(bestScore < beta)
            {
                beta = bestScore;
            }



            if(beta <= alpha)
            {
                break;
            }

        }

    }



    return bestScore;
}