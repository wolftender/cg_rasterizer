#include "world/sphere.hpp"
#include "math/vector.hpp"

Model Sphere::generate_sphere_model(const std::string& texture) {
    const int m = 8; // meridians
    const int n = 8; // parallels
    const float radius = 1.0f;

    std::vector<float> positions;
    std::vector<unsigned int> indices;
    std::vector<float> tex_coords_v;
    std::vector<float> tex_coords;

    positions.resize(n * m * 3 + 6); //nm+2 verts
    tex_coords_v.resize(n * m * 2 + 4);
    indices.resize(n * m * 6); // 2nm triangles
    tex_coords.resize(n * m * 12);

    positions[0 + 0] = 0.0f;
    positions[0 + 1] = radius;
    positions[0 + 2] = 0.0f;

    positions[3 * (n * m + 1) + 0] = 0.0f;
    positions[3 * (n * m + 1) + 1] = -radius;
    positions[3 * (n * m + 1) + 2] = 0.0f;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int index = (i * m) + j + 1;

            float alpha = 2.0f*M_PIf*j/(float)m;
            float beta = (float)(M_PIf/(n + 1)) * (float)(i + 1);

            positions[3 * index + 0] = radius * cos(alpha) * sin(beta); 
            positions[3 * index + 1] = radius * cos(beta); 
            positions[3 * index + 2] = radius * sin(alpha) * sin(beta); 
        }
    }

    tex_coords_v[0] = 1.0f;
    tex_coords_v[1] = 0.5f;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            int index = i * m + j + 1;

            tex_coords_v[2 * index + 0] = (float)j/(m - 1);
            tex_coords_v[2 * index + 1] = (float)(i + 1)/(n + 1);
        }
    }

    tex_coords_v[2*m*n+2] = 0.0f;
    tex_coords_v[2*m*n+3] = 0.5f;

    // lids
    for (int i = 0; i < m; ++i) {
        int index1 = 3 * i;
        int index2 = ((2 * n - 1) * m + i) * 3;

        indices[index1 + 2] = 0;
        indices[index2 + 2] = m * n + 1;

        if (i == m - 1) {
            indices[index1 + 1] = 1;
            indices[index1 + 0] = m;

            indices[index2 + 1] = n * m;
            indices[index2 + 0] = (n - 1) * m + 1;
        } else {
            indices[index1 + 1] = (i + 2);
            indices[index1 + 0] = (i + 1);

            indices[index2 + 1] = (n - 1) * m + i + 1;
            indices[index2 + 0] = (n - 1) * m + i + 2;
        }

        tex_coords[2 * index1 + 0] = tex_coords_v[2 * indices[index1 + 0] + 0];
        tex_coords[2 * index1 + 1] = tex_coords_v[2 * indices[index1 + 0] + 1];
        tex_coords[2 * index1 + 2] = tex_coords_v[2 * indices[index1 + 1] + 0];
        tex_coords[2 * index1 + 3] = tex_coords_v[2 * indices[index1 + 1] + 1];
        tex_coords[2 * index1 + 4] = tex_coords_v[2 * indices[index1 + 2] + 0];
        tex_coords[2 * index1 + 5] = tex_coords_v[2 * indices[index1 + 2] + 1];

        tex_coords[2 * index2 + 0] = tex_coords_v[2 * indices[index2 + 0] + 0];
        tex_coords[2 * index2 + 1] = tex_coords_v[2 * indices[index2 + 0] + 1];
        tex_coords[2 * index2 + 2] = tex_coords_v[2 * indices[index2 + 1] + 0];
        tex_coords[2 * index2 + 3] = tex_coords_v[2 * indices[index2 + 1] + 1];
        tex_coords[2 * index2 + 4] = tex_coords_v[2 * indices[index2 + 2] + 0];
        tex_coords[2 * index2 + 5] = tex_coords_v[2 * indices[index2 + 2] + 1];
    }

    // strips
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 1; j <= m; ++j) {
            int index1 = 3 * ((((2 * i) + 1) * m) + j - 1);
            int index2 = 3 * ((((2 * i) + 2) * m) + j - 1);

            if (j != m) {
                indices[index1 + 2] = (i * m) + j;
                indices[index1 + 1] = (i * m) + j + 1;
                indices[index1 + 0] = ((i + 1) * m) + j + 1;

                indices[index2 + 2] = (i * m) + j;
                indices[index2 + 1] = ((i + 1) * m) + j + 1;
                indices[index2 + 0] = ((i + 1) * m) + j;
            } else {
                indices[index1 + 2] = (i + 1) * m;
                indices[index1 + 1] = (i * m) + 1;
                indices[index1 + 0] = ((i + 1) * m) + 1;

                indices[index2 + 2] = (i + 1) * m;
                indices[index2 + 1] = ((i + 1) * m) + 1;
                indices[index2 + 0] = (i + 2) * m;
            }

            tex_coords[2 * index1 + 0] = tex_coords_v[2 * indices[index1 + 0] + 0];
            tex_coords[2 * index1 + 1] = tex_coords_v[2 * indices[index1 + 0] + 1];
            tex_coords[2 * index1 + 2] = tex_coords_v[2 * indices[index1 + 1] + 0];
            tex_coords[2 * index1 + 3] = tex_coords_v[2 * indices[index1 + 1] + 1];
            tex_coords[2 * index1 + 4] = tex_coords_v[2 * indices[index1 + 2] + 0];
            tex_coords[2 * index1 + 5] = tex_coords_v[2 * indices[index1 + 2] + 1];

            tex_coords[2 * index2 + 0] = tex_coords_v[2 * indices[index2 + 0] + 0];
            tex_coords[2 * index2 + 1] = tex_coords_v[2 * indices[index2 + 0] + 1];
            tex_coords[2 * index2 + 2] = tex_coords_v[2 * indices[index2 + 1] + 0];
            tex_coords[2 * index2 + 3] = tex_coords_v[2 * indices[index2 + 1] + 1];
            tex_coords[2 * index2 + 4] = tex_coords_v[2 * indices[index2 + 2] + 0];
            tex_coords[2 * index2 + 5] = tex_coords_v[2 * indices[index2 + 2] + 1];
        }
    }

    return Model(positions, indices, tex_coords, Texture(texture));
}

Sphere::Sphere() : m_model(generate_sphere_model("assets/texture.png")) { }
Sphere::Sphere(const std::string& texture) : m_model(generate_sphere_model(texture)) { }

void Sphere::event(const SDL_Event & event) { }
void Sphere::update(Level & level, float delta_time) { }

void Sphere::render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) {
    mat_t<float> world_view = view * get_world();
    m_model.render(context, projection, world_view);
}
