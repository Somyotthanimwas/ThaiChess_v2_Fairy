cmake_minimum_required(VERSION 3.20)
project(ThaiChess LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(include)
if (MINGW OR CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()
add_executable(ThaiChessApp
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
add_executable(ThaiChess
    src/launcher.cpp/launcher.cpp
    src/ThaiChess.rc
)
target_compile_definitions(ThaiChessApp PRIVATE SDL_MAIN_HANDLED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
target_link_libraries(ThaiChessApp PRIVATE
    SDL2::SDL2
    SDL2_ttf::SDL2_ttf
    SDL2_image::SDL2_image
)
add_custom_command(TARGET ThaiChessApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_SOURCE_DIR}/assets"
            "$<TARGET_FILE_DIR:ThaiChessApp>/assets"
)
install(TARGETS ThaiChess ThaiChessApp RUNTIME DESTINATION .)
install(DIRECTORY assets DESTINATION .)
set(CPACK_GENERATOR ZIP)
set(CPACK_PACKAGE_FILE_NAME "ThaiChess-Windows-${CMAKE_SYSTEM_PROCESSOR}")
include(CPack)
