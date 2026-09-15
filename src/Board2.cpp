#include "Board.h"


#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "King.h"
#include "Khon.h"
#include "Met.h"


#include <iostream>



Board::Board()
{
    currentTurn = Color::White;

    gameOver = false;

    Init();
}


Board::~Board()
{
    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
        }
    }
}



void Board::Init()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            board[y][x] = nullptr;
        }
    }


    // ===== White =====

    board[0][0] = new Rook(Color::White);
    board[0][1] = new Knight(Color::White);
    board[0][2] = new Khon(Color::White);
    board[0][3] = new Met(Color::White);
    board[0][4] = new King(Color::White);
    board[0][5] = new Khon(Color::White);
    board[0][6] = new Knight(Color::White);
    board[0][7] = new Rook(Color::White);


    for(int x=0;x<8;x++)
    {
        board[1][x] = new Pawn(Color::White);
    }



    // ===== Black =====


    for(int x=0;x<8;x++)
    {
        board[6][x] = new Pawn(Color::Black);
    }


    board[7][0] = new Rook(Color::Black);
    board[7][1] = new Knight(Color::Black);
    board[7][2] = new Khon(Color::Black);
    board[7][3] = new Met(Color::Black);
    board[7][4] = new King(Color::Black);
    board[7][5] = new Khon(Color::Black);
    board[7][6] = new Knight(Color::Black);
    board[7][7] = new Rook(Color::Black);

}



void Board::Print()
{

    std::cout
    << "----------------\n";


    for(int y=0;y<8;y++)
    {

        for(int x=0;x<8;x++)
        {

            if(board[y][x])
            {
                std::cout
                << board[y][x]->GetSymbol()
                << " ";
            }
            else
            {
                std::cout
                << ". ";
            }

        }

        std::cout << "\n";
    }


    std::cout
    << "----------------\n";

}



bool Board::Move(
    Position from,
    Position to
)
{

    if(gameOver)
{
    std::cout << "Move Fail : gameOver" << std::endl;
    return false;
}

    Piece* piece = board[from.y][from.x];

    Piece* capturedPiece = board[to.y][to.x];

    // ห้ามกินขุน
if(capturedPiece != nullptr &&
   capturedPiece->GetType() == PieceType::King)
{
    return false;
}

    if(piece == nullptr)
{
    std::cout << "Move Fail : piece == nullptr" << std::endl;
    return false;
}


    if(piece->GetColor() != currentTurn)
{
    std::cout << "Move Fail : wrong turn" << std::endl;
    return false;
}


    if(!piece->CanMove(
    *this,
    from,
    to))
{
    std::cout << "Move Fail : CanMove false" << std::endl;
    return false;
}


 
if(capturedPiece)
{
    if(capturedPiece->GetColor() == piece->GetColor())
    {
        return false;
    }
}



    board[to.y][to.x] = piece;

     board[from.y][from.x] = nullptr;

        
// ตรวจว่าหลังเดินแล้วขุนตัวเองโดนรุกหรือไม่

if(IsKingInCheck(piece->GetColor()))
{
    // คืนตำแหน่งเดิม

    board[from.y][from.x] = piece;
    board[to.y][to.x] = capturedPiece;

    return false;
}

// บันทึกการเดินที่ถูกต้อง
MoveRecord record;

record.from = from;
record.to = to;
record.movedPiece = piece;
record.capturedPiece = capturedPiece;
record.movedName = piece->GetName();


if(capturedPiece != nullptr)
{
    record.capturedName = capturedPiece->GetName();
}


// เดินใหม่หลัง Undo ต้องล้าง Redo
redoHistory.clear();




// ===== Promotion หมากรุกไทย =====

if(piece->GetType() == PieceType::Pawn)
{

    // เบี้ยขาวหงายเมื่อถึงแถว 5
    if(piece->GetColor() == Color::White &&
       to.y == 5)
    {
        delete board[to.y][to.x];
        board[to.y][to.x] = new Met(Color::White);
    }


    // เบี้ยดำหงายเมื่อถึงแถว 2
    if(piece->GetColor() == Color::Black &&
       to.y == 2)
    {
        delete board[to.y][to.x];
        board[to.y][to.x] = new Met(Color::Black);
    }

}
// เก็บประวัติหลัง Promotion
history.push_back(record);


    if(currentTurn == Color::White)
{
    currentTurn = Color::Black;
}
else
{
    currentTurn = Color::White;
}

// แจ้งเตือนถ้าฝ่ายที่กำลังจะเดินโดนรุก

if(IsKingInCheck(currentTurn))
{
    std::cout << "CHECK!" << std::endl;

    if(IsCheckmate(currentTurn))
    {
        std::cout << "CHECKMATE!" << std::endl;
        std::cout << "GAME OVER" << std::endl;

        gameOver = true;
    }
}


return true;

}

