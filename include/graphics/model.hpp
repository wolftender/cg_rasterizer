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
        struct vertex_t {
            vec_t<float> pos;
            float u, v;

            vertex_t() : pos(0.0f), u(0.0f), v(0.0f) { }
            vertex_t(const vec_t<float>& pos, float u, float v) : pos(pos), u(u), v(v) { }
            vertex_t(float x, float y, float z, float u, float v) : pos(x, y, z), u(u), v(v) { }
        };

        struct triangle_t {
            vertex_t v1, v2, v3;
            triangle_t() { }
            triangle_t(const vertex_t& v1, const vertex_t& v2, const vertex_t& v3) : v1(v1), v2(v2), v3(v3) { }
        };

        Texture m_texture;
        std::vector<triangle_t> m_triangles;

    public:
        Model(std::vector<float> positions, std::vector<unsigned int> indices, std::vector<float> tex_coords, const Texture & texture);
        void render(GraphicsContext& context, const mat_t<float>& projection, const mat_t<float>& world_view);

    private:
        void fill_scanlines(GraphicsContext& context, const triangle_t& triangle, const float area, const int min_x, const int max_x, const int min_y, const int max_y, const int step);
        void fill_triangle(GraphicsContext& context, triangle_t& triangle, const mat_t<float>& projection, bool wireframe = false);

        // inline functions
        inline float signed_distance(const vec_t<float>& normal, float d, const vec_t<float>& point);
        inline float intersect(const vec_t<float>& v1, const vec_t<float>& v2, const vec_t<float>& normal, float d);

        inline void clip_triangle(const vec_t<float>& normal, float d, vertex_t& v1, vertex_t& v2, vertex_t& v3);
        inline void clip_triangle(const vec_t<float>& normal, float d, vertex_t& v1, vertex_t& v2, vertex_t& v3, triangle_t& out_t1, triangle_t& out_t2);
        
        inline void sample_texture(float u, float v, color_t & out_color);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, const vec_t<float>& c);
        inline float edge_function(const vec_t<float>& a, const vec_t<float>& b, int cx, int cy);
};