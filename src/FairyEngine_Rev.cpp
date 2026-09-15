// FairyEngine.cpp

#include "FairyEngine.h"

#include <iostream>
#include <vector>


// Write command (raw bytes). Returns true if all bytes written.

bool FairyEngine::WriteCommand(
    const std::string& command
)
{
    if (!hChildStd_IN_Wr)
    {
        std::cout
            << "WRITE HANDLE INVALID"
            << std::endl;

        return false;
    }

    DWORD written = 0;

    BOOL ok = WriteFile(
        hChildStd_IN_Wr,
        command.c_str(),
        (DWORD)command.size(),
        &written,
        nullptr
    );

    if (!ok)
    {
        std::cout
            << "WRITE ERROR = "
            << GetLastError()
            << std::endl;

        return false;
    }

    FlushFileBuffers(hChildStd_IN_Wr);

    std::cout
        << "SEND CMD : "
        << command;

    std::cout
        << "WRITE RESULT = "
        << ok
        << " BYTES="
        << written
        << std::endl;

    return written == command.size();
}
     

// ReadLine: non-blocking, uses PeekNamedPipe, accumulates in outBuffer, returns one line (without CR/LF) if available.
// If no data available returns empty string.

std::string FairyEngine::ReadLine()
{
    if (!hChildStd_OUT_Rd)
        return "";

    // ถ้ามีบรรทัดค้างอยู่ใน buffer แล้ว
    size_t pos = outBuffer.find('\n');

    if (pos != std::string::npos)
    {
        std::string line = outBuffer.substr(0, pos);

        outBuffer.erase(0, pos + 1);

        if (!line.empty() && line.back() == '\r')
            line.pop_back();

        std::cout
            << "PIPE : "
            << line
            << std::endl;

        return line;
    }

    DWORD available = 0;

    if (!PeekNamedPipe(
            hChildStd_OUT_Rd,
            nullptr,
            0,
            nullptr,
            &available,
            nullptr))
    {
        std::cout
            << "PEEK ERROR = "
            << GetLastError()
            << std::endl;

        return "";
    }

    if (available == 0)
        return "";

    char buffer[4096];
    DWORD read = 0;

    BOOL ok = ReadFile(
        hChildStd_OUT_Rd,
        buffer,
        sizeof(buffer) - 1,
        &read,
        nullptr
    );

    if (!ok)
    {
        std::cout
            << "READFILE ERROR = "
            << GetLastError()
            << std::endl;

        return "";
    }

    if (read == 0)
        return "";

    buffer[read] = '\0';

    outBuffer += buffer;

    // ลองหา newline อีกครั้ง
    pos = outBuffer.find('\n');

    if (pos == std::string::npos)
        return "";

    std::string line = outBuffer.substr(0, pos);

    outBuffer.erase(0, pos + 1);

    if (!line.empty() && line.back() == '\r')
        line.pop_back();

    std::cout
        << "PIPE : "
        << line
        << std::endl;

    return line;
}
bool FairyEngine::Start()
{
    std::cout
        << "ENTER FairyEngine::Start()"
        << std::endl;

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;


    HANDLE outWrite = nullptr;
    HANDLE inRead = nullptr;


    // child stdout -> parent read
    if (!CreatePipe(
            &hChildStd_OUT_Rd,
            &outWrite,
            &sa,
            0))
    {
        std::cout
            << "CreatePipe stdout failed error="
            << GetLastError()
            << std::endl;

        return false;
    }


    // parent write -> child stdin
    if (!CreatePipe(
            &inRead,
            &hChildStd_IN_Wr,
            &sa,
            0))
    {
        std::cout
            << "CreatePipe stdin failed error="
            << GetLastError()
            << std::endl;


        CloseHandle(hChildStd_OUT_Rd);
        hChildStd_OUT_Rd = nullptr;

        return false;
    }



    // parent ห้ามถูก inherit
    SetHandleInformation(
        hChildStd_OUT_Rd,
        HANDLE_FLAG_INHERIT,
        0
    );



    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};


    si.cb = sizeof(si);

    si.dwFlags =
        STARTF_USESTDHANDLES |
        STARTF_USESHOWWINDOW;


    si.wShowWindow = SW_HIDE;


    si.hStdOutput = outWrite;
    si.hStdError  = outWrite;
    si.hStdInput  = inRead;



    std::cout
        << "OUT READ HANDLE = "
        << hChildStd_OUT_Rd
        << std::endl;


    std::cout
        << "OUT WRITE HANDLE = "
        << outWrite
        << std::endl;


    std::cout
        << "IN WRITE HANDLE = "
        << hChildStd_IN_Wr
        << std::endl;



     
std::string cmd =
    "\"" + stockfishPath + "\"";


std::cout
    << "STOCKFISH PATH = ["
    << stockfishPath
    << "]"
    << std::endl;


std::cout
    << "CMD = ["
    << cmd
    << "]"
    << std::endl;

   BOOL ok = CreateProcessA(
    nullptr,
    cmd.data(),
    nullptr,
    nullptr,
    TRUE,
    CREATE_NO_WINDOW,
    nullptr,
    nullptr,
    &si,
    &pi
);


if(!ok)
{
    std::cout
        << "CREATE PROCESS FAIL error="
        << GetLastError()
        << std::endl;

    CloseHandle(hChildStd_OUT_Rd);
    hChildStd_OUT_Rd = nullptr;

    CloseHandle(hChildStd_IN_Wr);
    hChildStd_IN_Wr = nullptr;

    CloseHandle(outWrite);
    CloseHandle(inRead);

    return false;
}


