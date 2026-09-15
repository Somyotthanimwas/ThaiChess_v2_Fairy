#include "Stockfish.h"

#include <iostream>
#include <fstream>
#include <cstdlib>


Stockfish::Stockfish()
{
    started = false;
}


Stockfish::~Stockfish()
{

}


bool Stockfish::Start()
{
    started = true;

    return true;
}



std::string Stockfish::GetBestMove(
    const std::string& fen,
    int depth
)
{

    if(!started)
        return "";


    std::ofstream cmd("stockfish_cmd.txt");


cmd
<< "uci\n"
<< "setoption name UCI_Variant value makruk\n"
<< "setoption name Skill Level value 20\n"
<< "setoption name UCI_Elo value 2850\n"
<< "setoption name Hash value 256\n"
<< "ucinewgame\n"
<< "setoption name Clear Hash\n"
<< "isready\n"

<< "position fen "
<< fen
<< "\n"

<< "d\n"

<< "go depth "
<< depth
<< "\n";
cmd.close();

#ifdef _WIN32

system(
    "wsl -e bash -c \"cd /home/thanimwas/ThaiChess_v2/build-win && ./stockfish.exe < stockfish_cmd.txt > stockfish_out.txt\""
);
#else

system(
    "./stockfish < stockfish_cmd.txt > stockfish_out.txt"
);
std::cout 
    << "Fairy system return = "
    << std::endl;

#endif



    std::ifstream out(
    "stockfish_out.txt"
);

if(!out)
{
    std::cout 
        << "Cannot open stockfish_out.txt"
        << std::endl;

    return "";
}


    std::string line;


    while(std::getline(out,line))
    {

        if(line.find("bestmove")
            != std::string::npos)
        {

            size_t pos =
                line.find("bestmove");


            std::string move =
                line.substr(pos + 9);


            return move;

        }

    }


    return "";

}