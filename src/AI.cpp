#include "AI.h"
#include <algorithm>
#include <iostream>
#include "Position.h"

// ============================
// Piece-Square Tables
// ============================

static const int KnightPST[8][8] =
{
    {-10,-5, 0, 0, 0, 0,-5,-10},
    {-5, 10,15,15,15,15,10,-5},
    { 0, 15,25,30,30,25,15, 0},
    { 0, 15,30,40,40,30,15, 0},
    { 0, 15,30,40,40,30,15, 0},
    { 0, 15,25,30,30,25,15, 0},
    {-5, 10,15,15,15,15,10,-5},
    {-10,-5, 0, 0, 0, 0,-5,-10}
};

static const int RookPST[8][8] =
{
    {0,0,5,5,5,5,0,0},
    {0,5,10,10,10,10,5,0},
    {0,5,10,15,15,10,5,0},
    {0,5,10,15,15,10,5,0},
    {0,5,10,15,15,10,5,0},
    {0,5,10,15,15,10,5,0},
    {0,5,10,10,10,10,5,0},
    {0,0,5,5,5,5,0,0}
};

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
    Piece* attacker = board.GetPiece(move.from);
    Piece* victim   = board.GetPiece(move.to);

    // เดินธรรมดา
    if(attacker == nullptr || victim == nullptr)
    {
        return 0;
    }

    int attackerValue = 0;
    int victimValue = 0;

    switch(attacker->GetType())
    {
        case PieceType::King:   attackerValue = 10000; break;
        case PieceType::Rook:   attackerValue = 500;   break;
        case PieceType::Knight: attackerValue = 350;   break;
        case PieceType::Khon:   attackerValue = 300;   break;
        case PieceType::Met:    attackerValue = 150;   break;
        case PieceType::Pawn:   attackerValue = 100;   break;
        default: break;
    }

    switch(victim->GetType())
    {
        case PieceType::King:   victimValue = 10000; break;
        case PieceType::Rook:   victimValue = 500;   break;
        case PieceType::Knight: victimValue = 350;   break;
        case PieceType::Khon:   victimValue = 300;   break;
        case PieceType::Met:    victimValue = 150;   break;
        case PieceType::Pawn:   victimValue = 100;   break;
        default: break;
    }

    return (victimValue * 8) - attackerValue;
}
int AI::OrderScore(
    Board& board,
    const LegalMove& move,
    int depth)
{
    int score = MoveScore(board, move);

    if(killerMove[depth].from == move.from &&
       killerMove[depth].to   == move.to)
    {
        score += 5000;
    }

    return score;
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


// ถ้าไม่มีการกิน
// ให้หยุดค้น
if(moves.empty())
{
    return standPat;
}

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

/*
               // ===== DEBUG =====


std::cout
    << "CHECK MOVE RESULT: "
    << move.from.x << ","
    << move.from.y
    << " -> "
    << move.to.x << ","
    << move.to.y
    << " score="
    << score
    << std::endl;
// =================
*/

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
// Static Exchange Evaluation
// ============================

int AI::StaticExchange(
    Board& board,
    const LegalMove& move,
    Color color)
{
    Piece* victim =
        board.GetPiece(move.to);


    if(victim == nullptr)
        return 0;


    int gain = 0;


    switch(victim->GetType())
    {
        case PieceType::King:
            gain = 10000;
            break;

        case PieceType::Rook:
            gain = 500;
            break;

        case PieceType::Knight:
            gain = 350;
            break;

        case PieceType::Khon:
            gain = 300;
            break;

        case PieceType::Met:
            gain = 150;
            break;

        case PieceType::Pawn:
            gain = 100;
            break;
    }



    Piece* captured =
        board.MakeTestMove(move);



    Color enemy =
        color == Color::White
        ? Color::Black
        : Color::White;



    auto enemyCaptures =
        board.GenerateCaptureMoves(enemy);



    int loss = 0;



    for(auto& e : enemyCaptures)
    {

        if(e.to.x == move.to.x &&
           e.to.y == move.to.y)
        {

            Piece* attacker =
                board.GetPiece(e.from);


            if(attacker)
            {

                switch(attacker->GetType())
                {

                    case PieceType::King:
                        loss = 10000;
                        break;

                    case PieceType::Rook:
                        loss = 500;
                        break;

                    case PieceType::Knight:
                        loss = 350;
                        break;

                    case PieceType::Khon:
                        loss = 300;
                        break;

                    case PieceType::Met:
                        loss = 150;
                        break;

                    case PieceType::Pawn:
                        loss = 100;
                        break;
                }

            }

            break;
        }
    }



    board.UndoTestMove(
        move,
        captured
    );


    return gain - loss;
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


    // เลือกคะแนนสูงสุด
    int bestScore = -999999;


    for(const auto& move : moves)
    {

      // เก็บตัวหมากก่อนเดิน
Piece* movingPiece =
    board.GetPiece(move.from);


Piece* targetPiece =
    board.GetPiece(move.to);


Piece* captured =
    board.MakeTestMove(move);


// ============================
// DEBUG Test Move
// ============================

if(board.GetPiece(move.to) == nullptr)
{
    std::cout
        << "ERROR MakeTestMove failed : "
        << move.from.x << ","
        << move.from.y
        << " -> "
        << move.to.x << ","
        << move.to.y
        << std::endl;
}


// ตรวจสอบว่าตัวหมากย้ายจริงหรือไม่
if(board.GetPiece(move.from) != nullptr)
{
    std::cout
        << "ERROR old square still has piece : "
        << move.from.x << ","
        << move.from.y
        << std::endl;
}


int safetyPenalty = 0;


Piece* moved =
    board.GetPiece(move.to);
if(moved)
{
    Color enemy =
        color == Color::White
        ? Color::Black
        : Color::White;


    auto attacks =
        board.GenerateCaptureMoves(enemy);


    for(auto& a : attacks)
    {
        if(a.to.x == move.to.x &&
           a.to.y == move.to.y)
        {
            switch(moved->GetType())
            {
                case PieceType::Rook:
                    safetyPenalty = 500;
                    break;

                case PieceType::Knight:
                    safetyPenalty = 350;
                    break;

                case PieceType::Khon:
                    safetyPenalty = 300;
                    break;

                case PieceType::Met:
                    safetyPenalty = 150;
                    break;

                case PieceType::Pawn:
                    safetyPenalty = 100;
                    break;
            }

            break;
        }
    }
}

int depthTable[5] =
{
    2, // Level 1
    3, // Level 2
    4, // Level 3
    5, // Level 4
    6  // Level 5
};


int depth =
    depthTable[level-1];



int score =
    Minimax(
        board,
        color == Color::White
            ? Color::Black
            : Color::White,

        color,   // << เพิ่มบรรทัดนี้

        depth,
        -999999,
        999999
    );

// ============================
// Penalize useless rook moves
// ============================

if(movingPiece &&
   movingPiece->GetType() == PieceType::Rook)
{
    int dx =
        abs(move.to.x - move.from.x);

    int dy =
        abs(move.to.y - move.from.y);


    // เรือเดินกลับไปใกล้จุดเดิม
    if(dx + dy == 1)
    {
        score -= 150;
    }
}
score -= safetyPenalty * 3;


// ============================
// Enemy attack
// ============================

auto enemyMoves =
    board.GenerateCaptureMoves(
        color == Color::White
        ? Color::Black
        : Color::White
    );
// ============================
// Don't hang piece
// ============================

 

if(moved != nullptr)
{

    int value = 0;


    switch(moved->GetType())
    {

        case PieceType::King:
            value = 10000;
            break;

        case PieceType::Rook:
            value = 500;
            break;

        case PieceType::Knight:
            value = 350;
            break;

        case PieceType::Khon:
            value = 300;
            break;

        case PieceType::Met:
            value = 150;
            break;

        case PieceType::Pawn:
            value = 100;
            break;
    }



    for(auto& e : enemyMoves)
    {

        if(e.to.x == move.to.x &&
           e.to.y == move.to.y)
        {
            score -= value * 2;
            break;
        }

    }

}


// ============================
// Capture bonus
// ============================

// ============================
// SEE Capture Check
// ============================

if(captured != nullptr)
{

    int see =
        StaticExchange(
            board,
            move,
            color
        );


    score += see;

}

// ============================
// Avoid immediate back move
// ============================

auto history =
    board.GetHistory();


if(!history.empty())
{

    auto last =
        history.back();


    if(last.from.x == move.to.x &&
       last.from.y == move.to.y &&
       last.to.x == move.from.x &&
       last.to.y == move.from.y)
    {
        score -= 2000;
    }

}
// ============================
// Avoid useless repetition
// ============================


int repeat = 0;


for(auto& h : history)
{
    if(h.from.x == move.from.x &&
       h.from.y == move.from.y &&
       h.to.x == move.to.x &&
       h.to.y == move.to.y)
    {
        repeat++;
    }
}


if(repeat > 0)
{
    score -= repeat * 1000;
}

// ============================
// Stop promotion
// ============================

for(auto& e : enemyMoves)
{

    Piece* p =
        board.GetPiece(e.from);


    if(p &&
       p->GetType() == PieceType::Pawn)
    {

        if(p->GetColor()==Color::Black &&
           e.to.y == 5)
        {
            score -= 1000;
        }


        if(p->GetColor()==Color::White &&
           e.to.y == 2)
        {
            score -= 1000;
        }

    }

}


// ============================
// King avoid walk
// ============================

 
// ============================
// ลดคะแนนขุนเดินเล่น
// ============================

if(movingPiece &&
   movingPiece->GetType() == PieceType::King)
{
    score -= 80;
}
std::cout
<< "UNDO MOVE "
<< move.from.x << ","
<< move.from.y
<< " -> "
<< move.to.x << ","
<< move.to.y
<< std::endl;

        board.UndoTestMove(
            move,
            captured
        );
if(board.GetPiece(move.from)==nullptr)
{
    std::cout
    << "ERROR Undo lost piece at "
    << move.from.x << ","
    << move.from.y
    << std::endl;
}

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



        if(score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }

    }


    std::cout
        << "AI Best Score = "
        << bestScore
        << std::endl;


    std::cout
        << "FINAL BEST MOVE = "
        << bestMove.from.x << ","
        << bestMove.from.y
        << " -> "
        << bestMove.to.x << ","
        << bestMove.to.y
        << std::endl;


    return bestMove;
}
// ============================
// Minimax
// ============================


int AI::Minimax(
    Board& board,
    Color color,
    Color aiColor,
    int depth,
    int alpha,
    int beta){


    if(board.IsCheckmate(color))
{
    if(color == aiColor)
    {
        return -999999;   // AI แพ้
    }
    else
    {
        return 999999;    // ฝั่งตรงข้ามแพ้
    }
}



if(depth == 0)
{
    return Quiescence(
        board,
        color,
        alpha,
        beta
    );
}

  std::vector<LegalMove> moves =
    board.GenerateLegalMoves(color);

if(depth >= 2)
{
    std::sort(
        moves.begin(),
        moves.end(),
        [&](const LegalMove& a,
            const LegalMove& b)
        {
            return OrderScore(board, a, depth) >
       OrderScore(board, b, depth);
        }
    );
}  
 if(moves.empty())
{
    if(board.IsKingInCheck(color))
    {
        return color == aiColor
            ? -999999
            : 999999;
    }

    return board.Evaluate();
}


    int bestScore;



   if(color == aiColor)
{
    bestScore = -999999;

        for(auto move : moves)
        {

            Piece* captured =
                board.MakeTestMove(move);



           int score =
    Minimax(
        board,
        color == Color::White
            ? Color::Black
            : Color::White,
        aiColor,
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
    killerMove[depth] = move;
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
        color == Color::White
            ? Color::Black
            : Color::White,
        aiColor,
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
    killerMove[depth] = move;
    break;
}

        }

    }



    return bestScore;
}