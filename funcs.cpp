#include "funcs.h"

bool yorN(SDL_Renderer* rend, int w, int h, string question) {
    SDL_Rect choice_area {
        .x = GRID_CELL_SIZE*((w/2)-3),
        .y = GRID_CELL_SIZE*((h/2)-2),
        .w = GRID_CELL_SIZE*5,
        .h = GRID_CELL_SIZE*3,
    };
    if(w%2) choice_area.x +=(GRID_CELL_SIZE/2);
    if(h%2) choice_area.y +=(GRID_CELL_SIZE/2);
    Box choice_box = Box(choice_area, "");
    choice_box.drawBox(rend);

    SDL_Rect choice_question {
        .x = choice_area.x,
        .y = choice_area.y,
        .w = choice_area.w,
        .h = choice_area.h/2,
    };
    Box question_box = Box(choice_question, question);
    question_box.drawBox(rend);


    SDL_Rect choice_yes {
        .x = GRID_CELL_SIZE*((w/2)-2),
        .y = GRID_CELL_SIZE*((h/2)-1),
        .w = GRID_CELL_SIZE*4,
        .h = GRID_CELL_SIZE*2,
    };
    SDL_Rect choice_no {
        .x = GRID_CELL_SIZE*((w/2)-2),
        .y = GRID_CELL_SIZE*((h/2)-1),
        .w = GRID_CELL_SIZE*4,
        .h = GRID_CELL_SIZE*2,
    };
    scanf("%d", &w);
    return 0;
}

int useMenu(SDL_Renderer* rend, Grid* g, int x, int y) {
    if(x < g->getWidth()/4) {                                       //flags on
        return 1;
    }
    if(x < g->getWidth()/2) {                                 //restart
        return 2;
    }
    if(x < (g->getWidth()/4)*3) {                             //save game
        //cout << g->getWidth() << " " << (g->getWidth()/4)*3;
        return 3;
    }
    if(x < g->getWidth()) {                                   //load game
        //cout << g->getWidth() << " " << (g->getWidth()/4)*3;
        return 4;
    }
    return 0;
}

void reloadGrid(SDL_Renderer* rend, Grid* g) {
    SDL_Rect cell = {
        .x = 0,
        .y = 0,
        .w = GRID_CELL_SIZE,
        .h = GRID_CELL_SIZE,
    };
    SDL_Color cell_color = {90,90,90};
    Box Cell = Box(cell, cell_color, cell_color, " ");
    Cell.border_color = {180, 180, 180};

    for(int i = 0; i < g->getWidth(); i++) {
        for(int j = 0; j < g->getHeight(); j++) {
            if(g->getOpenPos(i,j) == 'r') {
                cell.x = i*GRID_CELL_SIZE;
                cell.y = j*GRID_CELL_SIZE;
                g->drawDigit(rend, &cell);
            }else if(g->getOpenPos(i,j) == 'f') {
                SDL_Rect flag = {
                    .x = Cell.getAreaVar('x'),
                    .y = Cell.getAreaVar('y'),
                    .w = GRID_CELL_SIZE,
                    .h = GRID_CELL_SIZE,
                };
                SDL_Color cell_color = {90,90,90};
                Box Flag = Box(flag, cell_color, cell_color, "F");
                Flag.text_color = {255, 0, 0};
                Flag.border_color = {180, 180, 180};
                Flag.drawBox(rend);
            }else {
                Cell.drawBox(rend);
            }
            Cell.setAreaVar('y', Cell.getAreaVar('y')+GRID_CELL_SIZE);
        }
        Cell.setAreaVar('x', Cell.getAreaVar('x')+GRID_CELL_SIZE);
        Cell.setAreaVar('y', Cell.getAreaVar('y')-GRID_CELL_SIZE*g->getHeight());
    }
    SDL_Color grid_line_color = {44, 44, 44, 0};
    SDL_SetRenderDrawColor(rend, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);

    for (int i = 0; i < 1 + g->getWidth() * GRID_CELL_SIZE; i += GRID_CELL_SIZE) {
        SDL_RenderDrawLine(rend, i, 0, i, g->getHeight()*GRID_CELL_SIZE);
    }

    for (int i = 0; i < 1 + g->getHeight() * GRID_CELL_SIZE; i += GRID_CELL_SIZE) {
        SDL_RenderDrawLine(rend, 0, i, g->getWidth()*GRID_CELL_SIZE, i);
    }
}

void gameOver(int x, int y) {
    cout << "\nPosition (" << x << "," << y << ") killed you\n";
}

