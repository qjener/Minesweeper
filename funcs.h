#include <time.h>
#include <math.h>
#include <unistd.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

#define GRID_SIZE 500                               //500 is really big, any bigger would be pain to play (especially in the terminal)
#define GRID_CELL_SIZE 36                           //Amount of pixels per cell

class Grid {                                            //THE MATRIX
private:
    unsigned grid[GRID_SIZE];
    char open[GRID_SIZE];
    int width;
    int height;
    int nmines;
    unsigned fieldstoreveal;
    unsigned flags;
    bool first;
    unsigned mode;
public:
    Grid() {
        width = 16;
        height = 16;
        nmines = 40;
        fieldstoreveal = width*height-nmines;
        flags = nmines;
        first = true;
        mode = 0;
        for(int j = 0; j < height; j++) {
            for(int i = 0; i < width; i++) {
                grid[i+width*j] = 0;
                open[i+width*j] = 'x';
            }
        }
    }
    Grid(int w, int h, int m) : width(w), height(h), nmines(m) {
        fieldstoreveal = width*height-nmines;
        flags = nmines;
        first = true;
        mode = 0;
        for(int j = 0; j < height; j++) {
            for(int i = 0; i < width; i++) {
                grid[i+width*j] = 0;
                open[i+width*j] = 'x';
            }
        }
    }
    Grid(Grid &g) : width(g.width), height(g.height), nmines(g.nmines), fieldstoreveal(g.fieldstoreveal), flags(g.flags), first(g.first), mode(0) {}

    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }
    unsigned getMode() {
        return mode;
    }
    bool getFirst() {
        return first;
    }
    unsigned getFieldtoreveal() {
        return fieldstoreveal;
    }
    char getOpenPos(int x, int y) {
        return open[x/width*y];
    }

    void setMode(unsigned m) {
        mode = m;
    }


    void printHidden();
    void printGrid();
    void buildGrids(unsigned x, unsigned y);
    bool isStart(int current, int x, int y);
    void addNumbers(int i, int j);
    bool revealPos(SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
    void flagPos(SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
    void saveGame();
    bool drawDigit(SDL_Renderer* rend, SDL_Rect* grid_cursor, SDL_Color *color);
    void reset(SDL_Renderer* rend);
};

class Button {
public:
    SDL_Rect area, text_rect;
    SDL_Texture* text;
    SDL_Color color;
    string name;
    TTF_Font *font;
    
public:
    Button(SDL_Renderer* rend) : name("Button") {
        area = {
            .x = 0,
            .y = 0,
            .w = 72,
            .h = 36,
        };
        color = {255, 255, 255, 0};
        cout << area.x << area.y << area.w << area.h << name << endl;
        font = TTF_OpenFont("Times New Roman.ttf", 24);
        get_text_and_rect(rend);
        
    }
    Button(SDL_Renderer* rend, SDL_Rect a, SDL_Color c, string n) : area(a), color(c), name(n) {
        cout << area.x << area.y << area.w << area.h << name << endl;
        font = TTF_OpenFont("Times New Roman.ttf", 24);
        get_text_and_rect(rend);
    }
    

    void get_text_and_rect(SDL_Renderer *renderer);
    void useMenu(SDL_Renderer* rend, Grid* g, int x, int y);
};


//if you comment a function like this you can alway see their description when hovering them anywhere
//the description should say what the function does and what what it needs as parameter
Grid* getGrid();
void gameOver(int x, int y);
void victory();
bool yorN();