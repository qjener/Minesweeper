#include <time.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#define GRID_SIZE 500                               //500 is really big, any bigger would be pain to play (especially in the terminal)

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
void revealZero(grid* g, int i, int j);
bool revealPos(grid* g, SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
void flagPos(grid* g, SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
void gameOver();
void victory();
void saveGame(grid* g);
bool drawDigit(SDL_Color *color, grid* g, int x, int y);
void setMenu(SDL_Renderer *rend, grid* g, SDL_Color *grid_menu_color, SDL_Rect *grid_menu, int grid_cell_size);
void useMenu(grid* g, int x, int y);

//flag position doesnt always work and doesnt change the color of the cell
//reavealzero is not implemented
//all icons are still missing
//victory is not implemented