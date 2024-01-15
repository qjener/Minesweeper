#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define GRID_SIZE 500                               //500 is really big, any bigger would be pain to play (especially in the terminal)
#define GRID_CELL_SIZE 36                           //amount of pixels per cell

typedef struct {                                            //THE MATRIX
    unsigned grid[GRID_SIZE];
    char open[GRID_SIZE];
    int width;
    int height;
    int nmines;
    unsigned fieldstoreveal;
    unsigned flags;
    bool first;
    unsigned mode;
} grid;

//if you comment a function like this you can alway see their description when hovering them anywhere
bool yorN();
//the description should say what the function does and what what it needs as parameter
grid* getGrid(bool reset);
bool isStart(grid* g, int current, int x, int y);
void addNumbers(grid* g, int i, int j);
void buildGrids(grid* g, unsigned x, unsigned y);
bool revealPos(grid* g, SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
void flagPos(grid* g, SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
void gameOver();
void victory();
void saveGame(grid* g);
bool drawDigit(SDL_Renderer* rend, SDL_Rect* grid_cursor, SDL_Color *color, grid* g);
void setMenu(SDL_Renderer *rend, grid* g, SDL_Color *grid_menu_color, SDL_Rect *grid_menu);
void useMenu(grid* g, SDL_Renderer* rend, int x, int y);
void reset(grid* g, SDL_Renderer* rend);