#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <exception>
#include <vector>
#include <chrono>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "math/vector.hpp"
#include "math/transform.hpp"

#include "world/level.hpp"
#include "world/cube.hpp"
#include "world/maze.hpp"

const unsigned int window_width = 1366;
const unsigned int window_height = 768;

void run(SDL_Window * window);

int main(int argc, char ** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("failed to initialize sdl2");
    }

    if (IMG_Init(IMG_INIT_PNG) < 0) {
        throw std::runtime_error("failed to initialize image loading");
    }

    if (TTF_Init() < 0) {
        throw std::runtime_error("failed to initialize font engine");
    }

    // set vsync and better quality
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    unsigned int flags = SDL_WINDOW_SHOWN;
    SDL_Window * window = SDL_CreateWindow("computer graphics",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, flags);

    if (!window) {
        throw std::runtime_error("failed to create sdl2 window context: " + std::string(SDL_GetError()));
    }

    run(window);

    // cleanup
    SDL_DestroyWindow(window);

    // cleanup sdl
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}

void run(SDL_Window * window) {
    GraphicsContext * context = new GraphicsContext(window, window_width / 2, window_height / 2);
    Level * level = new Maze(15, 15);

    Cube& cube = level->add_entity<Cube>();
    cube.set_position(vec_t<float>(0.0f, 0.0f, 10.0f));

    auto last_time = std::chrono::steady_clock::now();
    mat_t<float> projection_mat = perspective(window_width / 2, window_height / 2, PI_f / 3.0f);

    bool running = true;

    while (running) {        
        SDL_Event event;

        auto now = std::chrono::steady_clock::now();
        float elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_time).count() / 1000.0f;

        last_time = now;

        level->update(elapsed);

        context->clear();
        level->render(*context, projection_mat);
        context->present();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    delete level;
    delete context;
}