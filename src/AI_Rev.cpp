#include "AI.h"
#include <algorithm>
#include <iostream>
#include <random> // For random number generation

// Initialize static members for Zobrist Hashing and Transposition Table
uint64_t AI::ZobristTable[NUM_PIECE_TYPES][NUM_SQUARES];
uint64_t AI::ZobristTurn;
bool AI::zobristKeysInitialized = false;
TTEntry AI::transpositionTable[1024 * 1024];

LegalMove AI::killerMoves[MAX_PLY][2];
int AI::historyScores[NUM_SQUARES][NUM_SQUARES];

// Helper function to compare LegalMove objects
bool AreMovesEqual(const LegalMove& a, const LegalMove& b) {
    return a.from.x == b.from.x && a.from.y == b.from.y &&
           a.to.x == b.to.x && a.to.y == b.to.y;
}

void AI::InitZobristKeys()
{
    if (zobristKeysInitialized) return;

    std::mt19937_64 rng(std::random_device{}());

    for (int i = 0; i < NUM_PIECE_TYPES; ++i)
    {
        for (int j = 0; j < NUM_SQUARES; ++j)
        {
            ZobristTable[i][j] = rng();
        }
    }
    ZobristTurn = rng();
    zobristKeysInitialized = true;
}

uint64_t AI::GetZobristHash(const Board& board)
{
    uint64_t hash = 0;
    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            Position pos = {i, j};
            Piece* piece = board.GetPiece(pos);
            if (piece != nullptr)
            {
                int pieceTypeIndex = static_cast<int>(piece->GetType());
                if (piece->GetColor() == Color::Black) {
                    pieceTypeIndex += 6;
                }
                hash ^= ZobristTable[pieceTypeIndex][i * 8 + j];
            }
        }
    }
    if (board.GetTurn() == Color::White) {
        hash ^= ZobristTurn;
    }
    return hash;
}

void AI::ResetSearchHeuristics()
{
    for (int i = 0; i < MAX_PLY; ++i)
    {
        AI::killerMoves[i][0] = LegalMove();
        AI::killerMoves[i][1] = LegalMove();
    }
    for (int i = 0; i < NUM_SQUARES; ++i)
    {
        for (int j = 0; j < NUM_SQUARES; ++j)
        {
            AI::historyScores[i][j] = 0;
        }
    }
}

// ============================
// AI Level
// ============================

int AI::level = 3;

void AI::SetLevel(int value)
{
    if(value < 1) value = 1;
    if(value > 5) value = 5;
    level = value;
    std::cout << "AI Level = " << level << std::endl;
}

// ============================
// Move Score (MVV-LVA)
// ============================

int AI::MoveScore(
    Board& board,
    const LegalMove& move,
    int ply,
    bool is_capture)
{
    Piece* attacker = board.GetPiece(move.from);
    Piece* victim = board.GetPiece(move.to);

    if(victim == nullptr) return 0;

    int attackerValue = 0;
    int victimValue = 0;

    auto getVal = [](PieceType type) {
        switch(type) {
            case PieceType::King:   return 10000;
            case PieceType::Rook:   return 500;
            case PieceType::Knight: return 350;
            case PieceType::Khon:   return 300;
            case PieceType::Met:    return 150;
            case PieceType::Pawn:   return 100;
            default:                return 0;
        }
    };

    attackerValue = getVal(attacker->GetType());
    victimValue = getVal(victim->GetType());

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

    if(color == Color::White)
    {
        if(standPat >= beta) return beta;
        if(standPat > alpha) alpha = standPat;
    }
    else
    {
        if(standPat <= alpha) return alpha;
        if(standPat < beta) beta = standPat;
    }

    auto moves = board.GenerateCaptureMoves(color);

    std::sort(
        moves.begin(),
        moves.end(),
        [&](const LegalMove& a, const LegalMove& b)
        {
            return MoveScore(board, a, 0, true) > MoveScore(board, b, 0, true);
        }
    );

    for(auto move : moves)
    {
        Piece* captured = board.MakeTestMove(move);
        int score = Quiescence(board, (color == Color::White ? Color::Black : Color::White), alpha, beta);
        board.UndoTestMove(move, captured);

        if(color == Color::White)
        {
            if(score > alpha) alpha = score;
        }
        else
        {
            if(score < beta) beta = score;
        }

        if(beta <= alpha) break;
    }

    return color == Color::White ? alpha : beta;
}

// ============================
// Find Best Move
// ============================

