#include "Board.h"
#include "SaveGame.h"
#include "AI.h"
#include "TestEngine.h"
#include "ChessGUI.h"

#include <iostream>


int main()
{
    Board board;


    // =========================
    // Test Engine
    // =========================
    TestEngine::RunAll();



    // =========================
    // Start GUI
    // =========================

    ChessGUI gui;


    if(gui.Init())
    {
        gui.Run(board);
    }


    return 0;
       
 }
 

std::cout
    << "Evaluate = "
    << board.Evaluate()
    << std::endl;
if(board.IsStalemate(Color::Black))
{
    std::cout << "STALEMATE PASS\n";
}
else
{
    std::cout << "NOT STALEMATE\n";
}



// แสดงหมากของฝ่ายขาว
std::vector<LegalMove> moves =
    board.GenerateLegalMoves(Color::White);

std::cout << "\nGENERATED MOVES\n";

for(const auto& move : moves)
{
    std::cout
        << move.from.x << ","
        << move.from.y
        << " -> "
        << move.to.x << ","
        << move.to.y
        << std::endl;
}
// หรือถ้าจะดูฝ่ายดำ
board.PrintLegalMoves(Color::Black);
board.GenerateLegalMoves(Color::White);

 while(true)
{
    board.Print();


   // =====================
// AI = Black
// =====================
if(board.GetTurn() == Color::Black)
{
    LegalMove aiMove =
        AI::FindBestMove(board, Color::Black);

    std::cout
    << "AI Move : "
    << aiMove.from.x << "," << aiMove.from.y
    << " -> "
    << aiMove.to.x << "," << aiMove.to.y
    << std::endl;


    if(board.Move(aiMove.from, aiMove.to))
    {
        std::cout << "AI MOVE OK\n";
    }
    else
    {
        std::cout << "AI MOVE FAIL\n";
        break;
    }

    continue;
}


    // =====================
    // Player = White only
    // =====================
    if(board.GetTurn() != Color::White)
    {
        continue;
    }


    char command;

    std::cout
    << "Command (m/u/r/h/s/l): ";

    std::cin >> command;
    // =====================
    // Undo
    // =====================

    if(command == 'u')
    {

        if(board.Undo())
        {
            std::cout << "Undo OK\n";
        }
        else
        {
            std::cout << "No move history\n";
        }

        continue;
    }

       // =====================
    // Khon
    // =====================

         
if(command == 'k')
{
    board.TestKhon();
    board.Print();

    continue;
 
}

       // =====================
    // Knight
    // =====================

if(command == 'n')
{
    board.TestKnight();
    board.Print();
    continue;
}

  // =====================
    // Rook
    // =====================
if(command == 'o')
{
    board.TestRook();
    board.Print();
    continue;
}

    // =====================
// Redo
// =====================

if(command == 'r')
{

    if(board.Redo())
    {
        std::cout << "Redo OK\n";
    }
    else
    {
        std::cout << "No redo history\n";
    }

    continue;
}

// =====================
// History
// =====================

if(command == 'h')
{

    board.PrintHistory();

    continue;

}


// =====================
// Save
// =====================

if(command == 's')
{

    if(SaveGame::Save(board,"save.txt"))
    {
        std::cout << "Save OK\n";
    }
    else
    {
        std::cout << "Save Failed\n";
    }

    continue;

}


// =====================
// Load
// =====================

if(command == 'l')
{

    if(SaveGame::Load(board,"save.txt"))
    {
        std::cout << "Load OK\n";
    }
    else
    {
        std::cout << "Load Failed\n";
    }

    continue;

}

    // =====================
    // Move
    // =====================

    if(command == 'm')
    {

        int fx,fy;
        int tx,ty;


        std::cout << "From (x y): ";

        std::cin >> fx >> fy;


        std::cout << "To (x y): ";

        std::cin >> tx >> ty;



        bool result = board.Move(
            Position{fx,fy},
            Position{tx,ty}
        );


        if(result)
        {

            if(board.IsGameOver())
            {
                std::cout << "Game Over\n";
                break;
            }


            std::cout << "Move OK\n";

        }

        else
        {
            std::cout << "Invalid Move\n";
        }
            
    }
}
 

    return 0;
}


