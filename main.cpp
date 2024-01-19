#include "funcs.h"


int main(int argc, char** argv) {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        cout << "Error initializing SDL: " << SDL_GetError() << endl;
        return 0;
    }
    if ( TTF_Init() < 0 ) {
	    cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
        return 0;
    }
    
    Grid *g = getGrid();
    
    int window_width = (g->getWidth() * GRID_CELL_SIZE) + 1;
    int window_height = (g->getWidth() * GRID_CELL_SIZE) + 1;

    SDL_Window* win = SDL_CreateWindow("Minesweeper", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    g->getWidth()*GRID_CELL_SIZE, (1+g->getWidth())*GRID_CELL_SIZE, 0);
    SDL_SetWindowAlwaysOnTop(win, SDL_TRUE);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Rect menu_area = {
        .x = 0,
        .y = GRID_CELL_SIZE*g->getHeight(),
        .w = ((g->getWidth()*GRID_CELL_SIZE)/4),
        .h = GRID_CELL_SIZE,
    };
    SDL_Color menu_text_color = {0,0,0,0};
    SDL_Color menu_area_color = {255,255,255,0};

    
    TextBox *menu = new TextBox(menu_area, menu_text_color, menu_area_color, "Save");
    menu->get_text_and_rect(rend);
    
   

    menu->area.x += (g->getWidth()*GRID_CELL_SIZE)/4;
    menu->name = "Reset";
    menu->get_text_and_rect(rend);

    menu->area.x += (g->getWidth()*GRID_CELL_SIZE)/4;
    menu->name = "Flag";
    menu->get_text_and_rect(rend);

    menu->area.x += (g->getWidth()*GRID_CELL_SIZE)/4;
    menu->name = "Exit";
    menu->get_text_and_rect(rend);

    
    SDL_Color grid_line_color = {44, 44, 44, 0}; // Dark grey
    //SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
    //SDL_Color grid_menu_color = {255, 255, 255, 255}; // White

    /*
    SDL_Rect grid_cursor_ghost = {
        grid_cursor.x,
        grid_cursor.y,
        GRID_CELL_SIZE, GRID_CELL_SIZE
    };*/

    SDL_Rect grid_backround = {
        .x = 0,
        .y = 0,
        .w = g->getWidth()*GRID_CELL_SIZE,
        .h = g->getHeight()*GRID_CELL_SIZE,
    };
    SDL_Color grid_background = {22, 22, 22, 255}; // Barely Black
    SDL_SetRenderDrawColor(rend, grid_background.r, grid_background.g, grid_background.b, grid_background.a);
    SDL_RenderFillRect(rend, &grid_backround);

    SDL_Rect grid_cursor = {
        .x = (g->getWidth() - 1) / 2 * GRID_CELL_SIZE,
        .y = (g->getWidth() - 1) / 2 * GRID_CELL_SIZE,
        .w = GRID_CELL_SIZE,
        .h = GRID_CELL_SIZE,
    };
    SDL_Color grid_cursor_color = {255, 255, 255, 0}; // White

    SDL_bool quit = SDL_FALSE; 
    SDL_bool mouse_active = SDL_FALSE;
    SDL_bool mouse_hover = SDL_FALSE;
    //SDL_bool defeat = SDL_FALSE;
    SDL_bool game_victory = SDL_FALSE;
    SDL_bool mouse_click = SDL_FALSE;
    bool b = 1;

    
    SDL_Surface *window_surf = SDL_GetWindowSurface( win );
    /* this return the color of a specific pixel on the window, i didnt need it
    if(!window_surf) {
        printf("%s", SDL_GetError());
        return;
    }
    const Uint8 Bpp = window_surf->format->BytesPerPixel;
    Uint8* pPixel = (Uint8*)window_surf->pixels + 0 * window_surf->pitch + 0 * Bpp;
    Uint32 PixelData = *(Uint32*)pPixel;
    SDL_Color Color = {34, 4, 65, 34};
    
    SDL_GetRGBA(PixelData, window_surf->format, &Color.r, &Color.g, &Color.b, &Color.a);
    printf("\n%d, %d, %d, %d\n", Color.r, Color.g, Color.b, Color.a);
    */

    while (!quit) {
        SDL_SetWindowAlwaysOnTop(win, SDL_FALSE);
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                grid_cursor.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                grid_cursor.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                mouse_click = SDL_TRUE;
                break;
            case SDL_MOUSEMOTION:
                //grid_cursor_ghost.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                //grid_cursor_ghost.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;

                if (!mouse_active)
                    mouse_active = SDL_TRUE;
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                    mouse_hover = SDL_TRUE;
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                    mouse_hover = SDL_FALSE;
                break;
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            }
        }
        if(game_victory == SDL_FALSE) {
        //SDL_SetRenderDrawColor(rend, grid_background.r, grid_background.g, grid_background.b, grid_background.a);
        //SDL_RenderClear(rend);

        // Draw grid lines.
        SDL_SetRenderDrawColor(rend, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);

        for (int i = 0; i < 1 + g->getWidth() * GRID_CELL_SIZE; i += GRID_CELL_SIZE) {
            SDL_RenderDrawLine(rend, i, 0, i, window_height);
        }

        for (int i = 0; i < 1 + g->getWidth() * GRID_CELL_SIZE; i += GRID_CELL_SIZE) {
            SDL_RenderDrawLine(rend, 0, i, window_width, i);
        }

        SDL_RenderDrawLine(rend, ((g->getWidth()*GRID_CELL_SIZE)/4), g->getHeight()*GRID_CELL_SIZE, ((g->getWidth()*GRID_CELL_SIZE)/4), g->getHeight()*GRID_CELL_SIZE+GRID_CELL_SIZE);
        SDL_RenderDrawLine(rend, ((g->getWidth()*GRID_CELL_SIZE)/4)*2, g->getHeight()*GRID_CELL_SIZE, ((g->getWidth()*GRID_CELL_SIZE)/4)*2, g->getHeight()*GRID_CELL_SIZE+GRID_CELL_SIZE);
        SDL_RenderDrawLine(rend, ((g->getWidth()*GRID_CELL_SIZE)/4)*3, g->getHeight()*GRID_CELL_SIZE, ((g->getWidth()*GRID_CELL_SIZE)/4)*3, g->getHeight()*GRID_CELL_SIZE+GRID_CELL_SIZE);
        
        
        //g->setMenu(rend, &grid_menu_color, &grid_menu);
/*
        SDL_SetRenderDrawColor(rend, 0, 0, 0, 130);

        for (int i = 0; i < 1 + (g->getWidth() * GRID_CELL_SIZE)/4; i += GRID_CELL_SIZE*(g->getWidth()/4)) {
            SDL_RenderDrawLine(rend, i, g->getHeight() *GRID_CELL_SIZE, i, window_height);
        }*/

        // Draw grid ghost cursor.
        /*if (mouse_active && mouse_click) {
            SDL_SetRenderDrawColor(rend, grid_cursor_ghost_color.r, grid_cursor_ghost_color.g, grid_cursor_ghost_color.b, grid_cursor_ghost_color.a);
            SDL_RenderFillRect(rend, &grid_cursor_ghost);
        }*/

        // clicking on the playing field
        if(mouse_click == SDL_TRUE && (grid_cursor.x/GRID_CELL_SIZE) < g->getWidth() && (grid_cursor.y/GRID_CELL_SIZE) < g->getHeight() ) {
            //printf("%u, %u\n", grid_cursor.x, grid_cursor.y);
            if(!g->getMode()) {
                if(g->getFirst()) g->buildGrids(grid_cursor.x/GRID_CELL_SIZE, grid_cursor.y/GRID_CELL_SIZE);
                b = g->revealPos(rend, &grid_cursor);
            } else {
                g->flagPos(rend, &grid_cursor_color, &grid_cursor);
            }
            //g->printGrid();
        }

        // clicking on the menu
        /*if(mouse_click == SDL_TRUE && (grid_cursor.x/GRID_CELL_SIZE) < g->getWidth() &&  (grid_cursor.y/GRID_CELL_SIZE) > (g->getHeight() -1)) {
            g->useMenu(rend, grid_cursor.x/GRID_CELL_SIZE, grid_cursor.y/GRID_CELL_SIZE);
            g->printGrid();
        }*/

        
        //SDL_RenderCopy(rend, button2->text, NULL, &(button2->area));

        mouse_click = SDL_FALSE;
        SDL_RenderPresent(rend);
        if(quit == SDL_TRUE) {
            SDL_Quit();
        }
        if(!b) {
            gameOver(grid_cursor.x/GRID_CELL_SIZE, grid_cursor.y/GRID_CELL_SIZE);
            break;
        }
        if(!g->getFieldtoreveal()) {

            game_victory = SDL_TRUE;
            cout << endl << "GOOD JOB" << endl << endl;
            //SDL_FillRect(window_surf, &victory_screen, 0);
            
        }
        }
        if(game_victory == SDL_TRUE) {
            SDL_RenderClear(rend);
            victory(rend, g, window_surf);
            
        }

    }

    //SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}