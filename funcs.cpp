#include "funcs.h"

bool yorN() {
    char choice;
    while(1) {
        scanf("%c", &choice);
        getchar();
        if (choice == 'Y' || choice == 'y') {
            return true;
        }else if(choice == 'N' || choice == 'n') {
            return false;
        }else {
            printf("Not a valid answer - try again\n");
        }
    }
}

void Grid::printHidden() {
    printf("%d\n%d\n%d\n", width, height, nmines);
    printf("\n   ");
    for(int i = 1; i <= width; i++) {
        if(i < 10) printf("%d  ", i);
        else printf("%d ", i);
    }
    printf("\n");
    for(int y = 0; y < height; y++) {
        if(y+1 < 10) printf("%d  ", y+1);
        else printf("%d ", y+1);
        for(int x = 0; x < width; x++) {
            if(grid[x+(width*y)] == 10) {
                printf("10 ");                            //•
            }
            else printf("%u  ", grid[x+(width*y)]);
        }
        printf("\n");
    }
}

void Grid::printGrid() {
    printHidden();
    printf("\n\n   ");
        for(int i = 1; i <= width; i++) {
            if(i < 10) printf("%d  ", i);
            else printf("%d ", i);
        }
        printf("\n");
        for(int y = 0; y < height; y++) {
            if(y+1 < 10) printf("%d  ", y+1);
            else printf("%d ", y+1);
            for(int x = 0; x < width; x++) {
                if(open[x+width*y] == 'x') printf("x  ");
                else if(open[x+width*y] == 'f') printf("f  ");
                else printf("%c  ", open[x+width*y]);
            }
            printf("\n");
        }
        printf("Flags: %u\n", flags);
}

Grid* getGrid() {
    Grid helper;
    FILE* save;
    if (!access("save.bin", F_OK)) {
        printf("Save file detected - do you want to continue your game? (Y/n)\n");
        if(yorN()) {
            save = fopen("save.bin", "rb");
            fread(&helper, sizeof(Grid), 1, save);
            fclose(save);
            helper.printGrid();
            return new Grid(helper);
        }
    }

    unsigned w, h, m;
    if(yorN()) {
        while(1) {
            printf("Width: ");
            scanf("%u", &w);
            printf("Height: ");
            scanf("%u", &h);
            if(w*h > GRID_SIZE) {
                printf("Chosen playingfield is too big - pls set a smaller one\n");
                continue;
            }
            printf("Number of mines: ");
            scanf("%u", &m);
            if(m > w*h) {
                printf("Too many mines - pls select less than %u (this is depended on your chosen field size)\n", w*h);
                continue;
            }
            break;
        }
        if(w == 16 && h == 16 && m == 40) {
            printf("\nvery original\n");
        }
        getchar();
        return new Grid(w, h, m);
    }else {
        return new Grid();
    }
}

bool Grid::isStart(int current, int x, int y) {
    for(unsigned n = 0; n < 9; n++, y++) {
        if(x >= 0 && y >= 0 && x < width && y < height && current == x+width*y) {
            return true;
        }
        if(n == 2 || n == 5) {
            x++;
            y -= 3;
        }
    }
    return false;
}

void Grid::addNumbers(int x, int y) {
    for(unsigned n = 0; n < 9; n++, y++) {
        if(x >= 0 && y >= 0 && x < width && y < height && grid[x+width*y] != 10) {
            grid[x+width*y] += 1;
        }
        if(n == 2 || n == 5) {
            x++;
            y -= 3;
        }
    }
}

void Grid::buildGrids(unsigned x, unsigned y) {
    if(open[x+width*y] == 'f') return;
    unsigned current;
    srand(time(NULL));
    for(int i = 0 ; i < nmines; i++ ) {
        current = rand() % (width * height);
        if(grid[current] == 10 || isStart(current, x-1, y-1)) {
            i--;
            continue;
        }
        grid[current] = 10;
        //printf("%u", grid[current]);
        //printGrid();
        addNumbers(current % width-1, current/width-1);
    }
    first = false;
    
}

bool Grid::revealPos(SDL_Renderer *rend, SDL_Rect *grid_cursor) {
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f' || open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'r') return 1;
    return drawDigit(rend, grid_cursor);
}

void Grid::flagPos(SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor) {
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f') {                                        //remove flag
        open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] = 'x';
        flags++;
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderFillRect(rend, grid_cursor);
        return;
    }
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] != 'x') {                                        //return if already revealed
        return;
    }
    if(flags <= 0) {                                                                                         //return if no flags left
        return;
    }
                                                                                                                //execute putting in flag (green)
    SDL_SetRenderDrawColor(rend, 46, 204, 113, 0);
    SDL_RenderFillRect(rend, grid_cursor);
    open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] = 'f';
    flags--;
    return;
}

void gameOver(int x, int y) {
    printf("\nPosition (%d,%d) killed you", x, y);
}

//void victory(SDL_Renderer* rend,Grid* g, SDL_Surface* win){}

void Grid::saveGame() {
    if(first) {
        printf("You cant save an unloaded game - pls start playing first\n");
        return;
    }
    Grid h = *this;
    FILE* save;
    save = fopen("save.bin", "wb");
    if (save == NULL) {
        printf("The file is not opened - try again");
        return;
    }
    fwrite(&h, sizeof(grid), 1, save);
    fclose(save);
    printf("Your game was saved successfully");
}

