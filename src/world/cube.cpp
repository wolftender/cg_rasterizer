#include "world/cube.hpp"

Model Cube::generate_cube_model(std::string texture) {
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

    std::vector<float> tex_coords = {
        0, 0,
        1, 0,
        1, 1,
        0, 1
    };

    return Model(positions, indices, tex_coords, Texture(texture));
}

Cube::Cube() : m_model(generate_cube_model("assets/cobblestone.png")) { }

void Cube::update(Level & level, float delta_time) { }

void Cube::render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) {
    mat_t<float> world_view = view * get_world();
    m_model.render(context, projection, get_world());
}
