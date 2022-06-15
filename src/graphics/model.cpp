#include "graphics/model.hpp"

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

        m_triangles[i].v1.pos.set(pos[base_index_1 + 0], pos[base_index_1 + 1], pos[base_index_1 + 2], 1.0f);
        m_triangles[i].v2.pos.set(pos[base_index_2 + 0], pos[base_index_2 + 1], pos[base_index_2 + 2], 1.0f);
        m_triangles[i].v3.pos.set(pos[base_index_3 + 0], pos[base_index_3 + 1], pos[base_index_3 + 2], 1.0f);

        //m_triangles[i].c1 = color_t(col[base_index_1 + 0], col[base_index_1 + 1], col[base_index_1 + 2]);
        //m_triangles[i].c2 = color_t(col[base_index_2 + 0], col[base_index_2 + 1], col[base_index_2 + 2]);
        //m_triangles[i].c3 = color_t(col[base_index_3 + 0], col[base_index_3 + 1], col[base_index_3 + 2]);

        tex_index_1 = (2 * (3 * i + 0)) % tex.size();
        tex_index_2 = (2 * (3 * i + 1)) % tex.size();
        tex_index_3 = (2 * (3 * i + 2)) % tex.size();

        m_triangles[i].v1.u = tex[tex_index_1 + 0];
        m_triangles[i].v1.v = tex[tex_index_1 + 1];

        m_triangles[i].v2.u = tex[tex_index_2 + 0];
        m_triangles[i].v2.v = tex[tex_index_2 + 1];

        m_triangles[i].v3.u = tex[tex_index_3 + 0];
        m_triangles[i].v3.v = tex[tex_index_3 + 1];
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

inline float Model::signed_distance(const vec_t<float>& normal, float d, const vec_t<float>& point) {
    return normal.dot(point) - d;
}

inline float Model::intersect(const vec_t<float>& v1, const vec_t<float>& v2, const vec_t<float>& normal, float d) {
    return (d - normal.dot(v1)) / normal.dot(v2 - v1);
}

inline void Model::clip_triangle(const vec_t<float>& normal, float d, vertex_t& v1, vertex_t& v2, vertex_t& v3) {
    float t2 = intersect(v1.pos, v2.pos, normal, d);
    float t3 = intersect(v1.pos, v3.pos, normal, d);

    // interpolate coordinates for texture
    v2.u = t2 * v2.u + (1 - t2) * v1.u;
    v2.v = t2 * v2.v + (1 - t2) * v1.v;

    v3.u = t3 * v3.u + (1 - t3) * v1.u;
    v3.v = t3 * v3.v + (1 - t3) * v1.v;

    v2.pos = v1.pos + t2 * (v2.pos - v1.pos);
    v3.pos = v1.pos + t3 * (v3.pos - v1.pos);
}

inline void Model::clip_triangle(const vec_t<float>& normal, float d, vertex_t& v1, vertex_t& v2, vertex_t& v3, triangle_t& out_t1, triangle_t& out_t2) {
    float t1 = intersect(v1.pos, v3.pos, normal, d);
    float t2 = intersect(v2.pos, v3.pos, normal, d);

    //vec_t<float> v2_p = v2.pos + t2 * (v3.pos - v2.pos);

    vertex_t v1_p(
        v1.pos + t1 * (v3.pos - v1.pos), 
        t1 * v3.u + (1 - t1) * v1.u, 
        t1 * v3.v + (1 - t1) * v1.v
    );

    vertex_t v2_p(
        v2.pos + t2 * (v3.pos - v2.pos), 
        t2 * v3.u + (1 - t2) * v2.u, 
        t2 * v3.v + (1 - t2) * v2.v
    );

    out_t1.v1 = v1;
    out_t1.v2 = v2_p;
    out_t1.v3 = v1_p;

    out_t2.v1 = v1;
    out_t2.v2 = v2;
    out_t2.v3 = v2_p;
}


