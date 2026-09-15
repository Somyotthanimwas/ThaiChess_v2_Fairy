#include "ChessGUI.h"
#include "FairyEngine.h"
#include "Board.h"
#include "Piece.h"
#include "SaveGame.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>


ChessGUI::ChessGUI()
{
    std::cout
        << "### CHESSGUI CONSTRUCTOR ###"
        << std::endl;

    window = nullptr;
    renderer = nullptr;
    font = nullptr;

    trainingMode = false;
    selected = false;
    aiThinking = false;

    std::cout
        << "START FAIRY ENGINE"
        << std::endl;

    bool result = fairy.Start();

    std::cout
        << "FAIRY START RESULT = "
        << result
        << std::endl;
}
ChessGUI::~ChessGUI()
{
    if(font)
        TTF_CloseFont(font);

    for(int i = 0; i < 12; i++)
    {
        if(pieceTexture[i])
            SDL_DestroyTexture(pieceTexture[i]);
    }

    if(renderer)
        SDL_DestroyRenderer(renderer);

    if(window)
        SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();
}

// << เพิ่มตรงนี้  ตัวหมากรุกไทย แบบใหม่ >>

SDL_Texture* ChessGUI::LoadTexture(const std::string& file)
{
    SDL_Surface* surface = IMG_Load(file.c_str());

    if (!surface)
    {
        std::cout << "Load failed : " << file << std::endl;
        return nullptr;
    }

    SDL_Texture* tex =
    SDL_CreateTextureFromSurface(renderer, surface);

SDL_SetTextureBlendMode(
    tex,
    SDL_BLENDMODE_BLEND
);

SDL_FreeSurface(surface);

return tex;
}

// แล้วค่อยเป็นฟังก์ชันอื่น

// =====================
// Check Effect
// =====================

void ChessGUI::UpdateCheckBlink(Board& board)
{
    Uint32 now = SDL_GetTicks();


    // เกิด CHECK ใหม่ครั้งแรก
    if(board.inCheck && !lastInCheck)
    {
        checkBlink = true;
        checkBlinkTimer = now;
    }


    // ไม่มี CHECK
    if(!board.inCheck)
    {
        checkBlink = false;
        checkBlinkTimer = 0;
    }


    // กระพริบ
    if(board.inCheck)
    {
        if(now - checkBlinkTimer > 400)
        {
            checkBlink = !checkBlink;
            checkBlinkTimer = now;
        }
    }


    lastInCheck = board.inCheck;
}
bool ChessGUI::Init()
{

    if(SDL_Init(SDL_INIT_VIDEO) < 0)


    {
        std::cout
        << "SDL Init Error\n";

        return false;
    }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
{
    std::cout
        << "IMG Init Error : "
        << IMG_GetError()
        << std::endl;

    return false;
}


    // ---------------------------
    // เพิ่มตรงนี้
    // ---------------------------

    if(TTF_Init() == -1)
    {
        std::cout
        << "TTF Init Error : "
        << TTF_GetError()
        << std::endl;

        return false;
    }

    window = SDL_CreateWindow(
        "Somyot Thai Chess",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1000,
        800,
        SDL_WINDOW_SHOWN
    );

    if(!window)
        return false;

    renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if(!renderer)
    return false;


// ---------------------------
// Load Font
// ---------------------------
font = TTF_OpenFont(
    "assets/fonts/DejaVuSans.ttf",
    48
);

if(font == nullptr)
{
    std::cout
        << "Font Error : "
        << TTF_GetError()
        << std::endl;

    return false;
}


// Turn Font
turnFont = TTF_OpenFont(
    "assets/fonts/DejaVuSans.ttf",
    24
);

if(turnFont == nullptr)
{
    std::cout
        << "Turn Font Error : "
        << TTF_GetError()
        << std::endl;

    return false;
}// =========================
// AI Level Buttons
// =========================

for(int i = 0; i < 5; i++)
{
    levelButton[i].x = 830 + (i * 35);
    levelButton[i].y = 480;

    levelButton[i].w = 35;
    levelButton[i].h = 35;
}

// =========================
// Menu Buttons
// =========================

newGameButton = {830, 40, 140, 40};
saveButton    = {830,100, 140, 40};
loadButton    = {830,160, 140, 40};
undoButton    = {830,220, 140, 40};
redoButton    = {830,280, 140, 40};


// =========================
// Load Thai Chess Pieces
// =========================
pieceTexture[0] = LoadTexture("assets/pieces/wk.png");
pieceTexture[1] = LoadTexture("assets/pieces/bk.png");

pieceTexture[2] = LoadTexture("assets/pieces/wr.png");
pieceTexture[3] = LoadTexture("assets/pieces/br.png");

pieceTexture[4] = LoadTexture("assets/pieces/wn.png");
pieceTexture[5] = LoadTexture("assets/pieces/bn.png");

pieceTexture[6] = LoadTexture("assets/pieces/wp.png");
pieceTexture[7] = LoadTexture("assets/pieces/bp.png");

pieceTexture[8] = LoadTexture("assets/pieces/wc.png");
pieceTexture[9] = LoadTexture("assets/pieces/bc.png");

pieceTexture[10] = LoadTexture("assets/pieces/wm.png");
pieceTexture[11] = LoadTexture("assets/pieces/bm.png");

return true;
}
 
