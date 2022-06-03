#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <exception>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "math/vector.hpp"
#include "world/transform.hpp"
#include "world/model.hpp"
#include "graphics/context.hpp"

const unsigned int window_width = 1366;
const unsigned int window_height = 768;

void run(SDL_Window * window);
void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2);
Model * generate_test_shape();

int main(int argc, char ** argv) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("failed to initialize sdl2");
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
    SDL_Quit();

    return 0;
}

void run(SDL_Window * window) {
    std::cout << "a" << std::endl;
    GraphicsContext * context = new GraphicsContext(window, window_width / 2, window_height / 2);
    std::cout << "b" << std::endl;
    Model * test_shape = generate_test_shape();

    mat_t<float> view_matrix = perspective(window_width / 2, window_height / 2, M_PIf / 6.0f);
    mat_t<float> position = translation(vec_t<float>(1.0f, 2.0f, 35.0f));
    mat_t<float> position2 = translation(vec_t<float>(-1.0f, 0.0f, 19.0f));
    mat_t<float> cam_matrix = look_at(vec_t<float>(10,0,0), vec_t<float>(0,0,27));
    std::cout << cam_matrix << std::endl;
    
    float angle = 0.0f;

    bool running = true;
    while (running) {
        angle += 0.02f;
        cam_matrix = identity();
        
        SDL_Event event;

        context->clear();
        test_shape->render(*context, view_matrix * cam_matrix * position);
        test_shape->render(*context, view_matrix * cam_matrix * position2);
        context->present();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
        }
    }

    delete context;
    delete test_shape;
}

Model * generate_test_shape() {
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

    std::vector<float> colors = {
        255.0f, 0.0f, 0.0f,
        0.0f, 255.0f, 0.0f,
        0.0f, 0.0f, 255.0f,
        255.0f, 0.0f, 0.0f,
        0.0f, 255.0f, 0.0f,
        0.0f, 0.0f, 255.0f,
        255.0f, 0.0f, 0.0f,
        0.0f, 255.0f, 0.0f
    };

    return new Model(positions, indices, colors);
}