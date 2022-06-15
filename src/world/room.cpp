#include "world/room.hpp"

Room::Room() : Maze(9, 8, std::vector<unsigned int> {
        1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1
    }),
    m_sphere1(add_entity<Sphere>()),
    m_sphere2(add_entity<Sphere>("assets/blue_wool.png")) {
    
    vec_t<float> sphere_pos1 = get_tile_pos(3, 5);
    vec_t<float> sphere_pos2 = get_tile_pos(5, 5);

    sphere_pos1[1] = 3.0f;
    sphere_pos2[1] = 3.0f;

    m_sphere1.set_position(sphere_pos1);
    m_sphere2.set_position(sphere_pos2);
}

void Room::update(float delta_time) {
    vec_t<float> rotation_s1 = m_sphere1.get_rotation();
    rotation_s1[1] = rotation_s1[1] + delta_time * M_PIf * 0.4f;

    m_sphere1.set_rotation(rotation_s1);

    Maze::update(delta_time);
}