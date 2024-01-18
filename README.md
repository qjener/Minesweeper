# Minesweeper

    Just recreating Minesweeper with SDL2 for fun
    This project started in c as a task for my university and is being translated to c++ now

## To-Do:

    -all icons are still missing, currently there are only colors
    -victory and game over are not well implemented
    -cant either export or import save files
    -menu is not working 
    -the button class doesnt work

## Compile and run:

### SINCE THE MAKEFILE WORKS THIS IS NOT NECESSARY

the paths in this should be changed depending on your personal sdl library location

    g++ -ISDL\SDL\include  -LSDL\SDL\lib  funcs.cpp main.cpp test.cpp -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_image -o Minesweeper

    .\Minesweeper
