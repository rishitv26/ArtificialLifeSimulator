#include <SDL.h>
#include <iostream>
using std::cout;
using std::endl;
#include "LifeSnapshot.h"

constexpr int SCALE = 2;
constexpr int WIDTH = 500/2;
constexpr int HEIGHT = 500/2;
constexpr int R = 255;
constexpr int G = 255;
constexpr int B = 255;
constexpr double SPEED_CONSTANTS = 0.0001;
constexpr double TIMELAPSE_INCREMENT = 0.001;
double TIMELAPSE_INITIAL = 0.1;

int main(int argc, char* args[]) {
    // Basic Setup: for initialization of the Window.
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event e;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH*SCALE, HEIGHT*SCALE, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, SCALE, SCALE);

    LifeSnapshot snapshot({100, 100}, 10, WIDTH, HEIGHT, R, G, B);
    snapshot.setInteractionMatrix({ {0.1, -0.1}, {0.1, -0.1} });

    int x = 0;
    int y = 0;
    int x_new = 0;
    int y_new = 0;

    // Main loop:
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&e)) { // get all events (capture them here)
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_x:
                    if (x == 0 && y == 0) {
                        SDL_GetMouseState(&x, &y);
                    }
                    else {
                        SDL_GetMouseState(&x_new, &y_new);

                        const int dx = x_new - x;
                        const int dy = y_new - y;
                        double r = dx * dx + dy * dy;
                        r = sqrt(r);
                        const double angle = atan2(dy, dx);

                        Particle part(x / (double)SCALE, y / (double)SCALE, R, G, B);
                        double force = r * SPEED_CONSTANTS;

                        part.vX = cos(angle) * force;
                        part.vY = sin(angle) * force;
                        snapshot.addParticle(&part);

                        x = 0;
                        y = 0;
                        y_new = 0;
                        x_new = 0;
                    }
                    break;
                case SDLK_UP:
                    cout << "INCREMENTING SPEED\n";
                    TIMELAPSE_INITIAL += TIMELAPSE_INCREMENT;
                    snapshot.setInteractionMatrix({ {TIMELAPSE_INITIAL} });
                    break;
                case SDLK_DOWN:
                    cout << "DECREMENTING SPEED\n";
                    TIMELAPSE_INITIAL -= TIMELAPSE_INCREMENT;
                    snapshot.setInteractionMatrix({ {TIMELAPSE_INITIAL} });
                    break;
                }
            }
        }

        snapshot.update();
        snapshot.draw(renderer);
    }

    return 0;
}