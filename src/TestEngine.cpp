#include "TestEngine.h"
#include "Board.h"
#include "King.h"
#include "Rook.h"
#include "Met.h"
#include "Pawn.h"


#include <iostream>

void TestEngine::RunAll()
{
    std::cout << "\n===== THAI CHESS TEST =====\n";

TestPawn();
TestRook();
TestKnight();
TestKhon();
TestMet();
TestKing();
TestUndo();
TestRedo();

TestPromotion();
TestCheck();
TestCheckmate();
TestCaptureCheck();
TestBlockCheck();
//TestStalemate();


}

void TestEngine::TestPawn()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Pawn\n";
    std::cout << "=========================\n";

    int pass = 0;
    int total = 0;

    // --------------------------
    // Test 1
    // --------------------------
    {
        Board board;

                 board.Print();

        total++;
        std::cout << "\nTest 1 : White Pawn Forward\n";

        if(board.Move({0,6},{0,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }

    // --------------------------
// Test 2
// --------------------------
{
    Board board;

    // เดินเบี้ยไปก่อน
    board.Move({0,6},{0,5});

    total++;

    std::cout 
        << "\nTest 2 : White Pawn Backward\n";

    // ลองเดินกลับ
    if(!board.Move({0,5},{0,6}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

    // --------------------------
    // Test 3
    // --------------------------
    {
        Board board;

        total++;
        std::cout << "\nTest 3 : White Pawn Sideways\n";

        if(!board.Move({0,6},{1,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }

  
// --------------------------
// Test 4 : Capture
// --------------------------
{
    Board board;


    // ขยับเบี้ยขาวออก
    board.Move({0,6},{0,5});


    // ข้ามตาขาว ให้ดำเดิน
    board.Move({1,1},{1,2});


    // ข้ามตาดำ ให้ขาวเดิน
    board.Move({7,6},{7,5});


    // ดำเดินต่อ
    board.Move({1,2},{1,3});


    // ขาวเดิน
    board.Move({7,5},{7,4});


    // ดำเดินต่อ
    board.Move({1,3},{1,4});


    total++;

    std::cout
        << "\nTest 4 : White Pawn Capture\n";


    // ขาวกินเฉียง
    if(board.Move({0,5},{1,4}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}
    // --------------------------
    // Summary
    // --------------------------

    std::cout
        << "\nPawn : "
        << pass
        << " / "
        << total
        << " PASS\n";
}

void TestEngine::TestRook()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Rook\n";
    std::cout << "=========================\n";

    int pass = 0;
    int total = 0;


    // Test 1
    {
        Board board;

        total++;

        std::cout
        << "\nTest 1 : Rook Vertical Move\n";


        // White เปิดทาง
        board.Move({0,6},{0,5});

        // Black เดินคั่น
        board.Move({1,1},{1,2});


        if(board.Move({0,7},{0,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // Test 2
{
    Board board;

    total++;

    std::cout
    << "\nTest 2 : Rook Horizontal Move\n";


    // เอาเรือออกจากแถว
    board.Move({0,6},{0,5});

    // ดำเดิน
    board.Move({1,1},{1,2});


    // เปิดทางให้เรือด้วยการให้ม้าขาวเดิน
    board.Move({1,7},{2,5});


    // ดำเดินคั่น
    board.Move({2,1},{2,2});


    // เรือเดินแนวนอน
    if(board.Move({0,7},{1,7}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}



    // Test 3
    {
        Board board;

        total++;

        std::cout
        << "\nTest 3 : Rook Blocked\n";


        if(!board.Move({0,7},{0,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



   // --------------------------
// Test 4 : Rook Capture
// --------------------------
{
    Board board;

    total++;

    std::cout
    << "\nTest 4 : Rook Capture\n";


    // เอาเบี้ยขาวหน้าเรือออก
    board.SetPiece(
        {0,6},
        nullptr
    );


    // วางหมากดำให้เรือกิน
    board.SetPiece(
        {0,5},
        new Pawn(Color::Black)
    );


    board.SetTurn(Color::White);


    // เรือขาวกินเบี้ยดำ
    if(board.Move({0,7},{0,5}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}


    std::cout
    << "\nRook : "
    << pass
    << " / "
    << total
    << " PASS\n";
}

void TestEngine::TestKnight()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Knight\n";
    std::cout << "=========================\n";

    int pass = 0;
    int total = 0;


    // Test 1 : เดิน L
    {
        Board board;

        total++;

        std::cout
        << "\nTest 1 : Knight L Move\n";


        if(board.Move({1,7},{2,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // Test 2 : กระโดดข้ามหมาก
    {
        Board board;

        total++;

        std::cout
        << "\nTest 2 : Knight Jump\n";


        if(board.Move({1,7},{2,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // Test 3 : เดินผิดรูป
    {
        Board board;

        total++;

        std::cout
        << "\nTest 3 : Knight Invalid Move\n";


        if(!board.Move({1,7},{1,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }


// --------------------------
// Test 4 : Knight Capture
// --------------------------
{
    Board board;

    total++;

    std::cout
    << "\nTest 4 : Knight Capture\n";


    // ม้าขาวเริ่มที่ 1,7
    // วางหมากดำตำแหน่งที่ม้ากินได้

    board.SetPiece(
        {2,5},
        new Pawn(Color::Black)
    );


    board.SetTurn(Color::White);


    // ม้าขาว 1,7 -> 2,5
    if(board.Move({1,7},{2,5}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

    std::cout
    << "\nKnight : "
    << pass
    << " / "
    << total
    << " PASS\n";
}
void TestEngine::TestKhon()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Khon\n";
    std::cout << "=========================\n";

    int pass = 0;
    int total = 0;


   // Test 1 : Khon Forward
{
    Board board;

    total++;

    std::cout
    << "\nTest 1 : Khon Forward\n";


    // เปิดทางเบี้ยขาว
    board.Move({2,6},{2,5});

    // ให้ดำเดิน
    board.Move({0,1},{0,2});


    if(board.Move({2,7},{2,6}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

// Test 2 : Khon Diagonal
{
    Board board;

    total++;

    std::cout
    << "\nTest 2 : Khon Diagonal\n";


    // เปิดทางเบี้ยที่หน้าโคน
    board.Move({3,6},{3,5});

    // ดำเดิน
    board.Move({0,1},{0,2});


    if(board.Move({2,7},{3,6}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}    // Test 3 : เดินไกลเกินไป
    {
        Board board;

        total++;

        std::cout
        << "\nTest 3 : Khon Long Move\n";


        if(!board.Move({2,7},{4,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }


    // --------------------------
// --------------------------
// Test 4 : Khon Capture
// --------------------------
{
    Board board;

    total++;

    std::cout
    << "\nTest 4 : Khon Capture\n";


    // โคนขาวอยู่ 2,7

    // วางหมากดำเฉียงหน้า
    board.SetPiece(
        {3,6},
        new Pawn(Color::Black)
    );


    board.SetTurn(Color::White);


    // โคนกิน
    if(board.Move({2,7},{3,6}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

// สรุปผล Khon ต้องอยู่นอก block
std::cout
    << "\nKhon : "
    << pass
    << " / "
    << total
    << " PASS\n";

}  

 // <-- ปิด void TestEngine::TestKhon()

void TestEngine::TestMet()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Met\n";
    std::cout << "=========================\n";

    int pass = 0;
    int total = 0;

// ==========================
// Test 1 : Met Diagonal Move
// ==========================
{
    Board board;

    total++;

    std::cout
    << "\nTest 1 : Met Diagonal\n";


    // ลบเบี้ยขาวที่อยู่เฉียงหน้าเม็ด
    // เดิมตำแหน่ง (3,6)
    board.SetPiece(
        {3,6},
        nullptr
    );


    // ตรวจสอบ (เอาไว้ debug ชั่วคราว)
    if(board.GetPiece({3,6}) == nullptr)
    {
        std::cout << "Position 3,6 EMPTY\n";
    }
    else
    {
        std::cout
        << "Position 3,6 = "
        << board.GetPiece({3,6})->GetName()
        << "\n";
    }


    // เม็ดขาว เดินเฉียง 1 ช่อง
    // 4,7 -> 3,6
    if(board.Move({4,7},{3,6}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}
   // ==========================
// Test 2 : Met Capture
// ==========================
{
    Board board;

    total++;

    std::cout
    << "\nTest 2 : Met Capture\n";


    // วางหมากดำเฉียงหน้าเม็ด
    board.SetPiece(
        {3,6},
        new Pawn(Color::Black)
    );


    board.SetTurn(Color::White);


    // เม็ดขาว 4,7 กิน 3,6
    if(board.Move({4,7},{3,6}))
    {
        pass++;
        std::cout << "[PASS]\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}


    // ==========================
    // Test 3 : Met Straight
    // ==========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 3 : Met Straight\n";


        // เม็ดห้ามเดินตรง
        if(!board.Move({4,7},{4,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // ==========================
    // Test 4 : Met Long Move
    // ==========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 4 : Met Long Move\n";


        // เม็ดห้ามเดินไกล
        if(!board.Move({4,7},{2,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    std::cout
        << "\nMet : "
        << pass
        << " / "
        << total
        << " PASS\n";
}
void TestEngine::TestKing()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : King\n";
    std::cout << "=========================\n";


    int pass = 0;
    int total = 0;


    // ==========================
    // Test 1 : King One Step
    // ==========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 1 : King One Step\n";


        // ขุนขาว 3,7 -> 3,6
        board.SetPiece(
            {3,6},
            nullptr
        );


        if(board.Move({3,7},{3,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // ==========================
    // Test 2 : King Long Move
    // ==========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 2 : King Long Move\n";


        if(!board.Move({3,7},{3,5}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // ==========================
    // Test 3 : King Diagonal
    // ==========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 3 : King Diagonal\n";


        board.SetPiece(
            {2,6},
            nullptr
        );


        if(board.Move({3,7},{2,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // ==========================
    // Test 4 : King Capture
    // ==========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 4 : King Capture\n";


        // วางหมากดำให้ขุนกิน
        board.SetPiece(
            {3,6},
            new Pawn(Color::Black)
        );


        if(board.Move({3,7},{3,6}))
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    std::cout
    << "\nKing : "
    << pass
    << " / "
    << total
    << " PASS\n";
}void TestEngine::TestPromotion()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Promotion\n";
    std::cout << "=========================\n";


    Board board;

    board.TestPromotion();

    board.Print();


    std::cout << "\nPromotion Test\n";


    board.SetTurn(Color::White);

    board.Move({0,4},{0,3});
    board.SetTurn(Color::White);

    board.Move({0,3},{0,2});
    board.SetTurn(Color::White);

    board.Move({0,2},{0,1});
    board.SetTurn(Color::White);


    if(board.Move({0,1},{0,0}))
    {
        std::cout << "[PASS] Promotion\n";
    }
    else
    {
        std::cout << "[FAIL] Promotion\n";
    }


    board.Print();
}



void TestEngine::TestCheck()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Check\n";
    std::cout << "=========================\n";


    Board board;


    board.Clear();


    board.SetPiece(
        Position{4,0},
        new King(Color::Black)
    );


    board.SetPiece(
        Position{0,0},
        new Rook(Color::White)
    );


    if(board.IsKingInCheck(Color::Black))
    {
        std::cout << "[PASS] Black King CHECK\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

void TestEngine::TestCheckmate()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Checkmate\n";
    std::cout << "=========================\n";

    Board board;

    board.Clear();


    // ขุนดำมุม
    board.SetPiece(
        {7,7},
        new King(Color::Black)
    );


    // ขุนขาว (ต้องมี)
    board.SetPiece(
        {5,5},
        new King(Color::White)
    );


    // เรือขาวรุก
    board.SetPiece(
        {7,5},
        new Rook(Color::White)
    );


    // ปิดช่องหนี 6,6
    board.SetPiece(
        {6,6},
        new Rook(Color::White)
    );


    // ปิดช่อง 6,7
    board.SetPiece(
        {6,7},
        new Met(Color::White)
    );


    // คุ้มกันเม็ด
    board.SetPiece(
        {5,6},
        new Rook(Color::White)
    );


    board.SetTurn(Color::Black);


    board.Print();


    if(board.IsCheckmate(Color::Black))
    {
        std::cout << "[PASS] Black Checkmate\n";
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

// ============================
// Test Capture Check
// ============================

void TestEngine::TestCaptureCheck()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Capture Check\n";
    std::cout << "=========================\n";


    Board board;

    board.Clear();


    // ขุนดำ
    board.SetPiece(
        {7,7},
        new King(Color::Black)
    );


    // ขุนขาว
    board.SetPiece(
        {5,5},
        new King(Color::White)
    );


    // เรือขาวให้รุก
    board.SetPiece(
        {7,5},
        new Rook(Color::White)
    );


    // เม็ดดำสามารถกินเรือได้
    board.SetPiece(
        {6,6},
        new Met(Color::Black)
    );


    board.SetTurn(Color::Black);


    board.Print();


    if(board.CanCaptureCheckingPiece(Color::Black))
    {
        std::cout
        << "[PASS] Can Capture Checking Piece\n";
    }
    else
    {
        std::cout
        << "[FAIL]\n";
    }


    if(!board.IsCheckmate(Color::Black))
    {
        std::cout
        << "[PASS] Not Checkmate\n";
    }
    else
    {
        std::cout
        << "[FAIL] Still Checkmate\n";
    }
}


// ============================
// Test Block Check
// ============================

void TestEngine::TestBlockCheck()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Block Check\n";
    std::cout << "=========================\n";

    Board board;

    board.Clear();


    board.SetPiece(
        {7,7},
        new King(Color::Black)
    );


    board.SetPiece(
        {0,0},
        new King(Color::White)
    );


    board.SetPiece(
        {7,5},
        new Rook(Color::White)
    );


    board.SetPiece(
        {6,5},
        new Met(Color::Black)
    );


    board.SetTurn(Color::Black);


    board.Print();


    if(board.CanBlockCheck(Color::Black))
    {
        std::cout << "[PASS] Can Block Check\n";
    }
    else
    {
        std::cout << "[FAIL] Cannot Block Check\n";
    }


    if(!board.IsCheckmate(Color::Black))
    {
        std::cout << "[PASS] Not Checkmate\n";
    }
    else
    {
        std::cout << "[FAIL] Still Checkmate\n";
    }
}


// ============================
// Test Stalemate
// ============================

void TestEngine::TestStalemate()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Stalemate\n";
    std::cout << "=========================\n";


    Board board;


    board.TestStalemate();


    board.Print();


    if(board.IsStalemate(Color::Black))
    {
        std::cout << "[PASS] Stalemate\n";
    }
    else
    {
        std::cout << "[FAIL] Not Stalemate\n";
    }
}

void TestEngine::TestUndo()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Undo\n";
    std::cout << "=========================\n";


    int pass = 0;
    int total = 0;


    // =========================
    // Test 1 : Undo Move
    // =========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 1 : Undo Normal Move\n";


        // เบี้ยขาวเดิน
        board.Move({0,6},{0,5});


        // Undo
        if(board.Undo())
        {
            Piece* piece =
                board.GetPiece({0,6});


            if(piece != nullptr &&
               piece->GetName() == "เบี้ย")
            {
                pass++;
                std::cout << "[PASS]\n";
            }
            else
            {
                std::cout << "[FAIL]\n";
            }
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



// =========================
// Test 2 : Undo Capture
// =========================
{
    Board board;

    total++;

    std::cout
    << "\nTest 2 : Undo Capture\n";


    // เบี้ยขาวเดินก่อน
    board.Move(
        {0,6},
        {0,5}
    );


    // วางเบี้ยดำตำแหน่งให้ถูกกิน
    board.SetPiece(
        {1,4},
        new Pawn(Color::Black)
    );


    board.SetTurn(Color::White);


    // เบี้ยขาวกิน
    board.Move(
        {0,5},
        {1,4}
    );


    // Undo
    if(board.Undo())
    {
        Piece* whitePawn =
            board.GetPiece({0,5});


        Piece* blackPawn =
            board.GetPiece({1,4});


        if(whitePawn != nullptr &&
           blackPawn != nullptr)
        {
            pass++;
            std::cout << "[PASS]\n";
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }
    else
    {
        std::cout << "[FAIL]\n";
    }
}

    std::cout
    << "\nUndo : "
    << pass
    << " / "
    << total
    << " PASS\n";
}

void TestEngine::TestRedo()
{
    std::cout << "\n=========================\n";
    std::cout << "TEST : Redo\n";
    std::cout << "=========================\n";


    int pass = 0;
    int total = 0;



    // =========================
    // Test 1 : Redo Normal Move
    // =========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 1 : Redo Normal Move\n";


        // เดิน
        board.Move(
            {0,6},
            {0,5}
        );


        // Undo
        board.Undo();


        // Redo
        if(board.Redo())
        {
            Piece* piece =
                board.GetPiece({0,5});


            if(piece != nullptr &&
               piece->GetName() == "เบี้ย")
            {
                pass++;
                std::cout << "[PASS]\n";
            }
            else
            {
                std::cout << "[FAIL]\n";
            }
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    // =========================
    // Test 2 : Redo Capture
    // =========================
    {
        Board board;

        total++;

        std::cout
        << "\nTest 2 : Redo Capture\n";


        // เปิดทาง
        board.Move(
            {0,6},
            {0,5}
        );


        // วางหมากดำ
        board.SetPiece(
            {1,4},
            new Pawn(Color::Black)
        );


        board.SetTurn(Color::White);


        // กิน
        board.Move(
            {0,5},
            {1,4}
        );


        // Undo
        board.Undo();


        // Redo
        if(board.Redo())
        {
            Piece* piece =
                board.GetPiece({1,4});


            if(piece != nullptr &&
               piece->GetColor() == Color::White)
            {
                pass++;
                std::cout << "[PASS]\n";
            }
            else
            {
                std::cout << "[FAIL]\n";
            }
        }
        else
        {
            std::cout << "[FAIL]\n";
        }
    }



    std::cout
    << "\nRedo : "
    << pass
    << " / "
    << total
    << " PASS\n";
}