Grid* loadGame(SDL_Renderer* rend) {
    Grid helper;
    FILE* save;
    save = fopen("save.bin", "rb");
    fread(&helper, sizeof(Grid), 1, save);
    helper.setFirst(false);
    helper.resetFieldstoreveal();
    fclose(save);
    return new Grid(helper);
}

void setMenu(SDL_Renderer* rend, Grid* g) {
    SDL_Rect menu_area = {
        .x = 0,
        .y = GRID_CELL_SIZE*g->getHeight(),
        .w = ((g->getWidth()*GRID_CELL_SIZE)/4),
        .h = GRID_CELL_SIZE,
    };
    SDL_Color menu_text_color = {0,0,0,0};
    SDL_Color menu_area_color = {255,255,255,0};

    Box *menu = new Box(menu_area, menu_text_color, menu_area_color, "Set size");
    menu->border_thickness = 4;
    menu->border_color = {90, 90, 90};
    menu->drawBox(rend);
    
    menu->setAreaVar('x', menu->getAreaVar('x')+(g->getWidth()*GRID_CELL_SIZE)/4);
    menu->name = "Restart";
    menu->drawBox(rend);

    menu->setAreaVar('x', menu->getAreaVar('x')+(g->getWidth()*GRID_CELL_SIZE)/4);
    menu->name = "Save";
    menu->drawBox(rend);

    menu->setAreaVar('x', menu->getAreaVar('x')+(g->getWidth()*GRID_CELL_SIZE)/4);
    menu->name = "Load";
    menu->drawBox(rend);
}

void setParams() {
    ;
}


//----------Grid functions----------

void Grid::saveGame() {
    Grid h = Grid(*this);
    h.first = false;
    FILE* save;
    save = fopen("save.bin", "wb");
    fwrite(&h, sizeof(Grid), 1, save);
    fclose(save);
}

void Grid::printHidden() {
    //printf("%d\n%d\n%d\n", width, height, nmines);
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

SDL_bool Grid::revealPos(SDL_Renderer *rend, SDL_Rect *grid_cursor) {
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f' || open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'r') return SDL_FALSE;
    bool b = drawDigit(rend, grid_cursor);
    if(b) return SDL_FALSE;
    else return SDL_TRUE;
}

void Grid::flagPos(SDL_Renderer *rend, SDL_Color *grid_cursor_color, SDL_Rect *grid_cursor, SDL_Texture* flagimg) {
    SDL_Color grid_line_color = {44, 44, 44, 0};
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] == 'f') {                                        //remove flag
        open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] = 'x';
        flags++;
        SDL_Rect cell = {
        .x = grid_cursor->x,
        .y = grid_cursor->y,
        .w = GRID_CELL_SIZE,
        .h = GRID_CELL_SIZE,
        };
        SDL_Color cell_color = {90,90,90};
        Box Cell = Box(cell, cell_color, cell_color, "");
        Cell.border_color = {180, 180, 180};
        Cell.drawBox(rend);
        
        SDL_SetRenderDrawColor(rend, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);
        SDL_RenderDrawLine(rend, Cell.getAreaVar('x'), Cell.getAreaVar('y'), Cell.getAreaVar('x')+GRID_CELL_SIZE, Cell.getAreaVar('y'));
        SDL_RenderDrawLine(rend, Cell.getAreaVar('x')+GRID_CELL_SIZE, Cell.getAreaVar('y'), Cell.getAreaVar('x')+GRID_CELL_SIZE, Cell.getAreaVar('y')+GRID_CELL_SIZE);
        SDL_RenderDrawLine(rend, Cell.getAreaVar('x'), Cell.getAreaVar('y'), Cell.getAreaVar('x'), Cell.getAreaVar('y')+GRID_CELL_SIZE);
        SDL_RenderDrawLine(rend, Cell.getAreaVar('x'), Cell.getAreaVar('y')+GRID_CELL_SIZE, Cell.getAreaVar('x')+GRID_CELL_SIZE, Cell.getAreaVar('y')+GRID_CELL_SIZE);
        return;
    }
    if(open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] != 'x') {                                        //return if already revealed
        return;
    }
    if(flags <= 0) {                                                                                         //return if no flags left
        return;
    }
    SDL_Rect flag = {
        .x = grid_cursor->x,
        .y = grid_cursor->y,
        .w = GRID_CELL_SIZE,
        .h = GRID_CELL_SIZE,
    };
    SDL_Color cell_color = {90,90,90};
    Box Flag = Box(flag, cell_color, cell_color, "F");
    Flag.text_color = {255, 0, 0};
    Flag.border_color = {180, 180, 180};
    Flag.drawBox(rend);

    SDL_SetRenderDrawColor(rend, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);
        SDL_RenderDrawLine(rend, Flag.getAreaVar('x'), Flag.getAreaVar('y'), Flag.getAreaVar('x')+GRID_CELL_SIZE, Flag.getAreaVar('y'));
        SDL_RenderDrawLine(rend, Flag.getAreaVar('x')+GRID_CELL_SIZE, Flag.getAreaVar('y'), Flag.getAreaVar('x')+GRID_CELL_SIZE, Flag.getAreaVar('y')+GRID_CELL_SIZE);
        SDL_RenderDrawLine(rend, Flag.getAreaVar('x'), Flag.getAreaVar('y'), Flag.getAreaVar('x'), Flag.getAreaVar('y')+GRID_CELL_SIZE);
        SDL_RenderDrawLine(rend, Flag.getAreaVar('x'), Flag.getAreaVar('y')+GRID_CELL_SIZE, Flag.getAreaVar('x')+GRID_CELL_SIZE, Flag.getAreaVar('y')+GRID_CELL_SIZE);
                                                                                                                //execute putting in flag (green)
    open[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)] = 'f';
    flags--;
    return;
}

