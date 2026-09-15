#ifndef FAIRYENGINE_H
#define FAIRYENGINE_H

#include <string>
#include <windows.h>


class FairyEngine
{

private:

    HANDLE hProcess = nullptr;

    HANDLE hChildStd_IN_Wr = nullptr;
    HANDLE hChildStd_OUT_Rd = nullptr;


    std::string stockfishPath;


    bool WriteCommand(
        const std::string& command
    );


    std::string ReadLine();


    std::string outBuffer;


public:


    // constructor default
   FairyEngine()
    :
    stockfishPath(
        "stockfish.exe"
    )
{
}



    // constructor กำหนด path เอง
    FairyEngine(
        const std::string& path
    )
        :
        stockfishPath(path)
    {
    }



    ~FairyEngine()
    {
        Stop();
    }



    bool Start();



    std::string GetBestMove(
        const std::string& fen,
        int depth
    );



    void Stop();



    void SetStockfishPath(
        const std::string& path
    )
    {
        stockfishPath = path;
    }


    std::string GetStockfishPath() const
    {
        return stockfishPath;
    }

};


#endif