LegalMove AI::FindBestMove(
    Board& board,
    Color color)
{
    InitZobristKeys();
    ResetSearchHeuristics();

    LegalMove bestMove;
    int bestScore = (color == Color::White) ? -999999 : 999999;

    for (int currentDepth = 1; currentDepth <= level; ++currentDepth)
    {
        std::vector<LegalMove> moves = board.GenerateLegalMoves(color);

        std::sort(
            moves.begin(),
            moves.end(),
            [&](const LegalMove& a, const LegalMove& b)
            {
                int scoreA = MoveScore(board, a, 0, board.GetPiece(a.to) != nullptr);
                int scoreB = MoveScore(board, b, 0, board.GetPiece(b.to) != nullptr);

                uint64_t hash = GetZobristHash(board);
                int ttIndex = hash % (1024 * 1024);
                if (transpositionTable[ttIndex].hash == hash && AreMovesEqual(transpositionTable[ttIndex].bestMove, a)) scoreA += 100000;
                if (transpositionTable[ttIndex].hash == hash && AreMovesEqual(transpositionTable[ttIndex].bestMove, b)) scoreB += 100000;

                if (AreMovesEqual(a, killerMoves[0][0])) scoreA += 90000;
                else if (AreMovesEqual(a, killerMoves[0][1])) scoreA += 80000;
                if (AreMovesEqual(b, killerMoves[0][0])) scoreB += 90000;
                else if (AreMovesEqual(b, killerMoves[0][1])) scoreB += 80000;

                scoreA += historyScores[a.from.x * 8 + a.from.y][a.to.x * 8 + a.to.y];
                scoreB += historyScores[b.from.x * 8 + b.from.y][b.to.x * 8 + b.to.y];

                return scoreA > scoreB;
            }
        );

        if (moves.empty()) return bestMove;

        int currentBestScore = (color == Color::White) ? -999999 : 999999;
        LegalMove currentBestMove;

        for (auto move : moves)
        {
            Piece* captured = board.MakeTestMove(move);
            int score = Minimax(board, (color == Color::White ? Color::Black : Color::White), currentDepth - 1, -999999, 999999, 1);
            board.UndoTestMove(move, captured);

            if (color == Color::White)
            {
                if (score > currentBestScore) { currentBestScore = score; currentBestMove = move; }
            }
            else
            {
                if (score < currentBestScore) { currentBestScore = score; currentBestMove = move; }
            }
        }

        bestScore = currentBestScore;
        bestMove = currentBestMove;

        std::cout << "Depth " << currentDepth << ": Best Move " << bestMove.from.x << "," << bestMove.from.y << " -> " << bestMove.to.x << "," << bestMove.to.y << " Score = " << bestScore << std::endl;
    }

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
    int beta,
    int ply)
{
    uint64_t hash = GetZobristHash(board);
    int ttIndex = hash % (1024 * 1024);
    TTEntry& entry = transpositionTable[ttIndex];

    if (entry.hash == hash && entry.depth >= depth) {
        if (entry.flag == TT_FLAG_EXACT) return entry.score;
        if (entry.flag == TT_FLAG_ALPHA && entry.score <= alpha) return alpha;
        if (entry.flag == TT_FLAG_BETA && entry.score >= beta) return beta;
    }

    if(board.IsCheckmate(color)) return (color == Color::White ? -999999 + ply : 999999 - ply);
    if(depth == 0) return Quiescence(board, color, alpha, beta);

    std::vector<LegalMove> moves = board.GenerateLegalMoves(color);

    std::sort(
        moves.begin(),
        moves.end(),
        [&](const LegalMove& a, const LegalMove& b)
        {
            int scoreA = MoveScore(board, a, ply, board.GetPiece(a.to) != nullptr);
            int scoreB = MoveScore(board, b, ply, board.GetPiece(b.to) != nullptr);
            if (AreMovesEqual(a, killerMoves[ply][0])) scoreA += 90000;
            else if (AreMovesEqual(a, killerMoves[ply][1])) scoreA += 80000;
            if (AreMovesEqual(b, killerMoves[ply][0])) scoreB += 90000;
            else if (AreMovesEqual(b, killerMoves[ply][1])) scoreB += 80000;
            scoreA += historyScores[a.from.x * 8 + a.from.y][a.to.x * 8 + a.to.y];
            scoreB += historyScores[b.from.x * 8 + b.from.y][b.to.x * 8 + b.to.y];
            return scoreA > scoreB;
        }
    );

    if(moves.empty()) return board.Evaluate();

    int bestScore;
    LegalMove bestMoveThisSearch;
    TT_FLAG flag = TT_FLAG_ALPHA;

    if(color == Color::White)
    {
        bestScore = -999999;
        for(auto move : moves)
        {
            Piece* captured = board.MakeTestMove(move);
            int score = Minimax(board, Color::Black, depth - 1, alpha, beta, ply + 1);
            board.UndoTestMove(move, captured);

            if(score > bestScore) { bestScore = score; bestMoveThisSearch = move; }
            if(bestScore > alpha) { alpha = bestScore; flag = TT_FLAG_EXACT; }
            if(alpha >= beta)
            {
                if (!board.GetPiece(move.to)) { killerMoves[ply][1] = killerMoves[ply][0]; killerMoves[ply][0] = move; }
                historyScores[move.from.x * 8 + move.from.y][move.to.x * 8 + move.to.y] += depth;
                flag = TT_FLAG_BETA;
                break;
            }
        }
    }
    else
    {
        bestScore = 999999;
        for(auto move : moves)
        {
            Piece* captured = board.MakeTestMove(move);
            int score = Minimax(board, Color::White, depth - 1, alpha, beta, ply + 1);
            board.UndoTestMove(move, captured);

            if(score < bestScore) { bestScore = score; bestMoveThisSearch = move; }
            if(bestScore < beta) { beta = bestScore; flag = TT_FLAG_EXACT; }
            if(alpha >= beta)
            {
                if (!board.GetPiece(move.to)) { killerMoves[ply][1] = killerMoves[ply][0]; killerMoves[ply][0] = move; }
                historyScores[move.from.x * 8 + move.from.y][move.to.x * 8 + move.to.y] += depth;
                flag = TT_FLAG_ALPHA;
                break;
            }
        }
    }

    entry.hash = hash; entry.depth = depth; entry.score = bestScore; entry.flag = flag; entry.bestMove = bestMoveThisSearch;
    return bestScore;
}