// ============================
// Highlight Selected Square
// ============================

void ChessGUI::DrawHighlight()
{
    if(!selected)
        return;


    int size = 100;


    SDL_Rect rect;

    rect.x = selectedPos.x * size;
    rect.y = selectedPos.y * size;
    rect.w = size;
    rect.h = size;


    // สีเหลืองโปร่งแสง
     

SDL_SetRenderDrawBlendMode(
    renderer,
    SDL_BLENDMODE_BLEND
);

SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

SDL_RenderDrawRect(renderer, &rect);

SDL_Rect rect2 = {
    rect.x - 1,
    rect.y - 1,
    rect.w + 2,
    rect.h + 2
};

SDL_RenderDrawRect(renderer, &rect2);

SDL_RenderDrawRect(
    renderer,
    &rect
);

    // กลับค่าเดิม
    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_NONE
    );
}
// ============================
// Draw Possible Moves
// ============================

void ChessGUI::DrawPossibleMoves()
{
    int size = 100;


    for(auto pos : possibleMoves)
    {

        SDL_Rect rect;


        rect.x = pos.x * size + 40;
        rect.y = pos.y * size + 40;
        rect.w = 20;
        rect.h = 20;


        SDL_SetRenderDrawColor(
            renderer,
            0,
            255,
            0,
            255
        );


        SDL_RenderFillRect(
            renderer,
            &rect
        );

    }
}
// ============================
// Draw Capture Moves
// ============================

void ChessGUI::DrawCaptureMoves()
{
    int size = 100;


    for(auto pos : captureMoves)
    {

        SDL_Rect rect;

        rect.x = pos.x * size + 40;
        rect.y = pos.y * size + 40;
        rect.w = 20;
        rect.h = 20;


        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255
        );


        SDL_RenderFillRect(
            renderer,
            &rect
        );

    }
}


// ============================
// Find Possible Moves
// ============================

void ChessGUI::FindPossibleMoves(Board& board)
{
    possibleMoves.clear();
        
        captureMoves.clear();

    Piece* piece =
        board.GetPiece(selectedPos);


    if(piece == nullptr)
        return;


    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {

          Position to{x,y};


// ตรวจว่าช่องปลายทางมีหมากหรือไม่
Piece* target =
    board.GetPiece(to);


// ห้ามกินพวกเดียวกัน
if(target != nullptr &&
   target->GetColor() == piece->GetColor())
{
    continue;
}

if(piece->CanMove(
        board,
        selectedPos,
        to))
{

    if(target != nullptr)
    {
        captureMoves.push_back(to);
    }
    else
    {
        possibleMoves.push_back(to);
    }

}
        }
    }
}

// ============================
// Draw Board
// ============================

