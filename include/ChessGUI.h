#ifndef CHESS_GUI_H
#define CHESS_GUI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <string>

#include "AI.h"
#include "FairyEngine.h"
#include "Position.h"


class Board;


class ChessGUI
{
public:

    ChessGUI();

    ~ChessGUI();


    bool Init();

        SDL_Texture* LoadTexture(const std::string& file);

    void Run(Board& board);



private:

    SDL_Window* window;
    SDL_Renderer* renderer;

    TTF_Font* font;
    TTF_Font* turnFont;
         SDL_Texture* pieceTexture[12] = { nullptr };

    // =====================
    // AI LEVEL
    // =====================

    int aiLevel = 3;


    SDL_Rect levelButton[5];


    SDL_Rect newGameButton;
    SDL_Rect saveButton;
    SDL_Rect loadButton;
    SDL_Rect undoButton;
    SDL_Rect redoButton;



    // =====================
    // Fairy Engine
    // =====================

    FairyEngine fairy;



    // =====================
    // Old AI (เก็บไว้ก่อน)
    // =====================

   AI ai;

 

    // =====================
    // Mouse Selection
    // =====================

    bool selected;

    Position selectedPos;



    bool trainingMode;


    bool aiThinking;

// แจ้ง AI เดินจากไหนไปไหน

Position lastAIMoveFrom;
Position lastAIMoveTo;
bool hasLastAIMove = false;


// เพิ่มตรงนี้
    Uint32 checkBlinkTimer = 0;
    bool checkBlink = false;
        bool lastInCheck = false;

    std::vector<Position> possibleMoves;

    std::vector<Position> captureMoves;



    // =====================
    // Draw
    // =====================

    void DrawBoard();

    void DrawHighlight();

    void DrawPossibleMoves();

    void DrawCaptureMoves();

        void UpdateCheckBlink(
    Board& board
);

    void FindPossibleMoves(
        Board& board
    );


    void DrawMenu();


    void DrawButton(
        SDL_Rect rect,
        SDL_Color color
    );


    void DrawText(
        const std::string& text,
        int x,
        int y,
        int size
    );



    // =====================
    // Event
    // =====================

    void HandleEvent(
        bool& running,
        Board& board
    );



    // =====================
    // Pieces / Turn
    // =====================

    void DrawPieces(
        Board& board
    );


    void DrawTurn(
        Board& board
    );

};

#endif