void Model::render(GraphicsContext& context, const mat_t<float>& projection, const mat_t<float>& world_view) {
    // clipping plane (const)
    static const vec_t<float> clip_normal(0.0f, 0.0f, 1.0f);
    static const float clip_d = 1.0f;

    const color_t color(255, 255, 255);

    triangle_t t1, t2;
    for (const triangle_t& triangle : m_triangles) {
        triangle_t wv_triangle = triangle;

        wv_triangle.v1.pos = world_view * wv_triangle.v1.pos;
        wv_triangle.v2.pos = world_view * wv_triangle.v2.pos;
        wv_triangle.v3.pos = world_view * wv_triangle.v3.pos;

        float d1 = signed_distance(clip_normal, clip_d, wv_triangle.v1.pos);
        float d2 = signed_distance(clip_normal, clip_d, wv_triangle.v2.pos);
        float d3 = signed_distance(clip_normal, clip_d, wv_triangle.v3.pos);

        bool wireframe = context.is_wireframe();
        
        const int condition = ((d3 > 0) << 2) | ((d2 > 0) << 1) | (d1 > 0);
        switch (condition) {
            case 0b000: // (VIII)
                break;

            case 0b011: // (II)
                clip_triangle(clip_normal, clip_d, wv_triangle.v1, wv_triangle.v2, wv_triangle.v3, t1, t2);
                fill_triangle(context, t1, projection, wireframe);
                fill_triangle(context, t2, projection, wireframe);
                break;

            case 0b101: // (III)
                clip_triangle(clip_normal, clip_d, wv_triangle.v3, wv_triangle.v1, wv_triangle.v2, t1, t2);
                fill_triangle(context, t1, projection, wireframe);
                fill_triangle(context, t2, projection, wireframe);
                break;

            case 0b110: // (I)
                clip_triangle(clip_normal, clip_d, wv_triangle.v2, wv_triangle.v3, wv_triangle.v1, t1, t2);
                fill_triangle(context, t1, projection, wireframe);
                fill_triangle(context, t2, projection, wireframe);
                break;

            case 0b001: // (VI)
                clip_triangle(clip_normal, clip_d, wv_triangle.v1, wv_triangle.v2, wv_triangle.v3);
                fill_triangle(context, wv_triangle, projection, wireframe);
                break;

            case 0b010: // (V)
                clip_triangle(clip_normal, clip_d, wv_triangle.v2, wv_triangle.v3, wv_triangle.v1);
                fill_triangle(context, wv_triangle, projection, wireframe);
                break;

            case 0b100: // (IV)
                clip_triangle(clip_normal, clip_d, wv_triangle.v3, wv_triangle.v1, wv_triangle.v2);
                fill_triangle(context, wv_triangle, projection, wireframe);
                break;

            case 0b111: // 111 (VII)
                fill_triangle(context, wv_triangle, projection, wireframe);
                break;
        }
    }
}

void Model::fill_triangle(GraphicsContext& context, triangle_t& triangle, const mat_t<float>& projection, bool wireframe) {  
    int width = context.get_width();
    int height = context.get_height();

    triangle.v1.pos = projection * triangle.v1.pos;
    triangle.v2.pos = projection * triangle.v2.pos;
    triangle.v3.pos = projection * triangle.v3.pos;

    triangle.v1.pos.perspective_divide();
    triangle.v2.pos.perspective_divide();
    triangle.v3.pos.perspective_divide();

    // area of the triangle, used in baricentric coordinates
    float area = edge_function(triangle.v1.pos, triangle.v2.pos, triangle.v3.pos);
    if (area < 0) { // backface culling
        return;
    }

    int min_x = std::min({triangle.v1.pos[0], triangle.v2.pos[0], triangle.v3.pos[0]});
    int max_x = std::max({triangle.v1.pos[0], triangle.v2.pos[0], triangle.v3.pos[0]});
    int min_y = std::min({triangle.v1.pos[1], triangle.v2.pos[1], triangle.v3.pos[1]});
    int max_y = std::max({triangle.v1.pos[1], triangle.v2.pos[1], triangle.v3.pos[1]});

    min_y = std::max(0, min_y - 1);
    min_x = std::max(0, min_x - 1);
    max_y = std::min(height, max_y + 1);
    max_x = std::min(width, max_x + 1);

    if (!wireframe) {
        fill_scanlines(context, triangle, area, min_x, max_x, min_y, max_y, 1);
    } else {
        context.draw_line(triangle.v1.pos[0], triangle.v1.pos[1], triangle.v2.pos[0], triangle.v2.pos[1], color_t(0, 255, 0));
        context.draw_line(triangle.v2.pos[0], triangle.v2.pos[1], triangle.v3.pos[0], triangle.v3.pos[1], color_t(0, 255, 0));
        context.draw_line(triangle.v3.pos[0], triangle.v3.pos[1], triangle.v1.pos[0], triangle.v1.pos[1], color_t(0, 255, 0));
    }
}

void Model::fill_scanlines(GraphicsContext& context, const triangle_t& triangle, const float area, const int min_x, const int max_x, const int min_y, const int max_y, const int step) {
    for (int y = min_y; y < max_y; y = y + step) {
        for (int x = min_x; x < max_x; ++x) {
            color_t color;

            float w1 = edge_function(triangle.v2.pos, triangle.v3.pos, x, y);
            float w2 = edge_function(triangle.v3.pos, triangle.v1.pos, x, y);
            float w3 = edge_function(triangle.v1.pos, triangle.v2.pos, x, y);

            if (w1 >= 0.0f && w2 >= 0.0f && w3 >= 0.0f) {
                w1 = w1 / area;
                w2 = w2 / area;
                w3 = w3 / area;

                float depth = 1.0f / (w1 * triangle.v1.pos[2] + w2 * triangle.v2.pos[2] + w3 * triangle.v3.pos[2]);
                
                if (context.set_depth(x, y, depth)) {
                    // perspective corrected interpolation
                    float u = depth * (
                        w1 * triangle.v1.u * triangle.v1.pos[2] + 
                        w2 * triangle.v2.u * triangle.v2.pos[2] + 
                        w3 * triangle.v3.u * triangle.v3.pos[2]
                    );

                    float v = depth * (
                        w1 * triangle.v1.v * triangle.v1.pos[2] + 
                        w2 * triangle.v2.v * triangle.v2.pos[2] + 
                        w3 * triangle.v3.v * triangle.v3.pos[2]
                    );

                    sample_texture(u, v, color);
                    context.set_pixel(x, y, color);
                }
            }
        }
    }
}
