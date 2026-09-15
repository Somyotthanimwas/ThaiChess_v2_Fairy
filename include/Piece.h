#ifndef PIECE_H
#define PIECE_H

#include <string>


// ============================
// Forward Declaration
// ============================

class Board;

struct Position;


// ============================
// Color
// ============================

enum class Color
{
    White,
    Black
};


// ============================
// Piece Type
// ============================

enum class PieceType
{
    King,
    Rook,
    Knight,
    Khon,
    Met,
    Pawn
};


// ============================
// Piece Base Class
// ============================

class Piece
{

protected:

    std::string name;

    std::string symbol;

    Color color;

    PieceType type;


public:


    Piece(
        const std::string& name,
        const std::string& symbol,
        Color color
    );


    virtual ~Piece();



    // ============================
    // Name
    // ============================

    std::string GetName() const;


    std::string GetEnglishName() const;



    // ============================
    // Symbol
    // ============================

    std::string GetSymbol() const;


    std::string GetColoredSymbol() const;



    // ============================
    // Color / Type
    // ============================

    Color GetColor() const;


    PieceType GetType() const;



    // ============================
    // Movement Rule
    // ============================

    virtual bool CanMove(
        Board& board,
        Position from,
        Position to
    ) = 0;


};

#endif