Piece* Board::GetPiece(
    Position pos
) const
{
    return board[pos.y][pos.x];
}

// ============================
// Clear Board
// ============================

void Board::Clear()
{

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            delete board[y][x];

            board[y][x] = nullptr;
        }
    }

}


// ============================
// Set Piece
// ============================

void Board::SetPiece(
    Position pos,
    Piece* piece
)
{

    board[pos.y][pos.x] = piece;

}

Color Board::GetTurn() const
{
    return currentTurn;
}
void Board::SetTurn(Color color)
{
    currentTurn = color;
}

bool Board::IsGameOver() const
{
    return gameOver;
}


void Board::TestPromotion()
{
    // ล้างกระดาน
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }

    // วางเบี้ยขาวไว้ก่อนถึงแถวสุดท้าย 1 ช่อง
    board[4][0] = new Pawn(Color::White);

         // เบี้ยดำ
    board[3][1] = new Pawn(Color::Black);    

    currentTurn = Color::White;

}
// ============================
// Test Khon
// ============================

void Board::TestKhon()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // โคนขาว
    board[5][0] = new Khon(Color::White);


    // โคนดำ
    board[2][7] = new Khon(Color::Black);


    currentTurn = Color::White;

}
// ============================
// Test Knight
// ============================

void Board::TestKnight()
{
    for(int y=0; y<8; y++)
    {
        for(int x=0; x<8; x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }

    // ม้าขาว
    board[5][5] = new Knight(Color::White);

    // ม้าดำ
    board[2][7] = new Knight(Color::Black);

    currentTurn = Color::White;
}
// ============================
// Test Rook
// ============================

void Board::TestRook()
{
    // ล้างกระดาน
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }

    // เรือขาว
    board[5][0] = new Rook(Color::White);

    // เบี้ยขาวขวาง
    board[5][3] = new Pawn(Color::White);

    // เรือดำ
    board[2][7] = new Rook(Color::Black);

    currentTurn = Color::White;
}

// ============================
// Test Capture
// ============================

void Board::TestCapture()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // เรือขาว
    board[3][4] = new Rook(Color::White);


    // เบี้ยดำ
    board[1][4] = new Pawn(Color::Black);


    currentTurn = Color::White;

}// ============================
// Test Check
// ============================

void Board::TestCheck()
{

    // ล้างกระดาน

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำ
    board[7][4] = new King(Color::Black);


    // เรือขาว
    board[4][4] = new Rook(Color::White);


    // ให้ขาวเริ่ม

    currentTurn = Color::White;

}


// ============================
// Check System
// ============================

bool Board::IsKingInCheck(Color color)
{
    Position kingPos{-1,-1};


    // หา King ของฝ่ายที่ต้องการตรวจ
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            Piece* piece = board[y][x];

            if(piece != nullptr)
            {
                if(piece->GetType() == PieceType::King &&
                   piece->GetColor() == color)
                {
                    kingPos.x = x;
                    kingPos.y = y;
                }
            }
        }
    }


    // ไม่พบขุน
    if(kingPos.x == -1)
    {
        return false;
    }



    // ตรวจว่ามีหมากฝ่ายตรงข้ามโจมตีขุนหรือไม่
    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {

            Piece* piece = board[y][x];


            if(piece != nullptr &&
               piece->GetColor() != color)
            {

                if(piece->CanMove(
                    *this,
                    Position{x,y},
                    kingPos))
                {
                    return true;
                }

            }

        }
    }


    return false;
}

