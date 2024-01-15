#include "funcs.h"

void printGrid(grid* g) {
    printf("\n\n   ");
        for(int i = 1; i <= g->width; i++) {
            if(i < 10) printf("%d  ", i);
            else printf("%d ", i);
        }
        printf("\n");
        for(int y = 0; y < g->height; y++) {
            if(y+1 < 10) printf("%d  ", y+1);
            else printf("%d ", y+1);
            for(int x = 0; x < g->width; x++) {
                if(g->open[x+g->width*y] == 'x') printf("x  ");
                else if(g->open[x+g->width*y] == 'f') printf("f  ");
                else printf("%c  ", g->open[x+g->width*y]);
            }
            printf("\n");
        }
        printf("Flags: %u\n", g->flags);
}

grid* getGrid(bool reset) {
    grid* g;
    //grid h;
    /*
    FILE* save;
    if (!access("save.bin", F_OK)) {
        printf("Save file detected - do you want to continue your game? (Y/n)\n");
        if(yorN()) {
            save = fopen("save.bin", "rb");
            fread(&h, sizeof(grid), 1, save);
            
            g = &h;
            fclose(save);
            return g;
        }
    }*/

    g = (grid*) malloc(sizeof(grid));
    /*
    if(!reset) {
        printf("You are currently playing with a medium playing field size of 16 x 16 and 40 mines\nDo you want to put in your own parameters? (Y/n)\n");
    }else {
        printf("\n\nParameters resetting to default...\n\nDo you want to set your own parameters? (Y/n)\n");
    }
    if(yorN()) {
        while(1) {
            printf("Width: ");
            scanf("%u", &g->width);
            printf("Height: ");
            scanf("%u", &g->height);
            if(g->width*g->height > GRID_SIZE) {
                printf("Chosen playingfield is too big - pls set a smaller one\n");
                continue;
            }
            printf("Number of mines: ");
            scanf("%u", &g->nmines);
            if(g->nmines > g->width*g->height) {
                printf("Too many mines - pls select less than %u (this is depended on your chosen field size)\n", g->width*g->height);
                continue;
            }
            break;
        }
        if(g->width == 16 && g->height == 16 && g->nmines == 40) {
            printf("\nvery original\n");
        }
        getchar();
    }else {*/
        g->width = 16, g->height = 16, g->nmines = 40;
    //}
    g->fieldstoreveal = g->width*g->height-g->nmines;
    for(int j = 0; j < g->height; j++) {
        for(int i = 0; i < g->width; i++) {
            g->grid[i+g->width*j] = 0;
            g->open[i+g->width*j] = 'x';
        }
    }

    g->flags = g->nmines;
    g->first = true;
    g->mode = 0;
    return g;
}

void addNumbers(grid* g, int x, int y) {
    for(unsigned n = 0; n < 9; n++, y++) {
        if(x >= 0 && y >= 0 && x < g->width && y < g->height && g->grid[x+g->width*y] != 10) {
            g->grid[x+g->width*y] += 1;
        }
        if(n == 2 || n == 5) {
            x++;
            y -= 3;
        }
    }
}

bool isStart(grid* g, int current, int x, int y) {
    for(unsigned n = 0; n < 9; n++, y++) {
        if(x >= 0 && y >= 0 && x < g->width && y < g->height && current == x+g->width*y) {
            return true;
        }
        if(n == 2 || n == 5) {
            x++;
            y -= 3;
        }
    }
    return false;
}

void buildGrids(grid* g, unsigned x, unsigned y) {
    unsigned current;
    srand(time(NULL));
    for(int i = 0 ; i < g->nmines; i++ ) {
        current = rand() % (g->width * g->height);
        if(g->grid[current] == 10 || isStart(g, current, x-1, y-1)) {
            i--;
            continue;
        }
        g->grid[current] = 10;
        addNumbers(g, current % g->width-1, current/g->width-1);
    }
    g->first = false;
}

bool revealPos(grid* g, SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor) {
    if(g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f' || g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] == 'r') return 1;
    bool b = drawDigit(rend, grid_cursor, grid_cursor_color, g);
    return b;
}

void flagPos(grid* g, SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor) {
    if(g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f') {                                        //remove flag
        g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] = 'x';
        g->flags++;
        return;
    }
    if(g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] != 'x') {                                        //return if already revealed
        return;
    }
    if(g->flags <= 0) {                                                                                         //return if no flags left
        return;
    }
                                                                                                                //execute putting in flag (green)
    grid_cursor_color->r = 46;
    grid_cursor_color->g = 204;
    grid_cursor_color->b = 113;
    
    SDL_SetRenderDrawColor(rend, grid_cursor_color->r, grid_cursor_color->g, grid_cursor_color->b, 0);
    SDL_RenderFillRect(rend, grid_cursor);
    g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] = 'f';
    g->flags--;
    return;
}

