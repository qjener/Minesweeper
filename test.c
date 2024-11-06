/*#include <stdio.h>
#include <SDL.h>

int main(int argc, char** argv) {
    //printf("%d", (8*sizeof(void*)));
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, "tit", "test", NULL);
return 0;
}*/

#include <stdio.h>

int main() {
	printf("xdtjigubypdsxur");
	return 0;
}

/*
#include "funcs.h"

int main(int argc, char** argv) {
	SDL_Window* win = SDL_CreateWindow("Minesweeper", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    500, 500, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	SDL_Texture* t;
	t = IMG_LoadTexture(rend, "red_mine.png");
	int w = 500;
	SDL_RenderCopy(rend, t, NULL, NULL);
	SDL_RenderPresent(rend);

	while(1) {
       SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                break;
                case SDL_QUIT:
                exit(1);
            }
        }

        //SDL_SetRenderDrawColor( rend, 255, 255, 255, 255 );
	    //SDL_RenderClear( rend );

        //SDL_RenderCopy(rend, texture, NULL, NULL);

        //surface = TTF_RenderText_Solid(font, s.c_str(), {255,255,255,255}); //SDL_CreateRGBSurface(0, area.w, area.h, 32, 0, 0, 0, 0);
        
        //text = SDL_CreateTextureFromSurface(rend, surface);

        //area.x = 320 - (surface->w / 2.0f);
		//area.y = 240;
		//area.w = surface->w;
		//area.h = surface->h;
		//SDL_RenderCopy(rend, text, NULL, &area);

		SDL_DestroyTexture(t);
		//SDL_FreeSurface(surface);

        SDL_RenderPresent( rend );
    }
}
*/

/*
int main(int argc, char** argv) {
    Grid *g = getGrid();
    SDL_Window* win = SDL_CreateWindow("Minesweeper", // creates a window
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    g->getWidth()*GRID_CELL_SIZE, (1+g->getWidth())*GRID_CELL_SIZE, 0);
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

    string s = "hallo";
    TTF_Font* font;
    font = TTF_OpenFont("Times New Roman.ttf", 24);


    SDL_Surface* surface = TTF_RenderText_Solid(font, s.c_str(), {255,255,255,255});
    SDL_Texture* text = SDL_CreateTextureFromSurface(rend, surface);
    int text_width = surface->w;
    int text_height = surface->h;
    SDL_FreeSurface(surface);
    SDL_Rect area = {
            .x = 20,
            .y = g->getWidth()*GRID_CELL_SIZE,
            .w = text_width,
            .h = text_height,
        };
    SDL_RenderCopy(rend, text, NULL, &area);
    SDL_DestroyTexture(text);



    
    SDL_Color color = {255, 255, 255, 255};
    SDL_Texture* texture, *text;

        SDL_Surface* buffer = SDL_GetWindowSurface(win);
        texture = SDL_CreateTextureFromSurface(rend, buffer);
        SDL_FreeSurface( buffer );
	    buffer = NULL;
    //Button button = Button(rend, win);
    while(1) {
       SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEBUTTONDOWN:
                break;
                case SDL_QUIT:
                exit(1);
            }
        }

        SDL_SetRenderDrawColor( rend, 255, 255, 255, 255 );
	    SDL_RenderClear( rend );

        SDL_RenderCopy(rend, texture, NULL, NULL);

        surface = TTF_RenderText_Solid(font, s.c_str(), {255,255,255,255}); //SDL_CreateRGBSurface(0, area.w, area.h, 32, 0, 0, 0, 0);
        
        text = SDL_CreateTextureFromSurface(rend, surface);

        area.x = 320 - (surface->w / 2.0f);
		area.y = 240;
		area.w = surface->w;
		area.h = surface->h;
		SDL_RenderCopy(rend, text, NULL, &area);

		SDL_DestroyTexture(text);
		SDL_FreeSurface(surface);

        SDL_RenderPresent( rend );
    }
        
    return 0;
}*/