// ============================
// Test Escape Check
// ============================

bool Board::CanEscapeCheck(Color color)
{

    // หา King

    Position king{-1,-1};


    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {

            Piece* piece = board[y][x];

            if(piece != nullptr &&
               piece->GetType() == PieceType::King &&
               piece->GetColor() == color)
            {
                king.x = x;
                king.y = y;
            }

        }
    }


    if(king.x == -1)
    {
        return false;
    }



    // ทดลองเดินขุน 8 ทิศ

    int dx[8] =
    {
        -1,0,1,
        -1,1,
        -1,0,1
    };


    int dy[8] =
    {
        -1,-1,-1,
        0,0,
        1,1,1
    };



    Piece* kingPiece = board[king.y][king.x];


    for(int i=0;i<8;i++)
    {

        Position to;

        to.x = king.x + dx[i];
        to.y = king.y + dy[i];


        if(to.x < 0 || to.x >= 8 ||
           to.y < 0 || to.y >= 8)
        {
            continue;
        }



        // ห้ามกินหมากฝ่ายเดียวกัน

        if(board[to.y][to.x] != nullptr &&
           board[to.y][to.x]->GetColor() == color)
        {
            continue;
        }



        Piece* oldTarget = board[to.y][to.x];



        // ย้ายขุนทดลอง

        board[to.y][to.x] = kingPiece;
        board[king.y][king.x] = nullptr;



        // ตรวจว่าช่องใหม่ปลอดภัยหรือไม่

        bool safe = !IsKingInCheck(color);



        // คืนกระดาน

        board[king.y][king.x] = kingPiece;
        board[to.y][to.x] = oldTarget;



        if(safe)
        {
            return true;
        }

    }


    return false;

}



// ============================
// Can Capture Checking Piece
// ============================

bool Board::CanCaptureCheckingPiece(Color color)
{

    // หา King
    Position kingPos{-1,-1};


    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            Piece* piece = board[y][x];

            if(piece != nullptr &&
               piece->GetType() == PieceType::King &&
               piece->GetColor() == color)
            {
                kingPos.x = x;
                kingPos.y = y;
            }
        }
    }


    if(kingPos.x == -1)
    {
        return false;
    }



    // หาเฉพาะตัวรุก
    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {

            Piece* enemy = board[y][x];


            if(enemy == nullptr)
                continue;


            if(enemy->GetColor() == color)
                continue;



            if(enemy->CanMove(
                *this,
                Position{x,y},
                kingPos))
            {

                // เจอตัวรุกแล้ว

                for(int yy=0;yy<8;yy++)
                {
                    for(int xx=0;xx<8;xx++)
                    {

                        Piece* ally = board[yy][xx];


                        if(ally == nullptr)
                            continue;


                        if(ally->GetColor()!=color)
                            continue;



                        Position from{xx,yy};
                        Position to{x,y};



                        if(ally->CanMove(
                            *this,
                            from,
                            to))
                        {

                            Piece* captured = board[to.y][to.x];


                            // ทดลองกิน
                            board[to.y][to.x] = ally;
                            board[from.y][from.x] = nullptr;



                            bool safe =
                                !IsKingInCheck(color);



                            // คืนกระดาน
                            board[from.y][from.x] = ally;
                            board[to.y][to.x] = captured;



                            if(safe)
                            {
                                return true;
                            }

                        }

                    }
                }
            }
        }
    }


    return false;
}


// ============================
// Can Block Check
// ============================

