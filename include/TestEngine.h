#ifndef TESTENGINE_H
#define TESTENGINE_H

class TestEngine
{
public:

    static void RunAll();

    static void TestPawn();
    static void TestRook();
    static void TestKnight();
    static void TestKhon();
    static void TestMet();
    static void TestKing();
         static void TestUndo();
      static void TestRedo();

        static void TestPromotion();
        static void TestCheck();
        static void TestCheckmate();
         static void TestCaptureCheck();
    static void TestBlockCheck();
        static void TestStalemate();

};

#endif