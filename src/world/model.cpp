#include "world/model.hpp"

#include <thread>
#include <algorithm>

Model::Model(std::vector<float> pos, std::vector<unsigned int> ind, std::vector<float> tex, const Texture & texture) {
    const unsigned int num_positions = pos.size();
    const unsigned int num_indices = ind.size();

    if (num_positions % 3 != 0) {
        throw std::runtime_error("positions need to be divisible by 3");
    }

    if (num_indices % 3 != 0) {
        throw std::runtime_error("indices need to be divisible by 3");
    }

    const unsigned int num_edges = num_indices / 3;
    unsigned int base_index_1, base_index_2, base_index_3;
    unsigned int tex_index_1, tex_index_2, tex_index_3;

    m_triangles.resize(num_edges);
    m_texture = texture;

    for (unsigned int i = 0; i < num_edges; ++i) {
        base_index_1 = 3 * ind[3 * i + 0];
        base_index_2 = 3 * ind[3 * i + 1];
        base_index_3 = 3 * ind[3 * i + 2];

        m_triangles[i].p1.set(pos[base_index_1 + 0], pos[base_index_1 + 1], pos[base_index_1 + 2], 1.0f);
        m_triangles[i].p2.set(pos[base_index_2 + 0], pos[base_index_2 + 1], pos[base_index_2 + 2], 1.0f);
        m_triangles[i].p3.set(pos[base_index_3 + 0], pos[base_index_3 + 1], pos[base_index_3 + 2], 1.0f);

        //m_triangles[i].c1 = color_t(col[base_index_1 + 0], col[base_index_1 + 1], col[base_index_1 + 2]);
        //m_triangles[i].c2 = color_t(col[base_index_2 + 0], col[base_index_2 + 1], col[base_index_2 + 2]);
        //m_triangles[i].c3 = color_t(col[base_index_3 + 0], col[base_index_3 + 1], col[base_index_3 + 2]);

        tex_index_1 = (2 * ind[3 * i + 0]) % tex.size();
        tex_index_2 = (2 * ind[3 * i + 1]) % tex.size();
        tex_index_3 = (2 * ind[3 * i + 2]) % tex.size();

        m_triangles[i].t1 = tex_coords_t(tex[tex_index_1 + 0], tex[tex_index_1 + 1]);
        m_triangles[i].t2 = tex_coords_t(tex[tex_index_2 + 0], tex[tex_index_2 + 1]);
        m_triangles[i].t3 = tex_coords_t(tex[tex_index_3 + 0], tex[tex_index_3 + 1]);
    }
}

inline void Model::sample_texture(float u, float v, color_t & out_color) {
    const int tex_width = m_texture.get_width();
    const int tex_height = m_texture.get_height();
    const std::vector<uint8_t>& tex_buffer = m_texture.get_buffer();

    int x = ((int)(u * tex_width)) % tex_width;
    int y = ((int)(v * tex_height)) % tex_height;
    int index = 4 * (y * tex_width + x);

    out_color.r = tex_buffer[index + 0];
    out_color.g = tex_buffer[index + 1];
    out_color.b = tex_buffer[index + 2];
}

inline float Model::edge_function(const vec_t<float>& a, const vec_t<float>& b, const vec_t<float>& c) {
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

inline float Model::edge_function(const vec_t<float>& a, const vec_t<float>& b, int cx, int cy) {
    return (cx - a[0] + 0.5f) * (b[1] - a[1]) - (cy - a[1] + 0.5f) * (b[0] - a[0]);
}

void Model::render(GraphicsContext& context, const mat_t<float>& projection, const mat_t<float>& world_view) {
    const color_t color(255, 255, 255);

    for (const triangle_t& triangle : m_triangles) {
        triangle_t wv_triangle = triangle;

        wv_triangle.p1 = world_view * wv_triangle.p1;
        wv_triangle.p2 = world_view * wv_triangle.p2;
        wv_triangle.p3 = world_view * wv_triangle.p3;

        // here we can do clipping later on, for now just fill the triangle "blindly"
        fill_triangle(context, wv_triangle, projection);
    }
}

void Model::fill_triangle(GraphicsContext& context, triangle_t& triangle, const mat_t<float>& projection) {
    int width = context.get_width();
    int height = context.get_height();

    triangle.p1 = projection * triangle.p1;
    triangle.p2 = projection * triangle.p2;
    triangle.p3 = projection * triangle.p3;

    triangle.p1.perspective_divide();
    triangle.p2.perspective_divide();
    triangle.p3.perspective_divide();

    // area of the triangle, used in baricentric coordinates
    float area = edge_function(triangle.p1, triangle.p2, triangle.p3);

    int min_x = std::min({triangle.p1[0], triangle.p2[0], triangle.p3[0]});
    int max_x = std::max({triangle.p1[0], triangle.p2[0], triangle.p3[0]});
    int min_y = std::min({triangle.p1[1], triangle.p2[1], triangle.p3[1]});
    int max_y = std::max({triangle.p1[1], triangle.p2[1], triangle.p3[1]});

    min_y = std::max(0, min_y - 1);
    min_x = std::max(0, min_x - 1);
    max_y = std::min(height, max_y + 1);
    max_x = std::min(width, max_x + 1);

    #ifdef MULTITHREADING
        std::thread worker_1(&Model::fill_scanlines, this, std::ref(context), std::ref(triangle), area, min_x, max_x, min_y + 0, max_y, 4);
        std::thread worker_2(&Model::fill_scanlines, this, std::ref(context), std::ref(triangle), area, min_x, max_x, min_y + 1, max_y, 4);
        std::thread worker_3(&Model::fill_scanlines, this, std::ref(context), std::ref(triangle), area, min_x, max_x, min_y + 2, max_y, 4);
        std::thread worker_4(&Model::fill_scanlines, this, std::ref(context), std::ref(triangle), area, min_x, max_x, min_y + 3, max_y, 4);

        worker_1.join();
        worker_2.join();
        worker_3.join();
        worker_4.join();
    #else
        fill_scanlines(context, triangle, area, min_x, max_x, min_y, max_y, 1);
    #endif
}

void Model::fill_scanlines(GraphicsContext& context, const triangle_t& triangle, const float area, const int min_x, const int max_x, const int min_y, const int max_y, const int step) {
    for (int y = min_y; y < max_y; y = y + step) {
        for (int x = min_x; x < max_x; ++x) {
            color_t color;

            float w1 = edge_function(triangle.p2, triangle.p3, x, y);
            float w2 = edge_function(triangle.p3, triangle.p1, x, y);
            float w3 = edge_function(triangle.p1, triangle.p2, x, y);

            if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) {
                w1 = w1 / area;
                w2 = w2 / area;
                w3 = w3 / area;

                float depth = 1.0f / (w1 * triangle.p1[2] + w2 * triangle.p2[2] + w3 * triangle.p3[2]);
                
                if (context.set_depth(x, y, depth)) {
                    // perspective corrected interpolation
                    float u = depth * (
                        w1 * triangle.t1.u * triangle.p1[2] + 
                        w2 * triangle.t2.u * triangle.p2[2] + 
                        w3 * triangle.t3.u * triangle.p3[2]
                    );

                    float v = depth * (
                        w1 * triangle.t1.v * triangle.p1[2] + 
                        w2 * triangle.t2.v * triangle.p2[2] + 
                        w3 * triangle.t3.v * triangle.p3[2]
                    );

                    sample_texture(u, v, color);
                    context.set_pixel(x, y, color);
                }
            }
        }
    }
}