bool Board::CanBlockCheck(Color color)
{
    Position kingPos{-1,-1};

    // หาขุน
    for(int y=0; y<8; y++)
    {
        for(int x=0; x<8; x++)
        {
            Piece* piece = board[y][x];

            if(piece != nullptr &&
               piece->GetType() == PieceType::King &&
               piece->GetColor() == color)
            {
                kingPos = {x,y};
            }
        }
    }

    if(kingPos.x == -1)
    {
        return false;
    }

   

// หาตัวรุก
Position checker{-1,-1};

for(int y=0; y<8; y++)
{
    for(int x=0; x<8; x++)
    {
        Piece* piece = board[y][x];

        if(piece != nullptr &&
           piece->GetColor() != color)
        {
            if(piece->CanMove(
                    *this,
                    Position{x,y},
                    kingPos))
            {
                checker = {x,y};
            }
        }
    }
}

if(checker.x == -1)
{
    return false;
}

Piece* checkerPiece = board[checker.y][checker.x];

if(checkerPiece->GetType() != PieceType::Rook)
{
    return false;
}

// ขั้นต่อไปจะหาช่องระหว่างเรือกับขุน

int dx = 0;
int dy = 0;

if(checker.x == kingPos.x)
{
    dy = (kingPos.y > checker.y) ? 1 : -1;
}
else if(checker.y == kingPos.y)
{
    dx = (kingPos.x > checker.x) ? 1 : -1;
}
else
{
    // ไม่ได้อยู่แนวเดียวกัน
    return false;
}


Position block;

block.x = checker.x + dx;
block.y = checker.y + dy;

while(block.x != kingPos.x ||
      block.y != kingPos.y)
{

    // ทดลองว่ามีหมากฝ่ายเราเดินมาบังได้ไหม

    for(int y=0; y<8; y++)
    {
        for(int x=0; x<8; x++)
        {

            Piece* ally = board[y][x];

            if(ally == nullptr)
                continue;

            if(ally->GetColor() != color)
                continue;

            if(ally->GetType() == PieceType::King)
                continue;

            if(ally->CanMove(
                    *this,
                    Position{x,y},
                    block))
            {
                return true;
            }

        }
    }

    block.x += dx;
    block.y += dy;
}

return false;
}

// ============================
// Checkmate System
// ============================

bool Board::IsCheckmate(Color color)
{

    if(!IsKingInCheck(color))
    {
        return false;
    }


    if(CanEscapeCheck(color))
    {
        return false;
    }


    if(CanCaptureCheckingPiece(color))
    {
        return false;
    }
    
    
 // มีหมากตัวอื่นมาขวางการรุกได้
    if(CanBlockCheck(color))
    {
        return false;
    }

    return true;

}

// ============================
// Test Checkmate
// ============================

void Board::TestCheckmate()
{

    // ล้างกระดาน

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำมุม
    board[7][7] = new King(Color::Black);


    // เรือขาวให้รุกแนวตั้ง
    board[5][7] = new Rook(Color::White);


    // เรือขาวปิดช่องหนี
    board[6][6] = new Rook(Color::White);


    // ให้ขาวเดิน

    currentTurn = Color::White;

}

// ============================
// Test Capture Check
// ============================

void Board::TestCaptureCheck()
{
    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำ
    board[7][4] = new King(Color::Black);


    // เรือขาวรุกขุนดำ
    board[5][4] = new Rook(Color::White);


    // ม้าดำกินเรือ
    board[7][2] = new Knight(Color::Black);


    currentTurn = Color::White;
}
// ============================
// Test Block Check
// ============================

void Board::TestBlockCheck()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำ
    board[7][4] = new King(Color::Black);


    // เรือขาวรุกแนวตั้ง
    board[4][4] = new Rook(Color::White);


    // หมากดำที่จะเอามาบัง
    board[6][3] = new Khon(Color::Black);


    currentTurn = Color::Black;

}

// ============================
// Test Knight Check
// ============================

void Board::TestKnightCheck()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำ
    board[7][4] = new King(Color::Black);


    // ม้าขาวตำแหน่งให้รุกขุนดำ
    board[5][5] = new Knight(Color::White);


    currentTurn = Color::White;

}

// ============================
// Test Knight Checkmate
// ============================

void Board::TestKnightCheckmate()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำมุม
    board[7][7] = new King(Color::Black);


    // ม้าขาวรุก
    board[5][6] = new Knight(Color::White);


    // ปิดช่องหนีด้วยเรือขาว
    board[6][6] = new Rook(Color::White);
    board[6][7] = new Rook(Color::White);


    currentTurn = Color::White;

}
// ============================
// Test AI
// ============================

