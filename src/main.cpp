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
#include "world/transform.hpp"
#include "world/model.hpp"
#include "graphics/context.hpp"

const unsigned int window_width = 1366;
const unsigned int window_height = 768;

void run(SDL_Window * window);
Model * generate_test_shape();

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
    Model * test_shape = generate_test_shape();

    mat_t<float> projection_mat = perspective(window_width / 2, window_height / 2, PI_f / 3.0f);

    // positions of cubes
    mat_t<float> position_1 = translation(vec_t<float>(1.0f, 2.0f, 35.0f));
    mat_t<float> position_2 = translation(vec_t<float>(-1.0f, 0.0f, 19.0f));

    // view matrix
    mat_t<float> view_mat = look_at(vec_t<float>(0,5,0), vec_t<float>(-1,0,19));

    // rotation of time
    mat_t<float> rot_matrix = rotation_y(0.0f);
    
    // variables for main loop
    float angle = 0.0f;
    bool running = true;

    while (running) {
        angle += 0.01f;

        // time dependant transforms
        position_1 = translation(vec_t<float>(1.0f, 2.0f, 35.0f - angle));
        rot_matrix = rotation_y(angle);
        
        SDL_Event event;

        context->clear();
        test_shape->render(*context, projection_mat, view_mat * position_1);
        test_shape->render(*context, projection_mat, view_mat * position_2 * rot_matrix);
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

    /*std::vector<float> colors = {
        255.0f, 0.0f, 0.0f,
        0.0f, 255.0f, 0.0f,
        0.0f, 0.0f, 255.0f,
        255.0f, 0.0f, 0.0f,
        0.0f, 255.0f, 0.0f,
        0.0f, 0.0f, 255.0f,
        255.0f, 0.0f, 0.0f,
        0.0f, 255.0f, 0.0f
    };*/

    std::vector<float> tex_coords = {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };

    return new Model(positions, indices, tex_coords, Texture("assets/texture.png"));
}