bool Grid::drawDigit(SDL_Renderer* rend, SDL_Rect* grid_cursor) {
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f') flags++;
    if(grid_cursor->x/GRID_CELL_SIZE < 0 || grid_cursor->y/GRID_CELL_SIZE < 0 || grid_cursor->x/GRID_CELL_SIZE >= width || grid_cursor->y/GRID_CELL_SIZE >= height) return 1;
    open[(grid_cursor->x/GRID_CELL_SIZE)+(width*(grid_cursor->y/GRID_CELL_SIZE))] = 'r';
    if(grid[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)]==10) return 0;

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 0);
    SDL_RenderFillRect(rend, grid_cursor);

    TextBox t = TextBox(*grid_cursor, {0, 0, 0, 0}, {255,255,255,0}, to_string(grid[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)])); 
    t.drawTextBox(rend);

    SDL_RenderCopy(rend, t.text, NULL, t.getArea());
    SDL_RenderPresent(rend);

    fieldstoreveal--;

    if(grid[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 0) {
        int i = grid_cursor->x - GRID_CELL_SIZE, j = grid_cursor->y - GRID_CELL_SIZE;
        for(int n = 0; n < 9; n++, i+=GRID_CELL_SIZE) {
            if(i/GRID_CELL_SIZE >= 0 && j/GRID_CELL_SIZE >= 0 && i/GRID_CELL_SIZE < width && j/GRID_CELL_SIZE < height && open[i/GRID_CELL_SIZE+width*(j/GRID_CELL_SIZE)] != 'r') {
                grid_cursor->x = i;
                grid_cursor->y = j;
                drawDigit(rend, grid_cursor);
            }
            if(n == 2 || n == 5) {
             j += GRID_CELL_SIZE;
             i -= 3*GRID_CELL_SIZE;
        }
        }
    }
    return 1;
}

/*void Grid::setMenu(SDL_Renderer *rend, SDL_Color *grid_menu_color, SDL_Rect *grid_menu) {
    SDL_SetRenderDrawColor(rend, grid_menu_color->r, grid_menu_color->g, grid_menu_color->b, 0);
    for (int i = 0; i < 4; i ++) {
        if(i == 3) grid_menu->w -= 4;

        SDL_RenderFillRect(rend, grid_menu);
        grid_menu->x += GRID_CELL_SIZE*(width/4);
        if(i == 3) {
            grid_menu->w = GRID_CELL_SIZE * (width/4)-4;
            grid_menu->x = 4;
        }
    }
}*/

void Grid::reset(SDL_Renderer* rend) {
    SDL_Rect grid_resetter = {
        .x = 0,
        .y = 0,
        .w = GRID_CELL_SIZE * width,
        .h = GRID_CELL_SIZE * height,
    };
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 0);
    SDL_RenderFillRect(rend, &grid_resetter);
    printf("\n\nParameters resetting to default...\n\nDo you want to set your own parameters? (Y/n)\n");
    unsigned w, h, m;
    if(yorN()) {
        while(1) {
            printf("Width: ");
            scanf("%u", &w);
            printf("Height: ");
            scanf("%u", &h);
            if(w*h > GRID_SIZE) {
                printf("Chosen playingfield is too big - pls set a smaller one\n");
                continue;
            }
            printf("Number of mines: ");
            scanf("%u", &m);
            if(m > w*h) {
                printf("Too many mines - pls select less than %u (this is depended on your chosen field size)\n", w*h);
                continue;
            }
            break;
        }
        if(w == 16 && h == 16 && m == 40) {
            printf("\nvery original\n");
        }
        getchar();
        width = w;
        height = h;
        nmines = m;
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
    }else {
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
}

/*
void Menu::useMenu(SDL_Renderer* rend, Grid* g, int x, int y) {
    if(x < g->getWidth()/4) g->saveGame();
    else if(x < g->getWidth()/2) g->reset(rend);
    else if(x < (g->getWidth()/4)*3) {                   //mode switch
        if(!g->getMode())g->setMode(1);
        else g->setMode(0);
    }
    else if(x < g->getWidth()) {exit(1);}
    return;
}*/




//----------TextBox functions----------

void TextBox::drawTextBox(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, area_color.r, area_color.g, area_color.b, area_color.a);
    SDL_RenderFillRect(rend, &area);
    SDL_Surface* surface = TTF_RenderText_Solid(font, name.c_str(), text_color);
    //if(surface->w > area.w) surface->w = area.w;
    //if(surface->h > area.h) surface->h = area.h;
    text = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(rend, text, NULL, &text_rect);
    //cout << endl << text_rect.x << " " << text_rect.y << " " << text_rect.w << " " << text_rect.h;
}

SDL_Rect* TextBox::getArea() {
    return &area;
}

bool TextBox::setAreaVar(char var, int n) {
    switch(var) {
        case 'x':
            area.x = n;
            text_rect.x = area.x+(area.w/5);
            break;
        case 'y':
            area.y = n;
            text_rect.y = area.y+(area.y/5);
            break;
        case 'w':
            area.w = n;
            text_rect.w = area.w+(area.w/5)*2;
            break;
        case 'h':
            area.h = n;
            text_rect.h = area.h+(area.h/5)*2;
            break;
        default:
            cout << "wrong area variable";
            return 0;
    }
    return 1;
}

int TextBox::getAreaVar(char var) {
    switch (var) {
        case 'x':
            return area.x;
        case 'y':
            return area.y;
        case 'w':
            return area.w;
        case 'h':
            return area.h;
        default:
            cout << "wrong area variable";
            return 0;
    }
}