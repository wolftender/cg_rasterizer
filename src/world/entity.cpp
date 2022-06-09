#include "world/entity.hpp"
#include "math/transform.hpp"

const vec_t<float> & Entity::get_position() {
    return m_position;
}

const vec_t<float> & Entity::get_rotation() {
    return m_rotation;
}

const vec_t<float> & Entity::get_scale() {
    return m_scale;
}

const mat_t<float> & Entity::get_world() {
    if (m_recalculate_matrix) {
        m_recalculate_matrix = false;
        m_world = translation(m_position) * rotation_z(m_rotation[2]) * rotation_y(m_rotation[1]) * rotation_x(m_rotation[0]) * scale(m_scale);
    }

    return m_world;
}

void Entity::set_position(const vec_t<float> & position) {
    m_position = position;
}

void Entity::set_rotation(const vec_t<float> & rotation) {
    m_rotation = rotation;
}

void Entity::set_scale(const vec_t<float> & scale) {
    m_scale = scale;
}