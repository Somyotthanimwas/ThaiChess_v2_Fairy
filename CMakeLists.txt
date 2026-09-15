cmake_minimum_required(VERSION 3.10)

project(ThaiChess)



set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_compile_options(
    -finput-charset=UTF-8
    -fexec-charset=UTF-8
)

add_executable(
    ThaiChess
       # WIN32
    src/main.cpp
    src/Board.cpp
    src/AI.cpp
    src/Stockfish.cpp
    src/Piece.cpp
    src/King.cpp
    src/Rook.cpp
    src/Knight.cpp
    src/Khon.cpp
    src/Met.cpp
    src/Pawn.cpp
    src/SaveGame.cpp
    src/TestEngine.cpp
    src/ChessGUI.cpp
    src/FairyEngine.cpp
         src/ThaiChess.rc
)


set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")


find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)

link_directories(
    $ENV{HOME}/vcpkg/installed/x64-mingw-static/lib
)

target_compile_definitions(
    ThaiChess PRIVATE
    SDL_MAIN_HANDLED
)

target_link_libraries(ThaiChess
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf-static
         SDL2_image::SDL2_image-static

)