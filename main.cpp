#include <SDL.h>
#include <iostream>
using std::cout;
using std::endl;
#include "LifeSnapshot.h"

constexpr int SCALE = 2;
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;
constexpr int LOOP_DELAY = 1;

int main(int argc, char* args[]) {
    // Basic Setup: for initialization of the Window.
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH*SCALE, HEIGHT*SCALE, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, SCALE, SCALE);

    LifeSnapshot snapshot(1000, 10, WIDTH, HEIGHT, 255, 255, 255);
    snapshot.setInteractionMatrix({ {0.001} });

    // Main loop:
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e)) { // get all events (capture them here)
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        snapshot.update();
        snapshot.draw(renderer);

        SDL_Delay(LOOP_DELAY);
    }

    return 0;
}