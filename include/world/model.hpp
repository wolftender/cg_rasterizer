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

            triangle_t() : p1(0.0f), p2(0.0f), p3(0.0f) { }
        };

        Texture m_texture;
        std::vector<triangle_t> m_triangles;

    public:
        Model(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> tex_coords, const Texture & texture);
        void render(GraphicsContext& context, const mat_t<float>& projection, const mat_t<float>& world_view);

    private:
        void fill_scanlines(GraphicsContext& context, const triangle_t& triangle, const float area, const int min_x, const int max_x, const int min_y, const int max_y, const int step);
        void fill_triangle(GraphicsContext& context, triangle_t& triangle, const mat_t<float>& projection);

        // inline functions
        inline void sample_texture(float u, float v, color_t & out_color);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, const vec_t<float>& c);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, int cx, int cy);
};