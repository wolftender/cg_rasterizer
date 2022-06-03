#pragma once
#include <vector>
#include <array>
#include <SDL2/SDL.h>

#include "math/matrix.hpp"
#include "math/vector.hpp"

#include "graphics/context.hpp"

struct vertex_t {

};

class Model {
    private:
        struct triangle_t {
            vec_t<float> p1, p2, p3;
            color_t c1, c2, c3;
            triangle_t() : p1(0.0f), p2(0.0f), p3(0.0f) { } 
        };

        std::vector<triangle_t> m_triangles;

    public:
        Model(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> colors);
        void render(GraphicsContext& context, mat_t<float> transform);

    private:
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, const vec_t<float>& c);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, int cx, int cy);
};