bool Grid::drawDigit(SDL_Renderer* rend, SDL_Rect* grid_cursor) {
    if(grid_cursor->x/GRID_CELL_SIZE < 0 || grid_cursor->y/GRID_CELL_SIZE < 0 || grid_cursor->x/GRID_CELL_SIZE >= width || grid_cursor->y/GRID_CELL_SIZE >= height) return 1;
    open[(grid_cursor->x/GRID_CELL_SIZE)+(width*(grid_cursor->y/GRID_CELL_SIZE))] = 'r';
    if(grid[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)]==10) return 0;

    Box t = Box(*grid_cursor, {0, 0, 0, 0}, {180,180,180, 0}, to_string(grid[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)])); 
    t.border_color = {};
    t.border_thickness = 0;
    t.area_color = {80,80,80};

    switch(grid[grid_cursor->x/GRID_CELL_SIZE+width*(grid_cursor->y/GRID_CELL_SIZE)]) {
        case 0:
            t.name = "";
            break;
        case 1:
            t.text_color = {0, 0, 255};
            break;
        case 2:
            t.text_color = {0, 128, 0};
            break;
        case 3:
            t.text_color = {255, 0, 0};
            break;
        case 4:
            t.text_color = {128, 0, 128};
            break;
        case 5:
            t.text_color = {128, 0, 0};
            break;
        case 6:
            t.text_color = {64, 224, 208};
            break;
        case 7:
            t.text_color = {0,0,0};
            break;
        case 8:
            t.text_color = {128, 128, 128};
        default:
            break;
    }
    t.area_color = {120, 120, 120};
    t.drawBox(rend);
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


//----------Box functions----------

void Box::drawBox(SDL_Renderer *rend) {
    SDL_SetRenderDrawColor(rend, area_color.r, area_color.g, area_color.b, area_color.a);
    SDL_RenderFillRect(rend, &area);

    SDL_SetRenderDrawColor(rend, border_color.r, border_color.g, border_color.b, border_color.a);
    for(int i = 0; i <= border_thickness; i++) {
        SDL_RenderDrawLine(rend, area.x+i, area.y, area.x+i, area.y+area.h);
        SDL_RenderDrawLine(rend, area.x+area.w-i, area.y, area.x+area.w-i, area.y+area.h);

        SDL_RenderDrawLine(rend, area.x, area.y+i, area.x+area.w, area.y+i);
        SDL_RenderDrawLine(rend, area.x, area.y+area.h-i, area.x+area.w, area.y+area.h-i);
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, name.c_str(), text_color);
    //if(surface->w > area.w) text_rect.w = area.w;
    //if(surface->h > area.h) text_rect.h = area.h;
    text = SDL_CreateTextureFromSurface(rend, surface);
    SDL_FreeSurface(surface);
    SDL_RenderCopy(rend, text, NULL, &text_rect);
    //cout << endl << text_rect.w << " " << text_rect.h << " " << text_rect.w << " " << text_rect.h;
}

SDL_Rect* Box::getArea() {
    return &area;
}

bool Box::setAreaVar(char var, int n) {
    switch(var) {
        case 'x':
            area.x = n;
            break;
        case 'y':
            area.y = n;
            break;
        case 'w':
            area.w = n;
            break;
        case 'h':
            area.h = n;
            break;
        default:
            cout << "wrong area variable";
            return 0;
    }
    text_rect = {
            .x = area.x+(area.w/5),
            .y = area.y+(area.h/5),
            .w = area.w-(area.w/5)*2,
            .h = area.h-(area.h/5)*2,
        };
    return 1;
}

int Box::getAreaVar(char var) {
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