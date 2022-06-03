#include "world/model.hpp"
#include <algorithm>

Model::Model(std::vector<float> pos, std::vector<unsigned int> ind, std::vector<float> col) {
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

    m_triangles.resize(num_edges);

    for (unsigned int i = 0; i < num_edges; ++i) {
        base_index_1 = 3 * ind[3 * i + 0];
        base_index_2 = 3 * ind[3 * i + 1];
        base_index_3 = 3 * ind[3 * i + 2];

        m_triangles[i].p1.set(pos[base_index_1 + 0], pos[base_index_1 + 1], pos[base_index_1 + 2], 1.0f);
        m_triangles[i].p2.set(pos[base_index_2 + 0], pos[base_index_2 + 1], pos[base_index_2 + 2], 1.0f);
        m_triangles[i].p3.set(pos[base_index_3 + 0], pos[base_index_3 + 1], pos[base_index_3 + 2], 1.0f);

        m_triangles[i].c1 = color_t(col[base_index_1 + 0], col[base_index_1 + 1], col[base_index_1 + 2]);
        m_triangles[i].c2 = color_t(col[base_index_2 + 0], col[base_index_2 + 1], col[base_index_2 + 2]);
        m_triangles[i].c3 = color_t(col[base_index_3 + 0], col[base_index_3 + 1], col[base_index_3 + 2]);
    }
}

inline float Model::edge_function(const vec_t<float>& a, const vec_t<float>& b, const vec_t<float>& c) {
    return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

inline float Model::edge_function(const vec_t<float>& a, const vec_t<float>& b, int cx, int cy) {
    return (cx - a[0] + 0.5f) * (b[1] - a[1]) - (cy - a[1] + 0.5f) * (b[0] - a[0]);
}

#include <iostream>
void Model::render(GraphicsContext& context, mat_t<float> transform) {
    const color_t color(255, 255, 255);
    vec_t<float> p1(0.0f), p2(0.0f), p3(0.0f);

    int width = context.get_width();
    int height = context.get_height();

    for (auto triangle : m_triangles) {
        p1 = transform * triangle.p1;
        p2 = transform * triangle.p2;
        p3 = transform * triangle.p3;

        p1.perspective_divide();
        p2.perspective_divide();
        p3.perspective_divide();

        float area = edge_function(p1, p2, p3);
        
        //context.draw_line(p1[0], p1[1], p2[0], p2[1], color);
        //context.draw_line(p2[0], p2[1], p3[0], p3[1], color);
        //context.draw_line(p3[0], p3[1], p1[0], p1[1], color);

        int min_x = std::min({p1[0], p2[0], p3[0]});
        int max_x = std::max({p1[0], p2[0], p3[0]});
        int min_y = std::min({p1[1], p2[1], p3[1]});
        int max_y = std::max({p1[1], p2[1], p3[1]});

        min_y = std::max(0, min_y - 1);
        min_x = std::max(0, min_x - 1);
        max_y = std::min(height, max_y + 1);
        max_x = std::min(width, max_x + 1);

        vec_t<float> p(0.0f);
        color_t color;

        for (int y = min_y; y < max_y; ++y) {
            for (int x = min_x; x < max_x; ++x) {
                float w1 = edge_function(p2, p3, x, y);
                float w2 = edge_function(p3, p1, x, y);
                float w3 = edge_function(p1, p2, x, y);

                if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) {
                    w1 = w1 / area;
                    w2 = w2 / area;
                    w3 = w3 / area;

                    float depth = (w1 / p1[2] + w2 / p2[2] + w3 / p3[2]);
                    
                    if (context.set_depth(x, y, depth)) {
                        color.r = w1 * triangle.c1.r + w2 * triangle.c2.r + w3 * triangle.c3.r;
                        color.g = w1 * triangle.c1.g + w2 * triangle.c2.g + w3 * triangle.c3.g;
                        color.b = w1 * triangle.c1.b + w2 * triangle.c2.b + w3 * triangle.c3.b;

                        context.set_pixel(x, y, color);
                    }
                }
            }
        }
    }
}