void gameOver() {
    ;
}

void victory() {
;
}

void saveGame(grid* g) {
    if(g->first) {
        printf("You cant save an unloaded game - pls start playing first\n");
        return;
    }
    grid h = *g;
    FILE* save;
    save = fopen("save.bin", "wb");
    if (save == NULL) {
        printf("The file is not opened - try again");
        return;
    }
    else {
        for(int j = 0; j < g->height; j++) {
            for(int i = 0; i < g->width; i++) {
                ;
            }
        }
        printf("Your game was saved successfully");
    }
    fwrite(&h, sizeof(grid), 1, save);
    fclose(save);
}

bool drawDigit(SDL_Renderer* rend, SDL_Rect* grid_cursor, SDL_Color *color, grid* g) {
    if(g->open[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f') g->flags++;
    if(grid_cursor->x/GRID_CELL_SIZE < 0 || grid_cursor->y/GRID_CELL_SIZE < 0 || grid_cursor->x/GRID_CELL_SIZE >= g->width || grid_cursor->y/GRID_CELL_SIZE >= g->height) return 1;
    g->open[(grid_cursor->x/GRID_CELL_SIZE)+(g->width*(grid_cursor->y/GRID_CELL_SIZE))] = 'r';
    unsigned num = g->grid[grid_cursor->x/GRID_CELL_SIZE+g->width*(grid_cursor->y/GRID_CELL_SIZE)];
    switch (num) {
        case 0: 
            color->r = 255;
            color->g = 255;
            color->b = 255;
            break;
        case 1:
            color->r = 239;
            color->g = 160;
            color->b = 166;
            break;
        case 2:
            color->r = 225;
            color->g = 104;
            color->b = 107;
            break;
        case 3:
            color->r = 208;
            color->g =  69;
            color->b = 75;
            break;
        case 4:
            color->r = 202;
            color->g = 31;
            color->b = 29;
            break;
        case 5:
            color->r = 172;
            color->g = 49;
            color->b = 54;
            break;
        case 6:
            color->r = 159;
            color->g = 17;
            color->b = 17;
            break;
        case 7:
            color->r = 112;
            color->g = 8;
            color->b = 8;
            break;
        case 8:
            color->r = 60;
            color->g = 5;
            color->b = 5;
            break;
        case 10:
            color->r = 15;
            color->g = 10;
            color->b = 222;
            return 0;
            break;
    }
    SDL_SetRenderDrawColor(rend, color->r, color->g, color->b, 0);
    SDL_RenderFillRect(rend, grid_cursor);

    if(num == 0) {
        int i = grid_cursor->x - GRID_CELL_SIZE, j = grid_cursor->y - GRID_CELL_SIZE;
        for(int n = 0; n < 9; n++, i+=GRID_CELL_SIZE) {
            if(i/GRID_CELL_SIZE >= 0 && j/GRID_CELL_SIZE >= 0 && i/GRID_CELL_SIZE < g->width && j/GRID_CELL_SIZE < g->height && g->open[i/GRID_CELL_SIZE+g->width*(j/GRID_CELL_SIZE)] != 'r') {
                grid_cursor->x = i;
                grid_cursor->y = j;
                drawDigit(rend, grid_cursor, color, g);
            }
            if(n == 2 || n == 5) {
             j += GRID_CELL_SIZE;
             i -= 3*GRID_CELL_SIZE;
        }
        }
    }
    return 1;
}

void setMenu(SDL_Renderer *rend, grid* g, SDL_Color *grid_menu_color, SDL_Rect *grid_menu) {
    SDL_SetRenderDrawColor(rend, grid_menu_color->r, grid_menu_color->g, grid_menu_color->b, 0);
    for (int i = 0; i < 4; i ++) {
        if(i == 3) grid_menu->w -= 4;

        SDL_RenderFillRect(rend, grid_menu);
        grid_menu->x += GRID_CELL_SIZE*(g->width/4);
        if(i == 3) {
            grid_menu->w = GRID_CELL_SIZE * (g->width/4)-4;
            grid_menu->x = 4;
        }
    }
}

void reset(grid* g, SDL_Renderer* rend) {
    g = getGrid(true);
    SDL_RenderClear(rend);
    grid h = *g;
    g = &h;
}

void useMenu(grid* g, SDL_Renderer* rend, int x, int y) {
    if(x < g->width/4) saveGame(g);
    else if(x < g->width/2) reset(g, rend);
    else if(x < (g->width/4)*3) {                   //mode switch
        if(!g->mode)g->mode = 1;
        else g->mode = 0;
    }
    else if(x < g->width) {;}
    return;
}