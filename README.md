# Minesweeper

    This project started in c as a task for my university and is being translated to c++ now

## To-Do:

    -all icons are still missing, currently there are only colors
    -victory and game over are not implemented
    -cant either export or import save files
    -to play it requires the SDL2.dll file in the same folder and the whole sdl2 library installed -_-

## Compile and run:
the paths in this should be changed depending on your personal sdl library location

    g++ main.cpp funcs.cpp -IC:\eigene\dev_lib\mingw\include -LC:\eigene\dev_lib\mingw\lib -Wall -lmingw32 -lSDL2main -lSDL2 -o Minesweeper

    .\Minesweeper