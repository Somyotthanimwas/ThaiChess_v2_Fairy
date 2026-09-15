#ifndef BOARD_H
#define BOARD_H


#include "Position.h"
#include "Piece.h"

#include <vector>
#include <string>
#include "Move.h"


class Board
{

private:

    Piece* board[8][8];

    Position kingPosition[2];

    Color currentTurn;

    bool gameOver;

        int moveNumber;   // เพิ่มตรงนี้


    std::vector<MoveRecord> history;
    std::vector<MoveRecord> redoHistory;


public:

        bool inCheck = false;

    Board();

    ~Board();


    void Init();


    void Print();


    bool Move(
        Position from,
        Position to
    );


    bool Undo();

    bool Redo();

    void PrintHistory() const;


    Piece* GetPiece(
        Position pos
    ) const;


    void Clear();


    void SetPiece(
        Position pos,
        Piece* piece
    );


    Color GetTurn() const;

    void SetTurn(Color color);


    bool IsGameOver() const;


    // Test

    void TestCapture();

    void TestPromotion();

    void TestCheck();

    void TestCheckmate();

    void TestCaptureCheck();

    void TestKhon();

    void TestKnight();

    void TestRook();

    void TestMet();

    void TestKing();

    void TestBlockCheck();

    void TestKnightCheck();

    void TestKnightCheckmate();

    void TestStalemate();

    void TestAI();

    void TestAI_Gain();

    void NewGame();



    // ============================
    // Check system
    // ============================

    bool IsKingInCheck(Color color);

    bool IsCheckmate(Color color);

    bool CanEscapeCheck(Color color);

    bool CanCaptureCheckingPiece(Color color);

    bool CanBlockCheck(Color color);

    bool IsStalemate(Color color);



    // ============================
    // Attack System
    // ============================

    bool IsSquareAttacked(
        Position pos,
        Color attacker
    );


    int PieceValue(
        PieceType type
    );


    int ThreatScore(
        Piece* piece,
        Position pos
    );



    // ============================
    // Legal Moves
    // ============================

    void PrintLegalMoves(Color color);


    std::vector<LegalMove> GenerateLegalMoves(
        Color color
    );


    std::vector<LegalMove> GenerateCaptureMoves(
        Color color
    );



    int Evaluate();


    std::string GetFEN() const;



    int MobilityScore(
        Piece* piece,
        Position pos
    );



    Piece* MakeTestMove(
        LegalMove move
    );


    void UndoTestMove(
        LegalMove move,
        Piece* captured
    );



    // ============================
    // History Access
    // ============================

    const std::vector<MoveRecord>& GetHistory() const
    {
        return history;
    }


    // จำนวนครั้งที่มีการเดินจริง
    int GetMoveCount() const
    {
        return static_cast<int>(history.size());
    }


};


#endif