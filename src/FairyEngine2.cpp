// FairyEngine.cpp
#include <windows.h>
#include <string>
#include <iostream>
#include <vector>

class FairyEngine
{
public:
    FairyEngine(const std::string& stockfishPath)
        : stockfishPath(stockfishPath),
          hChildStd_IN_Wr(nullptr),
          hChildStd_OUT_Rd(nullptr),
          hProcess(nullptr)
    {
    }

    ~FairyEngine()
    {
        Stop();
    }

    bool Start();
    void Stop();

    bool WriteCommand(const std::string& command);
    std::string ReadLine();
    std::string GetBestMove(const std::string& fen, int depth);

private:
    std::string stockfishPath;

    HANDLE hChildStd_IN_Wr;
    HANDLE hChildStd_OUT_Rd;
    HANDLE hProcess;

    std::string outBuffer;
};

bool FairyEngine::WriteCommand(const std::string& command)
{
    if (!hChildStd_IN_Wr)
        return false;

    DWORD written = 0;

    BOOL ok = WriteFile(
        hChildStd_IN_Wr,
        command.c_str(),
        (DWORD)command.size(),
        &written,
        nullptr);

    std::cout << "SEND CMD : " << command;

    return ok && written == command.size();
}

// ======================================================
// ReadLine (แก้ใหม่)
// ======================================================

std::string FairyEngine::ReadLine()
{
    // -------------------------------
    // มีข้อมูลค้างใน buffer หรือไม่
    // -------------------------------
    size_t pos = outBuffer.find_first_of("\r\n");

    if (pos != std::string::npos)
    {
        std::string line = outBuffer.substr(0, pos);

        size_t next = pos;

        while (next < outBuffer.size() &&
               (outBuffer[next] == '\r' ||
                outBuffer[next] == '\n'))
        {
            ++next;
        }

        outBuffer.erase(0, next);

        std::cout << "PIPE : " << line << std::endl;

        return line;
    }

    if (!hChildStd_OUT_Rd)
        return "";

    DWORD available = 0;

    if (!PeekNamedPipe(
            hChildStd_OUT_Rd,
            nullptr,
            0,
            nullptr,
            &available,
            nullptr))
    {
        return "";
    }

   if (available == 0)
{
    // ถ้ามีข้อมูลค้างแต่ไม่มี newline
    if (!outBuffer.empty())
    {
        std::string s = outBuffer;
        outBuffer.clear();

        std::cout << "PIPE BUFFER : "
                  << s
                  << std::endl;

        return s;
    }

    return "";
}

    return "";
}

    std::vector<char> buf(available + 1);

    DWORD read = 0;

    if (!ReadFile(
            hChildStd_OUT_Rd,
            buf.data(),
            available,
            &read,
            nullptr))
    {
        return "";
    }

    if (read == 0)
        return "";

    buf[read] = '\0';

    outBuffer.append(buf.data(), read);

    pos = outBuffer.find_first_of("\r\n");

    if (pos == std::string::npos)
        return "";

    std::string line = outBuffer.substr(0, pos);

    size_t next = pos;

    while (next < outBuffer.size() &&
          (outBuffer[next] == '\r' ||
           outBuffer[next] == '\n'))
    {
        ++next;
    }

    outBuffer.erase(0, next);

    std::cout << "PIPE : " << line << std::endl;

    return line;
}

bool FairyEngine::Start()
{
    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;

    HANDLE outWrite = nullptr;
    HANDLE inRead = nullptr;

    if (!CreatePipe(
            &hChildStd_OUT_Rd,
            &outWrite,
            &sa,
            0))
    {
        return false;
    }

    if (!CreatePipe(
            &inRead,
            &hChildStd_IN_Wr,
            &sa,
            0))
    {
        return false;
    }

    SetHandleInformation(
        hChildStd_OUT_Rd,
        HANDLE_FLAG_INHERIT,
        0);

    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};

    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;

    si.hStdInput = inRead;
    si.hStdOutput = outWrite;
    si.hStdError = outWrite;

    std::string cmd = "\"" + stockfishPath + "\"";

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
        &pi);

    CloseHandle(outWrite);
    CloseHandle(inRead);

    if (!ok)
    {
        std::cout << "CreateProcess Error = "
                  << GetLastError()
                  << std::endl;

        return false;
    }

    hProcess = pi.hProcess;

    CloseHandle(pi.hThread);

    WriteCommand("uci\n");

    while (true)
    {
        std::string line = ReadLine();

        if (!line.empty() &&
            line.find("uciok") != std::string::npos)
            break;

        Sleep(10);
    }

    WriteCommand("setoption name UCI_Variant value makruk\n");
    WriteCommand("isready\n");

    while (true)
    {
        std::string line = ReadLine();

        if (!line.empty() &&
            line.find("readyok") != std::string::npos)
            break;

        Sleep(10);
    }

    std::cout << "FAIRY READY" << std::endl;

    return true;
}

// ======================================================
// GetBestMove
// ======================================================

std::string FairyEngine::GetBestMove(const std::string& fen, int depth)
{
    std::cout << "SEND FEN : " << fen << std::endl;

    // เริ่มเกมใหม่ทุกครั้ง
  

    WriteCommand("position fen " + fen + "\n");
    WriteCommand("go depth " + std::to_string(depth) + "\n");

    while (true)
{
    std::string line = ReadLine();

    std::cout << "READ LINE = [" << line << "]" << std::endl;

    if (line.empty())
    {
        Sleep(10);
        continue;
    }

        // แสดง info ทั้งหมด
        if (line.rfind("info", 0) == 0)
        {
            continue;
        }

        if (line.rfind("bestmove", 0) == 0)
        {
            std::string move = line.substr(9);

            // ตัดคำว่า ponder ออก
            size_t p = move.find(" ponder");
            if (p != std::string::npos)
                move.erase(p);

            // trim
            while (!move.empty() &&
                  (move.front() == ' ' ||
                   move.front() == '\r' ||
                   move.front() == '\n'))
            {
                move.erase(move.begin());
            }

            while (!move.empty() &&
                  (move.back() == ' ' ||
                   move.back() == '\r' ||
                   move.back() == '\n'))
            {
                move.pop_back();
            }

            std::cout << "BESTMOVE = " << move << std::endl;

            return move;
        }
    }
}

// ======================================================
// Stop
// ======================================================

void FairyEngine::Stop()
{
    if (hChildStd_IN_Wr)
    {
        WriteCommand("quit\n");

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
        DWORD exitCode = STILL_ACTIVE;

        if (GetExitCodeProcess(hProcess, &exitCode) &&
            exitCode == STILL_ACTIVE)
        {
            WaitForSingleObject(hProcess, 1000);

            GetExitCodeProcess(hProcess, &exitCode);

            if (exitCode == STILL_ACTIVE)
                TerminateProcess(hProcess, 0);
        }

        CloseHandle(hProcess);
        hProcess = nullptr;
    }
}
