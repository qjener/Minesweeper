# Minesweeper

    This project started in c as a task for my university and is being translated to c++ now

## To-Do:

    -all icons are still missing, currently there are only colors
    -victory and game over are not well implemented
    -cant either export or import save files
    -menu is not working 
    -the button class doesnt work

## Compile and run:

### MAKEFILE WORKS

the paths in this should be changed depending on your personal sdl library location

    g++ main.cpp funcs.cpp -Wall -lmingw32 -lSDL2main -lSDL2 -Ix86_64-w64-mingw32\include -Lx86_64-w64-mingw32\lib -o Minesweeper

    .\Minesweeper