#pragma once
#include <vector>
#include <array>
#include <SDL2/SDL.h>

#include "math/matrix.hpp"
#include "math/vector.hpp"

#include "graphics/context.hpp"
#include "graphics/texture.hpp"

class Model {
    private:
        struct tex_coords_t {
            float u, v;

            tex_coords_t() : u(0.0), v(0.0) { }
            tex_coords_t(float u, float v) : u(u), v(v) { }
        };

        struct triangle_t {
            vec_t<float> p1, p2, p3;
            tex_coords_t t1, t2, t3;
            // color_t c1, c2, c3;

            triangle_t() : p1(0.0f), p2(0.0f), p3(0.0f) { } 
        };

        Texture m_texture;
        std::vector<triangle_t> m_triangles;

    public:
        Model(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> tex_coords, const Texture & texture);
        void render(GraphicsContext& context, mat_t<float> transform);

    private:
        inline void sample_texture(float u, float v, color_t & out_color);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, const vec_t<float>& c);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, int cx, int cy);
};