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
    Grid(Grid &g) : width(g.width), height(g.height), nmines(g.nmines), fieldstoreveal(g.fieldstoreveal),
                     flags(g.flags), first(g.first), mode(0) {
                        for(int i = 0; i < getHeight(); i++) {
                            for(int j = 0; j < getWidth(); j++) {
                                grid[j+getWidth()*i] = g.grid[j+getWidth()*i];
                                open[j+getWidth()*i] = g.open[j+getWidth()*i];
                            }
                        }
                     }

    int getWidth() {
        return width;
    }
    int getHeight() {
        return height;
    }
    int getMines() {
        return nmines;
    }
    unsigned getMode() {
        return mode;
    }
    bool getFirst() {
        return first;
    }
    unsigned getFieldstoreveal() {
        return fieldstoreveal;
    }
    char getOpenPos(int x, int y) {
        return open[x+width*y];
    }

    void setMode(unsigned m) {
        mode = m;
    }
    void setFirst(bool b) {
        first = b;
    }
    void resetFieldstoreveal() {
        fieldstoreveal = height*width-nmines;
    }

    void printHidden();
    void printGrid();
    void buildGrids(unsigned x, unsigned y);
    bool isStart(int current, int x, int y);
    void addNumbers(int i, int j);
    SDL_bool revealPos(SDL_Renderer *rend, SDL_Rect *grid_cursor);
    void flagPos(SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor);
    void saveGame();
    bool drawDigit(SDL_Renderer* rend, SDL_Rect* grid_cursor);
};

class Box {
public:
    SDL_Rect area, text_rect;
public:
    SDL_Texture* text;
    SDL_Color text_color, area_color, border_color;
    int border_thickness;
    string name;
    TTF_Font *font;
    
public:
    Box() : name("BOX") {
        area = {
            .x = 0,
            .y = 0,
            .w = 72,
            .h = 36,
        };
        text_rect = {
            .x = area.x+(area.w/5),
            .y = area.y+(area.h/5),
            .w = area.w-(area.w/5)*2,
            .h = area.h-(area.h/5)*2,
        };
        area_color = {255, 255, 255, 0};
        text_color = {0, 0, 0, 0};
        border_color = {0, 0, 0, 0};
        border_thickness = 5;
        //cout << area.x << area.y << " " << area.w << area.h << name << endl;
        font = TTF_OpenFont("Lucida Console Regular.ttf", 24);        
    }
    Box(Box &b) : area(b.area), text_rect(b.text_rect), text(b.text), text_color(b.text_color), area_color(b.area_color), border_color(b.border_color), border_thickness(b.border_thickness), name(b.name), font(b.font) {}
    Box(SDL_Rect a, string n) : area(a), name(n) {
        //cout << area.x << area.y << " " << area.w << area.h << name << endl;
        text_rect = {
            .x = area.x+(area.w/5),
            .y = area.y+(area.h/5),
            .w = area.w-(area.w/5)*2,
            .h = area.h-(area.h/5)*2,
        };
        area_color = {255, 255, 255, 0};
        text_color = {0, 0, 0, 0};
        border_color = {0, 0, 0, 0};
        border_thickness = 5;
        font = TTF_OpenFont("Lucida Console Regular.ttf", 24);
    }
    Box(SDL_Rect a, SDL_Color tc, SDL_Color ac, string n) : area(a), text_color(tc), area_color(ac),name(n) {
        //cout << area.x << area.y << " " << area.w << area.h << name << endl;
        text_rect = {
            .x = area.x+(area.w/5),
            .y = area.y+(area.h/5),
            .w = area.w-(area.w/5)*2,
            .h = area.h-(area.h/5)*2,
        };
        border_color = {0, 0, 0, 0};
        border_thickness = 5;
        font = TTF_OpenFont("Lucida Console Regular.ttf", 24);
    }

    /**
     * @brief draws a created Box
     * 
     * @param renderer SDL_Renderer*
     */
    void drawBox(SDL_Renderer *renderer);
    SDL_Rect* getArea();
    /**
     * @brief Set an Area Variable
     * 
     * @param var SDL_Rect.x | SDL_Rect.y | SDL_Rect.w | SDL_Rect.h
     * @param n int
     * @return true: var was an SDL_Rect parameter,
     * @return false: var was not an SDL_Rect parameter
     */
    bool setAreaVar(char var, int n);
    /**
     * @brief Get an Area Variable
     * 
     * @param var SDL_Rect.x | SDL_Rect.y | SDL_Rect.w | SDL_Rect.h
     * @return the selected variable of area
     */
    int getAreaVar(char var);
    
};

//if you comment a function like this you can alway see their description when hovering them anywhere
//the description should say what the function does and what what it needs as parameter
void gameOver(int x, int y);
void victory(SDL_Renderer* rend, Grid* g, SDL_Surface* win);
bool yorN(SDL_Renderer* rend, int w, int h, string question);
int useMenu(SDL_Renderer* rend, Grid* g, int x, int y);
void cleanGrid(SDL_Renderer* rend, Grid* g);
Grid* reset(SDL_Renderer* rend, Grid* g);
Grid* loadGame(SDL_Renderer* rend);
void reloadGrid(SDL_Renderer* rend, Grid* g);