// =============================
// PROCESS OK
// =============================

std::cout
    << "PROCESS CREATED OK"
    << std::endl;


hProcess = pi.hProcess;


std::cout
    << "PROCESS HANDLE = "
    << hProcess
    << std::endl;


std::cout
    << "OUT READ HANDLE = "
    << hChildStd_OUT_Rd
    << std::endl;


std::cout
    << "IN WRITE HANDLE = "
    << hChildStd_IN_Wr
    << std::endl;


DWORD exitCode = 0;

GetExitCodeProcess(
    hProcess,
    &exitCode
);


std::cout
    << "PROCESS STATUS = "
    << exitCode
    << std::endl;


// ปิด thread handle เท่านั้น
CloseHandle(pi.hThread);


// ปิด handle ฝั่ง child ที่ parent ไม่ใช้แล้ว
CloseHandle(outWrite);
CloseHandle(inRead);

std::cout
    << "AFTER CLOSE CHILD HANDLE"
    << std::endl;

std::cout
    << "IN WRITE HANDLE = "
    << hChildStd_IN_Wr
    << std::endl;


    // =====================
    // UCI INIT
    // =====================


     

WriteCommand("uci\r\n");

DWORD start = GetTickCount();

while (true)
{
    if (GetTickCount() - start > 5000)
    {
        std::cout
            << "WAIT UCI TIMEOUT"
            << std::endl;

        return false;
    }

    std::string line = ReadLine();

    if (!line.empty())
    {
        std::cout
            << "UCI : "
            << line
            << std::endl;

        if (line.find("uciok") != std::string::npos)
            break;
    }

    Sleep(10);
}

   WriteCommand(
    "setoption name UCI_Variant value makruk\r\n"
);

WriteCommand(
    "ucinewgame\r\n"
);

WriteCommand("isready\r\n");

start = GetTickCount();

while (true)
{
    if (GetTickCount() - start > 5000)
    {
        std::cout
            << "WAIT READY TIMEOUT"
            << std::endl;

        return false;
    }

    std::string line = ReadLine();

    if (!line.empty())
    {
        std::cout
            << "READY : "
            << line
            << std::endl;

        if (line.find("readyok") != std::string::npos)
            break;
    }

    Sleep(10);
}

    std::cout
        << "FAIRY READY"
        << std::endl;

    return true;
}

std::string FairyEngine::GetBestMove(const std::string& fen, int depth)
{
    std::cout
        << "SEND FEN : "
        << fen
        << std::endl;


    std::cout
        << "DEBUG IN HANDLE = "
        << hChildStd_IN_Wr
        << std::endl;


    std::cout
        << "DEBUG PROCESS = "
        << hProcess
        << std::endl;
std::cout
    << "GETBEST IN HANDLE = "
    << hChildStd_IN_Wr
    << std::endl;

std::cout
    << "GETBEST PROCESS = "
    << hProcess
    << std::endl;
    // ส่งตำแหน่ง
    WriteCommand(
        std::string("position fen ")
        + fen
        + "\n"
    );
Sleep(50);
    

    // เริ่มคิด
    WriteCommand(
        std::string("go depth ")
        + std::to_string(depth)
        + "\n"
    );


    std::cout
        << "WAIT BESTMOVE"
        << std::endl;


    DWORD start = GetTickCount();


    for (;;)
    {

        // timeout 10 วินาที ป้องกันวนไม่จบ
        if (GetTickCount() - start > 10000)
        {
            std::cout
                << "ENGINE TIMEOUT"
                << std::endl;

            return "";
        }


        std::string line = ReadLine();


        std::cout
            << "READ FROM ENGINE = ["
            << line
            << "]"
            << std::endl;



        if (!line.empty())
        {
            std::cout
                << "ENGINE: "
                << line
                << std::endl;


            if (line.find("bestmove") != std::string::npos)
            {

                size_t p = line.find("bestmove");


                std::string move =
                    line.substr(p + 9);



                // ตัด ponder ออก
                size_t space =
                    move.find(' ');


                if(space != std::string::npos)
                {
                    move =
                        move.substr(0, space);
                }



                // trim
                while(!move.empty() &&
                    (
                        move.back() == '\n' ||
                        move.back() == '\r' ||
                        move.back() == ' '
                    ))
                {
                    move.pop_back();
                }



                std::cout
                    << "FAIRY BEST MOVE = "
                    << move
                    << std::endl;



                return move;
            }
        }


        Sleep(50);
    }


    return "";
}
void FairyEngine::Stop()
{
    std::cout
        << "FAIRY STOP CALLED"
        << std::endl;

    if (hChildStd_IN_Wr)
    {
        WriteCommand("quit\r\n");

        // รอให้ engine ปิดเองก่อน
        if (hProcess)
            WaitForSingleObject(hProcess, 1000);

        CloseHandle(hChildStd_IN_Wr);
        hChildStd_IN_Wr = nullptr;
    }

    if (hChildStd_OUT_Rd)
    {
        CloseHandle(hChildStd_OUT_Rd);
        hChildStd_OUT_Rd = nullptr;
    }

    if (hProcess)
    {
        CloseHandle(hProcess);
        hProcess = nullptr;
    }
}
 

