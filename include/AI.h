#ifndef AI_H
#define AI_H

#include "Board.h"
#include <string>
#include "Move.h"

#include <unordered_map>
#include <cstdint>


struct TTEntry
{
    uint64_t key;

    int depth;

    int score;

    LegalMove bestMove;
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

       std::string GetFEN() const;  

private:


    // ============================
    // Alpha Beta Search
    // ============================

    static int Search(
        Board& board,
        int depth,
        int alpha,
        int beta,
        Color color
    );



    // ============================
    // Minimax
    // ============================

    static int Minimax(
        Board& board,
        Color color,
        Color maximizingColor,
        int depth,
        int alpha,
        int beta
    );



    // ============================
    // Quiescence Search
    // ============================

    static int Quiescence(
        Board& board,
        Color color,
        int alpha,
        int beta
    );



    // ============================
    // Static Exchange Evaluation
    // ============================

    static int StaticExchange(
        Board& board,
        const LegalMove& move,
        Color color
    );



    // ============================
    // Evaluation
    // ============================

    static int Evaluate(
        Board& board,
        Color color
    );



    // ============================
    // Move Ordering
    // ============================

    static int MoveScore(
        Board& board,
        const LegalMove& move
    );


    static int OrderScore(
        Board& board,
        const LegalMove& move,
        int depth
    );



    // ============================
    // Transposition Table Hash
    // ============================

    static uint64_t Hash(
        Board& board
    );



private:


    // ============================
    // Transposition Table
    // ============================

    static std::unordered_map<uint64_t, TTEntry> table;



    // ============================
    // Killer Move Heuristic
    // ============================

    static LegalMove killerMove[20];



    // ============================
    // History Heuristic
    // ============================

    static int history[64][64];

};


#endif