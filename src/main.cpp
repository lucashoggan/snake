#include <SDL3/SDL.h>
#include <iostream>

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Snake", 800, 600, SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}