/*
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

bool init();
void kill();
bool loop();

// Pointers to our window, renderer, texture, and font
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture *texture, *text;
TTF_Font* font;
string input;

int main(int argc, char** args) {

	if ( !init() ) {
		system("pause");
		return 1;
	}

	while ( loop() ) {
		// wait before processing the next frame
		SDL_Delay(10); 
	}

	kill();
	return 0;
}

bool loop() {

	static const unsigned char* keys = SDL_GetKeyboardState( NULL );

	SDL_Event e;
    
	SDL_Rect dest;

	// Clear the window to white
	SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
	SDL_RenderClear( renderer );
    input = "hallo";
	// Event loop
	while ( SDL_PollEvent( &e ) != 0 ) {
		switch (e.type) {
			case SDL_QUIT:
				return false;
			case SDL_TEXTINPUT:
				input += e.text.text;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_BACKSPACE && input.size()) {
					input.pop_back();
				}
				break;
		}
	}

	// Render texture
	SDL_RenderCopy(renderer, texture, NULL, NULL);

	SDL_Color foreground = { 0, 0, 0 };

	if ( input.size() ) {
		SDL_Surface* text_surf = TTF_RenderText_Solid(font, input.c_str(), foreground);
		text = SDL_CreateTextureFromSurface(renderer, text_surf);

		dest.x = 320 - (text_surf->w / 2.0f);
		dest.y = 240;
		dest.w = text_surf->w;
		dest.h = text_surf->h;
		SDL_RenderCopy(renderer, text, NULL, &dest);

		SDL_DestroyTexture(text);
		SDL_FreeSurface(text_surf);
	}

	// Update window
	SDL_RenderPresent( renderer );

	return true;
}

bool init() {
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		cout << "Error initializing SDL: " << SDL_GetError() << endl;
		return false;
	} 

	if ( IMG_Init(IMG_INIT_PNG) < 0 ) {
		cout << "Error initializing SDL_image: " << IMG_GetError() << endl;
		return false;
	}

	// Initialize SDL_ttf
	if ( TTF_Init() < 0 ) {
		cout << "Error intializing SDL_ttf: " << TTF_GetError() << endl;
		return false;
	}

	window = SDL_CreateWindow( "Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN );
	if ( !window ) {
		cout << "Error creating window: " << SDL_GetError()  << endl;
		return false;
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer ) {
		cout << "Error creating renderer: " << SDL_GetError() << endl;
		return false;
	}

	SDL_Surface* buffer = IMG_Load("test.png");
	if ( !buffer ) {
		cout << "Error loading image test.png: " << SDL_GetError() << endl;
		return false;
	}

	texture = SDL_CreateTextureFromSurface( renderer, buffer );
	SDL_FreeSurface( buffer );
	buffer = NULL;
	if ( !texture ) {
		cout << "Error creating texture: " << SDL_GetError() << endl;
		return false;
	}

	// Load font
	font = TTF_OpenFont("Times New Roman.ttf", 72);
	if ( !font ) {
		cout << "Error loading font: " << TTF_GetError() << endl;
		return false;
	}

	// Start sending SDL_TextInput events
	SDL_StartTextInput();

	return true;
}

void kill() {
	SDL_StopTextInput();

	TTF_CloseFont( font );
	SDL_DestroyTexture( texture );
	texture = NULL;

	SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}*/


/*

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT (WINDOW_WIDTH)

void get_text_and_rect(SDL_Renderer *renderer, int x, int y, char *text,
        TTF_Font *font, SDL_Texture **texture, SDL_Rect *rect) {
    int text_width;
    int text_height;
    SDL_Surface *surface;
    SDL_Color textColor = {255, 255, 255, 0};

    surface = TTF_RenderText_Solid(font, text, textColor);
    *texture = SDL_CreateTextureFromSurface(renderer, surface);
    text_width = surface->w;
    text_height = surface->h;
    SDL_FreeSurface(surface);
    rect->x = x;
    rect->y = y;
    rect->w = text_width;
    rect->h = text_height;
}

int main(int argc, char **argv) {
    SDL_Event event;
    SDL_Rect rect1, rect2;
    SDL_Renderer *renderer;
    SDL_Texture *texture1, *texture2;
    SDL_Window *window;
    char *font_path;
    int quit;

    // Inint TTF.
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_WIDTH, 0, &window, &renderer);
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("Times New Roman.ttf", 24);

    get_text_and_rect(renderer, 0, 0, "hello", font, &texture1, &rect1);
    //get_text_and_rect(renderer, 0, rect1.y + rect1.h, "world", font, &texture2, &rect2);

    quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event) == 1) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        //SDL_RenderClear(renderer);

        // Use TTF textures. 
        SDL_RenderCopy(renderer, texture1, NULL, &rect1);
        SDL_RenderCopy(renderer, texture2, NULL, &rect2);

        SDL_RenderPresent(renderer);
    }

    // Deinit TTF.
    SDL_DestroyTexture(texture1);
    SDL_DestroyTexture(texture2);
    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}*/