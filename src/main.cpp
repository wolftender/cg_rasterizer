#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <exception>
#include <vector>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "math/vector.hpp"
#include "math/transform.hpp"

#include "world/level.hpp"

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
    Level * level = new Level();

    mat_t<float> projection_mat = perspective(window_width / 2, window_height / 2, PI_f / 3.0f);

    bool running = true;

    while (running) {        
        SDL_Event event;

        context->clear();
        
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

/*Model * generate_test_shape() {
    std::vector<float> positions = {
        -1, -1, -1,
        1, -1, -1,
        1, 1, -1,
        -1, 1, -1,
        -1, -1, 1,
        1, -1, 1,
        1, 1, 1,
        -1, 1, 1
    };

    std::vector<unsigned int> indices = { 
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };

    std::vector<float> tex_coords = {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };

    return new Model(positions, indices, tex_coords, Texture("assets/cobblestone.png"));
}*/