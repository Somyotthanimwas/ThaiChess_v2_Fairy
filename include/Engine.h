#pragma once

#include <string>

class FairyEngine
{
private:
    FILE* pipe;

public:

    bool Start();

    std::string GetBestMove(
        const std::string& fen,
        int depth
    );

    void Stop();
};