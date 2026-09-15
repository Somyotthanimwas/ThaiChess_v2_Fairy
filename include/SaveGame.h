#ifndef SAVEGAME_H
#define SAVEGAME_H


#include "Board.h"

#include <string>


class SaveGame
{

public:

    static bool Save(
        const Board& board,
        const std::string& filename
    );


    static bool Load(
        Board& board,
        const std::string& filename
    );

};


#endif