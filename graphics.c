#include "graphics.h"

#include <stdio.h>
#include <stdbool.h>

#ifndef _WIN32
#include <SDL2/SDL.h> /* Windows SDL2 import */
#else 
#include <SDL2/SDL.h> /* MacOS/Linux SDL2 import */
#endif

#define WIDTH 64
#define HEIGHT 32
#define PX_SIZE 8


SDL_Window* window;

SDL_Renderer* renderer;

/*
* SDL Keyboard Mapping
* 
* 1 2 3 4
* q w e r
* a s d f
* z x c v
*/
SDL_Scancode keymappings[16] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V,
};

bool quit = false;

int init_display() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "CHIP-8 Emulator",
        SDL_WINDOWPOS_UNDEFINED, 
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH*PX_SIZE,
        HEIGHT*PX_SIZE,
        0
    );

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    return 0;
}

void draw(unsigned char* display) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < HEIGHT; y++) {
        for(int x = 0; x < WIDTH; x++) {
            if (display[x + (y*WIDTH)]) {
                SDL_Rect rect;

                rect.x = x*PX_SIZE;
                rect.y = y*PX_SIZE;
                rect.h = PX_SIZE;
                rect.w = PX_SIZE;

                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void sdl_ehandler(unsigned char *keypad) {
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            default:
                if (state[SDL_SCANCODE_ESCAPE]) {
                    quit = true;
                }

                for (int keycode = 0; keycode < 16; keycode++) {
                    keypad[keycode] = state[keymappings[keycode]];
                }

                break;
        }
    }
}

bool should_quit() { return quit; }

void stop_display() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
