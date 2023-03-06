#include <stdio.h>
#include <stdbool.h>

#ifndef _WIN32
#include <SDL2/SDL.h> /* Windows SDL2 import */
#else 
#include <SDL2/SDL.h> /* MacOS/Linux SDL2 import */
#endif

/* Set constants */
#define WIDTH   640
#define HEIGHT  320

int main(int argc, char** argv) {
    SDL_Window *window = NULL;
    SDL_Surface *windowSurface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL failed to initialise: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
            "CHIP-8 Emulator",
            SDL_WINDOWPOS_UNDEFINED, 
            SDL_WINDOWPOS_UNDEFINED,
            WIDTH,
            HEIGHT,
            0
        );

    if (window == NULL) {
        fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
        return 1;
    } else {
        windowSurface = SDL_GetWindowSurface(window);

        SDL_FillRect(windowSurface, NULL, SDL_MapRGB(windowSurface->format, 0xFF, 0xFF, 0xFF));

        SDL_UpdateWindowSurface(window);

        SDL_Event e;
        bool quit = false;

        // Main loop
        while (quit == false) {
            while (SDL_PollEvent(&e)) { 
                if (e.type == SDL_QUIT) quit = true; 
            }
        }

    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
