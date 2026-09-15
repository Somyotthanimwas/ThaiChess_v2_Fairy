#ifndef AI_H
#define AI_H

#include "Board.h"
#include "Move.h"
#include <cstdint> // For uint64_t

// Define constants based on error messages and common chess AI practices
#define MAX_PLY 64 // Maximum search depth for killer moves
#define NUM_SQUARES 64 // Number of squares on the board (8x8)
#define NUM_PIECE_TYPES 12 // 6 piece types * 2 colors

// Transposition Table Flags
enum TT_FLAG {
    TT_FLAG_EXACT, // Exact score
    TT_FLAG_ALPHA, // Upper bound (fail-low)
    TT_FLAG_BETA   // Lower bound (fail-high)
};

// Transposition Table Entry structure
struct TTEntry {
    uint64_t hash;      // Zobrist hash of the position
    int depth;          // Depth searched to get this entry
    int score;          // Score of the position
    TT_FLAG flag;       // Type of score (exact, alpha, beta)
    LegalMove bestMove; // Best move found from this position
};

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
        int beta,
        int ply
    );

    static int MoveScore(
        Board& board,
        const LegalMove& move,
        int ply,
        bool is_capture
    );

    static int Quiescence(
        Board& board,
        Color color,
        int alpha,
        int beta
    );

    static void ResetSearchHeuristics();

    // Zobrist Hashing and Transposition Table declarations
    static void InitZobristKeys();
    static uint64_t GetZobristHash(const Board& board);
    static TTEntry transpositionTable[1024 * 1024];

private:

    static LegalMove killerMoves[MAX_PLY][2];
    static int historyScores[NUM_SQUARES][NUM_SQUARES];

    static uint64_t ZobristTable[NUM_PIECE_TYPES][NUM_SQUARES];
    static uint64_t ZobristTurn;
    static bool zobristKeysInitialized;

};

#endif
