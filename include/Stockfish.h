#ifndef STOCKFISH_H
#define STOCKFISH_H

#include <string>

class Stockfish
{

public:

    Stockfish();

    ~Stockfish();


    bool Start();


    std::string GetBestMove(
        const std::string& fen,
        int depth
    );


private:

    bool started;

};

#endif