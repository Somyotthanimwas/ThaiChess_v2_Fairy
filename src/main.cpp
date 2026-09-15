#ifdef _WIN32
#include <windows.h>
#endif

#include "Board.h"
#include "SaveGame.h"
#include "AI.h"
#include "TestEngine.h"

#include "ChessGUI.h"

#include <iostream>

int main()
{

 #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    Board board;


    // =========================
    // Test Engine
    // =========================

    TestEngine::RunAll();



    // =========================
    // Start GUI
    // =========================

    ChessGUI gui;


    if(!gui.Init())
    {
        std::cout
            << "GUI Init Failed\n";

        return -1;
    }


    gui.Run(board);



    return 0;
}