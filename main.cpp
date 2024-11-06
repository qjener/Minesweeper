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
    if ( IMG_Init(IMG_INIT_PNG) < 0 ) {
	    cout << "Error initializing SDL_ttf: " << TTF_GetError() << endl;
        return 0;
    }
    
    SDL_Window* win = SDL_CreateWindow("Minesweeper", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    16*GRID_CELL_SIZE, (16+1)*GRID_CELL_SIZE, 0); 

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    SDL_Texture* flagimg = IMG_LoadTexture(rend, "visuals/flag.png");
    SDL_Texture* grey_mine = IMG_LoadTexture(rend, "visuals/grey_mine.png");
    SDL_Texture* red_mine = IMG_LoadTexture(rend, "visuals/red_mine.png");


    Grid *g = new Grid();
    
    SDL_SetWindowSize(win, g->getWidth()*GRID_CELL_SIZE, (1+g->getHeight())*GRID_CELL_SIZE);

    setMenu(rend, g);

    SDL_Color grid_line_color = {44, 44, 44, 0}; // Dark grey
    //SDL_Color grid_cursor_ghost_color = {44, 44, 44, 255};
    //SDL_Color grid_menu_color = {255, 255, 255, 255}; // White

    SDL_SetRenderDrawColor(rend, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);
    SDL_RenderDrawLine(rend, (g->getWidth()*GRID_CELL_SIZE)/4, (g->getHeight())*GRID_CELL_SIZE, (g->getWidth()*GRID_CELL_SIZE)/4, (1+g->getHeight())*GRID_CELL_SIZE);
    SDL_RenderDrawLine(rend, (g->getWidth()*GRID_CELL_SIZE)/2, (g->getHeight())*GRID_CELL_SIZE, (g->getWidth()*GRID_CELL_SIZE)/2, (1+g->getHeight())*GRID_CELL_SIZE);
    SDL_RenderDrawLine(rend, ((g->getWidth()*GRID_CELL_SIZE)/4)*2, (g->getHeight())*GRID_CELL_SIZE, ((g->getWidth()*GRID_CELL_SIZE)/4)*2, (1+g->getHeight())*GRID_CELL_SIZE);

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
    SDL_Point cursor;

    SDL_bool quit = SDL_FALSE; 
    //SDL_bool mouse_active = SDL_FALSE;
    //SDL_bool mouse_hover = SDL_FALSE;
    SDL_bool game_defeat = SDL_FALSE;
    SDL_bool game_victory = SDL_FALSE;
    SDL_bool mouse_click = SDL_FALSE;
    SDL_bool menu_clicked = SDL_FALSE;
    SDL_bool reload = SDL_FALSE;
    SDL_bool set_size = SDL_FALSE;
    
    int menu_choice;


    SDL_Rect game_end = {
        .x = GRID_CELL_SIZE*((g->getWidth()/2)-2),
        .y = GRID_CELL_SIZE*((g->getHeight()/2)-1),
        .w = GRID_CELL_SIZE*4,
        .h = GRID_CELL_SIZE*2,
    };
    if(g->getWidth()%2) game_end.x +=(GRID_CELL_SIZE/2);
    if(g->getHeight()%2) game_end.y +=(GRID_CELL_SIZE/2);
    Box end = Box(game_end, "End");


    SDL_Rect menu_msg = {
        .x = 0,
        .y = 0,
        .w = 0,
        .h = 0,
    };
    if(g->getWidth()%2) menu_msg.x +=(GRID_CELL_SIZE/2);
    if(g->getHeight()%2) menu_msg.y +=(GRID_CELL_SIZE/2);
    Box Menu_Msg = Box(menu_msg, "Error");
    Box Menu_Msg2 = Box(Menu_Msg);

    int menu_button_width = (g->getWidth()*GRID_CELL_SIZE)/4;
    SDL_Rect grid_cursor_menu = {
        grid_cursor.x,
        grid_cursor.y,
        menu_button_width, GRID_CELL_SIZE
    };



    reloadGrid(rend, g, flagimg);
    

    /*this returns the color of a specific pixel on the window, i didnt need it

    SDL_Surface *window_surf = SDL_GetWindowSurface( win );
     
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
        //SDL_SetWindowAlwaysOnTop(win, SDL_FALSE);
        SDL_Event event;
        
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
                cursor.x = event.motion.x;
                cursor.y = event.motion.y;
                grid_cursor.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                grid_cursor.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                grid_cursor_menu.x = (event.motion.x / menu_button_width) * menu_button_width;
                mouse_click = SDL_TRUE;
                if(reload == SDL_TRUE) {
                    reload = SDL_FALSE;
                    reloadGrid(rend, g, flagimg);
                }
                if(event.button.button == SDL_BUTTON_LEFT) g->setMode(0);
                else g->setMode(1);
                break;/*
            case SDL_MOUSEMOTION:
                //grid_cursor_ghost.x = (event.motion.x / GRID_CELL_SIZE) * GRID_CELL_SIZE;
                //grid_cursor_ghost.y = (event.motion.y / GRID_CELL_SIZE) * GRID_CELL_SIZE;

                if (!mouse_active)
                    mouse_active = SDL_TRUE;
                break;*/
            /*case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_ENTER && !mouse_hover)
                    mouse_hover = SDL_TRUE;
                else if (event.window.event == SDL_WINDOWEVENT_LEAVE && mouse_hover)
                    mouse_hover = SDL_FALSE;
                break;*/
            case SDL_QUIT:
                quit = SDL_TRUE;
                break;
            }
        }
        
        //SDL_SetRenderDrawColor(rend, grid_background.r, grid_background.g, grid_background.b, grid_background.a);
        //SDL_RenderClear(rend);

        // Draw grid lines.
        //SDL_SetRenderDrawColor(rend, grid_line_color.r, grid_line_color.g, grid_line_color.b, grid_line_color.a);

        // Draw grid ghost cursor.
        /*if (mouse_active && mouse_click) {
            SDL_SetRenderDrawColor(rend, grid_cursor_ghost_color.r, grid_cursor_ghost_color.g, grid_cursor_ghost_color.b, grid_cursor_ghost_color.a);
            SDL_RenderFillRect(rend, &grid_cursor_ghost);
        }*/
        // clicking on the playing field
        if(game_victory == SDL_FALSE && set_size == SDL_FALSE && game_defeat == SDL_FALSE && menu_clicked == SDL_FALSE && mouse_click == SDL_TRUE && (grid_cursor.x/GRID_CELL_SIZE) < g->getWidth() && (grid_cursor.y/GRID_CELL_SIZE) < g->getHeight() ) {
                //printf("%u, %u\n", grid_cursor.x, grid_cursor.y);
            if(!g->getMode()) {
                if(g->getFirst()) {
                    g->buildGrids(grid_cursor.x/GRID_CELL_SIZE, grid_cursor.y/GRID_CELL_SIZE);
                }
                game_defeat = g->revealPos(rend, &grid_cursor);
            } else {
                g->flagPos(rend, &grid_cursor_color, &grid_cursor, flagimg);
            }
        }else if(mouse_click == SDL_TRUE && set_size == SDL_FALSE && (grid_cursor.x/GRID_CELL_SIZE) < g->getWidth() &&  (grid_cursor.y/GRID_CELL_SIZE) > (g->getHeight() -1)) {
            menu_choice = useMenu(rend, g, grid_cursor_menu.x, grid_cursor_menu.y);
            menu_clicked = SDL_TRUE;
            if(!menu_choice) {
                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Menu Error", "There was an error when clicking the menu", win);
                quit = SDL_TRUE;
            }
        }else if(mouse_click == SDL_TRUE && set_size == SDL_TRUE) {
            if(cursor.x < GRID_CELL_SIZE*(g->getWidth()/2)-2.5*GRID_CELL_SIZE || cursor.x > GRID_CELL_SIZE*(g->getWidth()/2)+2.5*GRID_CELL_SIZE || cursor.y < GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE || cursor.y > GRID_CELL_SIZE*(g->getHeight()/2)+2.5*GRID_CELL_SIZE) {
                set_size = SDL_FALSE;
                reloadGrid(rend, g, flagimg);
            }else if(cursor.x > GRID_CELL_SIZE*(g->getWidth()/2)-2.5*GRID_CELL_SIZE+20 || cursor.y < GRID_CELL_SIZE*(g->getWidth()/2)+2.5*GRID_CELL_SIZE-20) {
                if(cursor.y > GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE+18 && cursor.y < GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE+18+GRID_CELL_SIZE) {
                    g = new Grid(9, 9, 10);
                    SDL_SetWindowSize(win, g->getWidth()*GRID_CELL_SIZE, (1+g->getHeight())*GRID_CELL_SIZE);
                    reloadGrid(rend, g, flagimg);
                    setMenu(rend, g);
                    menu_button_width = (g->getWidth()*GRID_CELL_SIZE)/4;
                    set_size = SDL_FALSE;
                }else if(cursor.y > GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE+18+GRID_CELL_SIZE+18 && cursor.y < GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE+18+GRID_CELL_SIZE+18+GRID_CELL_SIZE) {
                    g = new Grid(16, 16, 40);
                    SDL_SetWindowSize(win, g->getWidth()*GRID_CELL_SIZE, (1+g->getHeight())*GRID_CELL_SIZE);
                    reloadGrid(rend, g, flagimg);
                    setMenu(rend, g);
                    menu_button_width = (g->getWidth()*GRID_CELL_SIZE)/4;
                    set_size = SDL_FALSE;
                }else if(cursor.y > GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE+18+GRID_CELL_SIZE+18+GRID_CELL_SIZE+18 && cursor.y < GRID_CELL_SIZE*(g->getHeight()/2)-2.5*GRID_CELL_SIZE+18+GRID_CELL_SIZE+18+GRID_CELL_SIZE+18+GRID_CELL_SIZE) {
                    g = new Grid(30, 24, 160);
                    cout << g->getWidth();
                    //g->printGrid();
                    SDL_SetWindowSize(win, g->getWidth()*GRID_CELL_SIZE, (1+g->getHeight())*GRID_CELL_SIZE);
                    reloadGrid(rend, g, flagimg);
                    setMenu(rend, g);
                    menu_button_width = (g->getWidth()*GRID_CELL_SIZE)/4;
                    set_size = SDL_FALSE;
                }
            }
        }

        if(!g->getFieldstoreveal()) {
            game_victory = SDL_TRUE;                
        }

        if(game_victory == SDL_TRUE) {
            game_end = {
                .x = GRID_CELL_SIZE*((g->getWidth()/2)-2),
                .y = GRID_CELL_SIZE*((g->getHeight()/2)-1),
                .w = GRID_CELL_SIZE*4,
                .h = GRID_CELL_SIZE*2,
            };
            if(g->getWidth()%2) game_end.x +=(GRID_CELL_SIZE/2);
            if(g->getHeight()%2) game_end.y +=(GRID_CELL_SIZE/2);
            end = Box(game_end, "End");
            end.name = "VICTORY";
        }
        if(game_defeat == SDL_TRUE) {
            if(g->getFieldstoreveal() != -1 && menu_clicked == SDL_FALSE) {
                gameOver(grid_cursor.x, grid_cursor.y, rend, g, grey_mine, red_mine, flagimg);
                g->setDefeat();
            }
            game_end = {
                .x = GRID_CELL_SIZE*((g->getWidth()/2)-2),
                .y = GRID_CELL_SIZE*((g->getHeight()/2)-1),
                .w = GRID_CELL_SIZE*4,
                .h = GRID_CELL_SIZE*2,
            };
            if(g->getWidth()%2) game_end.x +=(GRID_CELL_SIZE/2);
            if(g->getHeight()%2) game_end.y +=(GRID_CELL_SIZE/2);
            end = Box(game_end, "End");
            end.name = "GAME OVER";
        }
        if(menu_clicked == SDL_TRUE) {
            switch(menu_choice) {
                case 1:
                    setParams(rend, grid_cursor, g);
                    set_size = SDL_TRUE;
                    game_defeat = SDL_FALSE;
                    game_victory = SDL_FALSE;
                    break;
                case 2:
                    g = new Grid(g->getWidth(), g->getHeight(), g->getMines());
                    reloadGrid(rend, g, flagimg);
                    game_defeat = SDL_FALSE;
                    game_victory = SDL_FALSE;
                    break;
                case 3:
                    if(game_defeat == SDL_TRUE || game_victory == SDL_TRUE) {
                        Menu_Msg.name = "Save failed"; // - please start playing first
                        Menu_Msg.setAreaVar('w', GRID_CELL_SIZE*6);
                        Menu_Msg.setAreaVar('h', GRID_CELL_SIZE*2);
                        Menu_Msg.setAreaVar('x', GRID_CELL_SIZE*((g->getWidth()/2)-3));
                        Menu_Msg.setAreaVar('y', GRID_CELL_SIZE*((g->getHeight()/2)-4));
                        if(g->getWidth()%2) Menu_Msg.setAreaVar('x', GRID_CELL_SIZE*((g->getWidth()/2)-4)+GRID_CELL_SIZE/2);
                        if(g->getHeight()%2) Menu_Msg.setAreaVar('y', GRID_CELL_SIZE*((g->getHeight()/2)-4)+GRID_CELL_SIZE/2);

                        reload = SDL_TRUE;
                    }else if(g->getFirst()) {
                        Menu_Msg.name = "Save failed"; // - please start playing first
                        Menu_Msg.setAreaVar('w', GRID_CELL_SIZE*6);
                        Menu_Msg.setAreaVar('h', GRID_CELL_SIZE*2);
                        Menu_Msg.setAreaVar('x', GRID_CELL_SIZE*((g->getWidth()/2)-3));
                        Menu_Msg.setAreaVar('y', GRID_CELL_SIZE*((g->getHeight()/2)-2));
                        if(g->getWidth()%2) Menu_Msg.setAreaVar('x', GRID_CELL_SIZE*((g->getWidth()/2)-4)+GRID_CELL_SIZE/2);
                        if(g->getHeight()%2) Menu_Msg.setAreaVar('y', GRID_CELL_SIZE*((g->getHeight()/2)-4)+GRID_CELL_SIZE/2);
                        
                        Menu_Msg2 = Box(Menu_Msg);
                        Menu_Msg2.name = "start playing";
                        Menu_Msg2.setAreaVar('y', Menu_Msg.getAreaVar('y')+Menu_Msg.getAreaVar('h'));

                        reload = SDL_TRUE;
                    }else {
                        Menu_Msg.name = "Game saved";
                        Menu_Msg.setAreaVar('w', GRID_CELL_SIZE*4);
                        Menu_Msg.setAreaVar('h', GRID_CELL_SIZE*2);
                        Menu_Msg.setAreaVar('x', GRID_CELL_SIZE*((g->getWidth()/2)-2));
                        Menu_Msg.setAreaVar('y', GRID_CELL_SIZE*((g->getHeight()/2)-1));
                        if(g->getWidth()%2) Menu_Msg.setAreaVar('x', GRID_CELL_SIZE*((g->getWidth()/2)-4)+GRID_CELL_SIZE/2);
                        if(g->getHeight()%2) Menu_Msg.setAreaVar('y', GRID_CELL_SIZE*((g->getHeight()/2)-4)+GRID_CELL_SIZE/2);
                        g->saveGame();

                        Menu_Msg.drawBox(rend);
                        Menu_Msg2.setAreaVar('w', 0);
                        Menu_Msg2.setAreaVar('h', 0);
                        reload = SDL_TRUE;
                    }
                    break;
                case 4:
                    //cout << "load";
                    if(!access("save.bin", F_OK)) {
                        //if(yorN(rend, grid_cursor.x/GRID_CELL_SIZE, grid_cursor.y/GRID_CELL_SIZE, "Save file detected - do you want to continue your game?")) {
                            g = loadGame(rend);
                            SDL_SetWindowSize(win, g->getWidth()*GRID_CELL_SIZE, (1+g->getHeight())*GRID_CELL_SIZE);
                            setMenu(rend, g);
                            menu_button_width = (g->getWidth()*GRID_CELL_SIZE)/4;
                            reloadGrid(rend, g, flagimg);
                        //}
                    }else {
                        Menu_Msg.name = "No save file";
                        Menu_Msg.drawBox(rend);
                    }
                    game_defeat = SDL_FALSE;
                    game_victory = SDL_FALSE;
                    break;
            }
            menu_clicked = SDL_FALSE;
        }
        mouse_click = SDL_FALSE;
        SDL_RenderPresent(rend);
        if(quit == SDL_TRUE) {
            SDL_Quit();
        }
        if(game_victory == SDL_TRUE || game_defeat == SDL_TRUE) {
            end.drawBox(rend);
        }
        if(reload == SDL_TRUE) {
            Menu_Msg.drawBox(rend);
            if(game_victory != SDL_TRUE || game_defeat != SDL_TRUE) {
                Menu_Msg2.drawBox(rend);
            }
        }
    }

    //SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}