void Board::TestAI()
{
    Clear();


    // White
    SetPiece(
        Position{5,5},
        new King(Color::White)
    );


    SetPiece(
        Position{4,5},
        new Rook(Color::White)
    );


    // Black
    SetPiece(
        Position{7,7},
        new King(Color::Black)
    );


    SetTurn(Color::Black);


    std::cout << "TEST AI POSITION\n";
}
// ============================
// Test AI Gain Material
// ============================

void Board::TestAI_Gain()
{
    Clear();


    // White
    SetPiece(
        Position{5,5},
        new King(Color::White)
    );


    SetPiece(
        Position{5,6},
        new Rook(Color::White)
    );


    // Black
    SetPiece(
        Position{5,7},
        new King(Color::Black)
    );


    SetTurn(Color::Black);


    std::cout
        << "TEST AI GAIN POSITION\n";
}

// ============================
// Stalemate System
// ============================

bool Board::IsStalemate(Color color)
{

    // ถ้าโดนรุก ไม่ใช่ Stalemate
   if(IsKingInCheck(color))
{
    std::cout << "KING IS CHECKED\n";
    return false;
}


    // ตรวจว่ามีหมากตัวไหนเดินได้หรือไม่

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {

            Piece* piece = board[y][x];


            if(piece != nullptr &&
               piece->GetColor() == color)
            {


                for(int ty = 0; ty < 8; ty++)
                {
                    for(int tx = 0; tx < 8; tx++)
                    {

                        Position from{x,y};
                        Position to{tx,ty};


if(piece->CanMove(
    *this,
    from,
    to))
{
    std::cout
        << "CAN MOVE: "
        << piece->GetName()
        << " FROM "
        << x << "," << y
        << " TO "
        << tx << "," << ty
        << std::endl;

    return false;
}

                    }
                }

            }

        }
    }


    return true;

}

// ============================
// Test Stalemate
// ============================

void Board::TestStalemate()
{

    for(int y=0;y<8;y++)
    {
        for(int x=0;x<8;x++)
        {
            delete board[y][x];
            board[y][x] = nullptr;
        }
    }


    // ขุนดำ
    board[7][7] = new King(Color::Black);


    // ขุนขาว
    board[5][5] = new King(Color::White);


    // เม็ดขาว ปิดช่อง
    board[6][7] = new Met(Color::White);


    // โคนขาว ปิดอีกช่อง
    board[7][6] = new Khon(Color::White);


    currentTurn = Color::Black;

}

// ============================
// Print Legal Moves
// ============================

void Board::PrintLegalMoves(Color color)
{
    std::cout << "\nLEGAL MOVES\n";

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            Piece* piece = board[y][x];

            if(piece != nullptr &&
               piece->GetColor() == color)
            {
                std::cout
                    << piece->GetName()
                    << " (" << x << "," << y << ")\n";

                for(int ty = 0; ty < 8; ty++)
                {
                    for(int tx = 0; tx < 8; tx++)
                    {
                        if(piece->CanMove(
                                *this,
                                Position{x,y},
                                Position{tx,ty}))
                        {
                            std::cout
                                << "   -> "
                                << tx << ","
                                << ty
                                << "\n";
                        }
                    }
                }
            }
        }
    }
}

// ============================
// Evaluate Board
// ============================

int Board::Evaluate() const
{
    int score = 0;

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            Piece* piece = board[y][x];

            if(piece == nullptr)
            {
                continue;
            }

            int value = 0;

            switch(piece->GetType())
            {
                case PieceType::King:
                    value = 10000;
                    break;

                case PieceType::Rook:
                    value = 500;
                    break;

                case PieceType::Knight:
                    value = 300;
                    break;

                case PieceType::Khon:
                    value = 250;
                    break;

                case PieceType::Met:
                    value = 150;
                    break;

                case PieceType::Pawn:
                    value = 100;
                    break;

                default:
                    value = 0;
                    break;
            }

            if(piece->GetColor() == Color::White)
            {
                score += value;
            }
            else
            {
                score -= value;
            }
        }
    }

    return score;
}