void ChessGUI::DrawBoard()
{
    int size = 100;


    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {

            SDL_Rect rect;

            rect.x = x * size;
            rect.y = y * size;
            rect.w = size;
            rect.h = size;


            if((x+y)%2 == 0)
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    240,
                    217,
                    181,
                    255
                );
            }
            else
            {
                SDL_SetRenderDrawColor(
                    renderer,
                    181,
                    136,
                    99,
                    255
                );
            }


            SDL_RenderFillRect(
                renderer,
                &rect
            );
       }
    }

    // ============================
    // AI Last Move
    // ============================
//กรอบสีดำ 1 ชั้น


if(hasLastAIMove)
{
    SDL_SetRenderDrawColor(renderer,0,0,0,255);

    SDL_Rect fromRect =
    {
        lastAIMoveFrom.x * size + 2,
        lastAIMoveFrom.y * size + 2,
        size - 4,
        size - 4
    };

    SDL_RenderDrawRect(renderer,&fromRect);


    SDL_Rect toRect =
    {
        lastAIMoveTo.x * size + 2,
        lastAIMoveTo.y * size + 2,
        size - 4,
        size - 4
    };

    SDL_RenderDrawRect(renderer,&toRect);


//กรอบสีดำ 2 ชั้น
/*
    if(hasLastAIMove)
{
   // SDL_SetRenderDrawColor(renderer,255,255,255,255);

SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

SDL_Rect fromRect;

fromRect.x = lastAIMoveFrom.x * size + 2;
fromRect.y = lastAIMoveFrom.y * size + 2;
fromRect.w = size - 4;
fromRect.h = size - 4;

SDL_RenderDrawRect(renderer, &fromRect);
   SDL_Rect toRect;

toRect.x = lastAIMoveTo.x * size + 2;
toRect.y = lastAIMoveTo.y * size + 2;
toRect.w = size - 4;
toRect.h = size - 4;

SDL_RenderDrawRect(renderer, &toRect);
}
*/
}   // ปิด if(hasLastAIMove)

}   // <<< ปิด DrawBoard()

// ============================
// Draw Pieces
// ============================

void ChessGUI::DrawPieces(Board& board)
{
    int size = 100;

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            Piece* piece =
                board.GetPiece(Position{x,y});

            if(piece == nullptr)
                continue;
// ============================
// Check King Blink
// ============================
if(board.inCheck &&
   checkBlink &&
   piece->GetType() == PieceType::King &&
   piece->GetColor() == board.GetTurn())
{
    SDL_SetRenderDrawColor(
        renderer,
        255,
        0,
        0,
        150
    );

    SDL_Rect rect;

    rect.x = x * size;
    rect.y = y * size;
    rect.w = size;
    rect.h = size;

    SDL_RenderFillRect(
        renderer,
        &rect
    );
}

  // ============================
   // Select Piece Texture
    // ============================

            SDL_Texture* texture = nullptr;


            if(piece->GetType() == PieceType::King)
            {
                texture = (piece->GetColor() == Color::White)
                    ? pieceTexture[0]
                    : pieceTexture[1];
            }

            else if(piece->GetType() == PieceType::Rook)
            {
                texture = (piece->GetColor() == Color::White)
                    ? pieceTexture[2]
                    : pieceTexture[3];
            }

            else if(piece->GetType() == PieceType::Knight)
            {
                texture = (piece->GetColor() == Color::White)
                    ? pieceTexture[4]
                    : pieceTexture[5];
            }

            else if(piece->GetType() == PieceType::Pawn)
            {
                texture = (piece->GetColor() == Color::White)
                    ? pieceTexture[6]
                    : pieceTexture[7];
            }

            else if(piece->GetType() == PieceType::Khon)
            {
                texture = (piece->GetColor() == Color::White)
                    ? pieceTexture[8]
                    : pieceTexture[9];
            }

            else if(piece->GetType() == PieceType::Met)
            {
                texture = (piece->GetColor() == Color::White)
                    ? pieceTexture[10]
                    : pieceTexture[11];
            } 


// ============================
// Draw Piece Texture
// ============================

if(texture != nullptr)
{
    SDL_Rect dst;

    dst.x = x * size;
    dst.y = y * size;
    dst.w = size;
    dst.h = size;

    SDL_RenderCopy(
        renderer,
        texture,
        nullptr,
        &dst
    );
}

        }
    }
}


// ============================
// Draw Turn
// ============================

