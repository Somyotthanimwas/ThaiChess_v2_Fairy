#include "FairyEngine.h"

#include <iostream>
#include <windows.h>


#ifdef _WIN32


bool FairyEngine::WriteCommand(
    const std::string& command
)
{
    if(!hChildStd_IN_Wr)
        return false;


    DWORD written = 0;


    BOOL ok = WriteFile(
        hChildStd_IN_Wr,
        command.c_str(),
        (DWORD)command.size(),
        &written,
        nullptr
    );


    std::cout
    << "SEND CMD : "
    << command;


    return ok && written == command.size();
}




std::string FairyEngine::ReadLine()
{
    if(!hChildStd_OUT_Rd)
        return "";


    char buffer[2048];

    DWORD read = 0;


    BOOL ok = ReadFile(
        hChildStd_OUT_Rd,
        buffer,
        sizeof(buffer)-1,
        &read,
        nullptr
    );


    if(!ok || read==0)
        return "";


    buffer[read]='\0';


    std::string text(buffer);


    std::cout
    << "PIPE : "
    << text;


    return text;
}





bool FairyEngine::Start()
{

    SECURITY_ATTRIBUTES sa{};

    sa.nLength =
        sizeof(sa);

    sa.bInheritHandle =
        TRUE;



    HANDLE outWrite=nullptr;
    HANDLE inRead=nullptr;



    CreatePipe(
        &hChildStd_OUT_Rd,
        &outWrite,
        &sa,
        0
    );


    CreatePipe(
        &inRead,
        &hChildStd_IN_Wr,
        &sa,
        0
    );


    SetHandleInformation(
        hChildStd_OUT_Rd,
        HANDLE_FLAG_INHERIT,
        0
    );



    STARTUPINFOA si{};

    PROCESS_INFORMATION pi{};


    si.cb =
        sizeof(si);


    si.dwFlags =
        STARTF_USESTDHANDLES;


    si.hStdOutput =
        outWrite;


    si.hStdError =
        outWrite;


    si.hStdInput =
        inRead;



    std::string cmd =
        "\"" +
        stockfishPath +
        "\"";



    BOOL ok =
    CreateProcessA(
        nullptr,
        &cmd[0],
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        nullptr,
        &si,
        &pi
    );



    CloseHandle(outWrite);
    CloseHandle(inRead);



    if(!ok)
    {
        std::cout
        << "CREATE PROCESS FAIL\n";

        return false;
    }



    hProcess =
        pi.hProcess;


    CloseHandle(pi.hThread);



    WriteCommand("uci\n");



    while(true)
    {
        std::string line =
            ReadLine();


        if(line.find("uciok")
            !=std::string::npos)
            break;

        Sleep(10);
    }



    WriteCommand(
        "setoption name UCI_Variant value makruk\n"
    );


    WriteCommand(
        "isready\n"
    );



    while(true)
    {

        std::string line =
            ReadLine();


        if(line.find("readyok")
            !=std::string::npos)
            break;


        Sleep(10);
    }



    std::cout
    << "FAIRY READY\n";


    return true;
}







std::string FairyEngine::GetBestMove(
    const std::string& fen,
    int depth
)
{

    std::cout
    << "SEND FEN : "
    << fen
    << std::endl;



    WriteCommand(
        "position fen "
        + fen
        + "\n"
    );


    Sleep(50);



    WriteCommand(
        "go depth "
        + std::to_string(depth)
        + "\n"
    );



    while(true)
    {

        std::string line =
            ReadLine();



        if(line.find("bestmove")
            !=std::string::npos)
        {

            size_t p =
            line.find("bestmove");


            std::string move =
            line.substr(p+9);



            while(!move.empty() &&
                 (move.back()=='\n' ||
                  move.back()=='\r'))
            {
                move.pop_back();
            }


            return move;
        }


        Sleep(10);

    }


    return "";
}






void FairyEngine::Stop()
{

    if(hChildStd_IN_Wr)
    {

        WriteCommand("quit\n");


        CloseHandle(
            hChildStd_IN_Wr
        );


        hChildStd_IN_Wr=nullptr;
    }



    if(hChildStd_OUT_Rd)
    {

        CloseHandle(
            hChildStd_OUT_Rd
        );


        hChildStd_OUT_Rd=nullptr;
    }



    if(hProcess)
    {

        TerminateProcess(
            hProcess,
            0
        );


        CloseHandle(
            hProcess
        );


        hProcess=nullptr;
    }

}


#endif