// ============================
// Generate Legal Moves
// ============================

std::vector<LegalMove> Board::GenerateLegalMoves(Color color)
{
    std::vector<LegalMove> moves;


    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            Piece* piece = board[y][x];


            if(piece != nullptr &&
               piece->GetColor() == color)
            {

                for(int ty = 0; ty < 8; ty++)
                {
                    for(int tx = 0; tx < 8; tx++)
                    {

                       Piece* target =
    GetPiece(Position{tx,ty});


if(piece->CanMove(
        *this,
        Position{x,y},
        Position{tx,ty}))
{
    if(target == nullptr ||
       target->GetColor() != color)
    {
        LegalMove move;

        move.from = Position{x,y};
        move.to = Position{tx,ty};

        moves.push_back(move);
    }
}
{
    LegalMove move;

    move.from = Position{x,y};
    move.to   = Position{tx,ty};

    Piece* captured = board[ty][tx];

    MakeTestMove(move);

    bool safe = !IsKingInCheck(color);

    UndoTestMove(move, captured);

    if(safe)
    {
        moves.push_back(move);
    }
}

                    }
                }

            }
        }
    }


    return moves;
}

// ============================
// Make Test Move
// ============================

Piece* Board::MakeTestMove(LegalMove move)
{
    Piece* piece = board[move.from.y][move.from.x];

    if(piece == nullptr)
    {
        return nullptr;
    }

    Piece* captured = board[move.to.y][move.to.x];

    board[move.to.y][move.to.x] = piece;
    board[move.from.y][move.from.x] = nullptr;

    return captured;
}


// ============================
// Undo Test Move
// ============================

void Board::UndoTestMove(
    LegalMove move,
    Piece* captured
)
{
    Piece* piece = board[move.to.y][move.to.x];


    board[move.from.y][move.from.x] = piece;


    board[move.to.y][move.to.x] = captured;
}

// ============================
// Undo Move
// ============================

bool Board::Undo()
{

    if(history.empty())
    {
        return false;
    }


    MoveRecord last = history.back();

    history.pop_back();

    // เก็บไว้สำหรับ Redo
    redoHistory.push_back(last);
          

    // คืนหมากที่เดินกลับ
    board[last.from.y][last.from.x]
        = last.movedPiece;


    // คืนหมากที่ถูกกิน
    board[last.to.y][last.to.x]
        = last.capturedPiece;


    // เปลี่ยนตากลับ
    if(currentTurn == Color::White)
    {
        currentTurn = Color::Black;
    }
    else
    {
        currentTurn = Color::White;
    }


    return true;
}

// ============================
// Redo Move
// ============================

bool Board::Redo()
{

    if(redoHistory.empty())
    {
        return false;
    }


    MoveRecord record = redoHistory.back();

    redoHistory.pop_back();


    // เดินหมากกลับไปข้างหน้า

    board[record.to.y][record.to.x]
        = record.movedPiece;


    board[record.from.y][record.from.x]
        = nullptr;


    // คืนหมากที่ถูกกินออกจากกระดาน
    // (ถ้ามี)

    if(record.capturedPiece != nullptr)
    {
        delete record.capturedPiece;
        record.capturedPiece = nullptr;
    }


    history.push_back(record);


    // เปลี่ยนตา

    if(currentTurn == Color::White)
    {
        currentTurn = Color::Black;
    }
    else
    {
        currentTurn = Color::White;
    }


    return true;
}

// ============================
// Print Move History
// ============================

void Board::PrintHistory() const
{

    if(history.empty())
    {
        std::cout << "No move history\n";
        return;
    }


    std::cout << "Move History\n";


    int count = 1;


    for(const auto& move : history)
    {

        std::cout
    << count
    << ". "
    << move.movedName
    << " ("
    << move.from.x
    << ","
    << move.from.y
    << ") -> ("
    << move.to.x
    << ","
    << move.to.y
    << ")";


if(move.capturedPiece != nullptr)
{
    std::cout
        << " กิน "
        << move.capturedName;
}


        std::cout << "\n";

        count++;
    }

}