void ChessGUI::DrawTurn(Board& board)
{
    std::string text;

    if(board.GetTurn() == Color::White)
    {
        text = "White Turn";
    }
    else
    {
        text = "Black Turn";
    }

 SDL_Color color =
{
    255,
    255,
    255,
    255
};

    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(
            turnFont,
            text.c_str(),
            color
        );

    if(surface == nullptr)
        return;

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    if(texture == nullptr)
    {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst;

    dst.x = 830;
    dst.y = 550;
    dst.w = surface->w;
    dst.h = surface->h;

    SDL_RenderCopy(
        renderer,
        texture,
        nullptr,
        &dst
    );

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

 

// ============================
// Event
// ============================

void ChessGUI::HandleEvent(bool& running, Board& board)
{
    SDL_Event event;


    while(SDL_PollEvent(&event))
    {

        if(event.type == SDL_QUIT)
        {
            running = false;
        }


  // =========================
// Keyboard
// =========================

if(event.type == SDL_KEYDOWN)
{

    // UNDO
    if(event.key.keysym.sym == SDLK_u)
    {
        std::cout
            << "UNDO"
            << std::endl;

        board.Undo();
    }


    // SAVE
    if(event.key.keysym.sym == SDLK_s)
    {
        if(SaveGame::Save(board,"save.txt"))
        {
            std::cout
                << "SAVE OK"
                << std::endl;
        }
        else
        {
            std::cout
                << "SAVE FAILED"
                << std::endl;
        }
    }


    // LOAD
    if(event.key.keysym.sym == SDLK_l)
    {
        if(SaveGame::Load(board,"save.txt"))
        {
            std::cout
                << "LOAD OK"
                << std::endl;
        }
        else
        {
            std::cout
                << "LOAD FAILED"
                 << std::endl;
                 
          }     
        }
    }
 

// =========================
// Mouse
// =========================

if(event.type == SDL_MOUSEBUTTONDOWN)
{

    std::cout
        << "Mouse Pixel : "
        << event.button.x
        << ","
        << event.button.y
        << std::endl;


  // =========================
// Menu Area
// =========================

if(event.button.x >= 800)
{
    // NEW GAME
    if(event.button.x >= newGameButton.x &&
   event.button.x <= newGameButton.x + newGameButton.w &&
   event.button.y >= newGameButton.y &&
   event.button.y <= newGameButton.y + newGameButton.h)
{
    board.NewGame();

    selected = false;
    aiThinking = false;

    possibleMoves.clear();
    captureMoves.clear();

    std::cout
        << "NEW GAME OK"
        << std::endl;

    return;
}

    // SAVE
    if(event.button.x >= saveButton.x &&
       event.button.x <= saveButton.x + saveButton.w &&
       event.button.y >= saveButton.y &&
       event.button.y <= saveButton.y + saveButton.h)
    {
        SaveGame::Save(board,"save.txt");
        std::cout << "SAVE Ok" << std::endl;
        return;
    }

    // LOAD
    if(event.button.x >= loadButton.x &&
       event.button.x <= loadButton.x + loadButton.w &&
       event.button.y >= loadButton.y &&
       event.button.y <= loadButton.y + loadButton.h)
    {
        SaveGame::Load(board,"save.txt");
        std::cout << "LOAD OK" << std::endl;
        return;
    }

    // UNDO
    if(event.button.x >= undoButton.x &&
       event.button.x <= undoButton.x + undoButton.w &&
       event.button.y >= undoButton.y &&
       event.button.y <= undoButton.y + undoButton.h)
    {
        board.Undo();
        std::cout << "UNDO" << std::endl;
        return;
    }

    // REDO
    if(event.button.x >= redoButton.x &&
       event.button.x <= redoButton.x + redoButton.w &&
       event.button.y >= redoButton.y &&
       event.button.y <= redoButton.y + redoButton.h)
    {
        board.Redo();
        std::cout << "REDO" << std::endl;
        return;
    }



    // AI LEVEL

for(int i = 0; i < 5; i++)
{
    if(event.button.x >= levelButton[i].x &&
       event.button.x <= levelButton[i].x + levelButton[i].w &&
       event.button.y >= levelButton[i].y &&
       event.button.y <= levelButton[i].y + levelButton[i].h)
    {

        aiLevel = i + 1;


        std::cout
            << "AI LEVEL = "
            << aiLevel
            << std::endl;


        return;
    }
}

    return;
}

    int x = event.button.x / 100;
    int y = event.button.y / 100;


    std::cout
        << "CLICK : "
        << x
        << ","
        << y
        << std::endl;


    if(!selected)
    {

        Piece* piece =
            board.GetPiece(
                Position{x,y}
            );


        if(piece == nullptr)
        {
            std::cout
                << "NO PIECE"
                << std::endl;

            return;
        }


        if(piece->GetColor() != board.GetTurn())
        {
            std::cout
                << "NOT YOUR TURN"
                << std::endl;

            return;
        }


        selectedPos =
            Position{x,y};


        selected = true;


        FindPossibleMoves(board);


        std::cout
            << "SELECT PIECE"
            << std::endl;

    }
    else
    {

        Position to{x,y};


        bool result =
            board.Move(
                selectedPos,
                to
            );


        if(result)
        {
            selected = false;

            possibleMoves.clear();
            captureMoves.clear();


            if(board.GetTurn() == Color::Black)
            {
                aiThinking = true;
            }
        }
        else
        {
            std::cout
                << "INVALID MOVE"
                << std::endl;

            selected = false;
        }

    }

}
    } // while SDL_PollEvent

} // HandleEvent


 
 
// ============================// 
// Run
// ============================

void ChessGUI::Run(Board& board)
{
    bool running = true;


    while(running)
    {

        SDL_SetRenderDrawColor(
            renderer,
            0,0,0,255
        );


        SDL_RenderClear(renderer);

  // =========================
  // UPDATE CHECK EFFECT
  // =========================
     UpdateCheckBlink(board);


      DrawBoard();

DrawPieces(board);

if(trainingMode)
{
    DrawPossibleMoves();
    DrawCaptureMoves();
}

DrawMenu();

DrawPieces(board);

DrawHighlight();

DrawTurn(board);

SDL_RenderPresent(renderer);

 // =========================
// PLAYER EVENT
// =========================

HandleEvent(
    running,
    board
);

// =========================
// Refresh after player move
// =========================

if(aiThinking)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    DrawBoard();

    if(trainingMode)
    {
        DrawPossibleMoves();
        DrawCaptureMoves();
    }

    DrawHighlight();

DrawMenu();

DrawPieces(board);

DrawTurn(board);

SDL_RenderPresent(renderer);

}

// =========================
// AI TURN
// =========================

if(aiThinking)
{
    std::cout
        << "AI Thinking..."
        << std::endl;


    std::string fen =
        board.GetFEN();
       
       // DEBUG FEN
board.Print();

std::cout
    << "DEBUG FEN = "
    << fen
    << std::endl;

    int depth = 3;


    switch(aiLevel)
    {
        case 1:
            depth = 3;
            break;

        case 2:
            depth = 5;
            break;

        case 3:
            depth = 8;
            break;

        case 4:
            depth = 12;
            break;

        case 5:
            depth = 18;
            break;

        default:
            depth = 5;
            break;
    }


    std::cout
        << "AI LEVEL = "
        << aiLevel
        << " DEPTH = "
        << depth
        << std::endl;



    std::string best =
        fairy.GetBestMove(
            fen,
            depth
        );


    std::cout
        << "Fairy bestmove = "
        << best
        << std::endl;



    if(best.length() >= 4)
    {
        Position from;
        Position to;


      // Fairy coordinate -> ThaiChess coordinate

 

from.x = best[0] - 'a';
from.y = 7 - (best[1] - '1');


to.x = best[2] - 'a';
to.y = 7 - (best[3] - '1');

std::cout
    << "CONVERT MOVE : "
    << from.x << "," << from.y
    << " -> "
    << to.x << "," << to.y
    << std::endl;


        std::cout
            << "AI MOVE : "
            << from.x
            << ","
            << from.y
            << " -> "
            << to.x
            << ","
            << to.y
            << std::endl;
  
// ============================
// DEBUG AI SOURCE PIECE
// ============================

Piece* p = board.GetPiece(from);

if(p)
{
    std::cout
        << "AI PIECE = "
        << p->GetEnglishName()
        << " COLOR="
        << (p->GetColor()==Color::White ? "White" : "Black")
        << std::endl;
}
else
{
    std::cout
        << "AI SOURCE EMPTY"
        << std::endl;
}


// ============================


       bool moved =
    board.Move(
        from,
        to
    );

if(!moved)
{
    std::cout
        << "AI MOVE FAILED"
        << std::endl;
}
else
{
    lastAIMoveFrom = from;
    lastAIMoveTo   = to;
    hasLastAIMove  = true;

    std::cout
        << "AI MOVE SUCCESS"
        << std::endl;

    board.Print();
}

} // ปิด if(best.length() >= 4)

aiThinking = false;

} // ปิด if(aiThinking)

SDL_Delay(10);

} // ปิด while

} // ปิด ChessGUI::Run()

// ============================
// Draw Menu
// ============================

void ChessGUI::DrawMenu()
{
    // พื้นหลังเมนูด้านขวา
    SDL_Rect menu;

    menu.x = 800;
    menu.y = 0;
    menu.w = 200;
    menu.h = 800;

    SDL_SetRenderDrawColor(
        renderer,
        70,
        70,
        70,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &menu
    );

// New Game
DrawButton(
    newGameButton,
    SDL_Color{70,120,220,255}
);



// Save
DrawButton(
    saveButton,
    SDL_Color{60,170,90,255}
);

// Load
DrawButton(
    loadButton,
    SDL_Color{220,170,50,255}
);

// Undo
DrawButton(
    undoButton,
    SDL_Color{180,120,60,255}
);

// Redo
DrawButton(
    redoButton,
    SDL_Color{180,80,180,255}
);

// =========================
// Button Text
// =========================

DrawText(
    "NEW GAME",
    newGameButton.x + 20,
    newGameButton.y + 10,
    14
);


DrawText(
    "SAVE",
    saveButton.x + 45,
    saveButton.y + 10,
    14
);


DrawText(
    "LOAD",
    loadButton.x + 45,
    loadButton.y + 10,
    14
);


DrawText(
    "UNDO",
    undoButton.x + 40,
    undoButton.y + 10,
    14
);


DrawText(
    "REDO",
    redoButton.x + 40,
    redoButton.y + 10,
    14
);

// =========================
// AI LEVEL
// =========================

DrawText(
    "AI LEVEL",
    830,
    450,
    14
);


for(int i = 0; i < 5; i++)
{
    DrawButton(
        levelButton[i],
        SDL_Color{100,100,100,255}
    );


    DrawText(
        std::to_string(i+1),
        levelButton[i].x + 8,
        levelButton[i].y + 6,
        14
    );
}

}

// ============================
// Draw Button
// ============================

void ChessGUI::DrawButton(
    SDL_Rect rect,
    SDL_Color color
)
{
    SDL_SetRenderDrawColor(
        renderer,
        color.r,
        color.g,
        color.b,
        color.a
    );

    SDL_RenderFillRect(
        renderer,
        &rect
    );
}
// ============================
// Draw Text
// ============================

void ChessGUI::DrawText(
    const std::string& text,
    int x,
    int y,
    int size
)
{
    SDL_Color color =
    {
        255,
        255,
        255,
        255
    };

    TTF_Font* useFont =
        TTF_OpenFont(
            "assets/fonts/DejaVuSans.ttf",
            size
        );

    if(useFont == nullptr)
    {
        std::cout
            << "Font Error : "
            << TTF_GetError()
            << std::endl;

        return;
    }

    SDL_Surface* surface =
        TTF_RenderUTF8_Blended(
            useFont,
            text.c_str(),
            color
        );

    if(surface == nullptr)
    {
        TTF_CloseFont(useFont);
        return;
    }

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_Rect dst;

    dst.x = x;
    dst.y = y;
    dst.w = surface->w;
    dst.h = surface->h;

    SDL_RenderCopy(
        renderer,
        texture,
        nullptr,
        &dst
    );

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(useFont);
}