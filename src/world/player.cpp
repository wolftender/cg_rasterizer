#include "world/player.hpp"
#include "math/transform.hpp"
#include "world/level.hpp"

#include <iostream>

Player::Player() : Cube("assets/blue_wool.png") {}

void Player::event(const SDL_Event& event) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        bool value = (event.type == SDL_KEYDOWN);

        switch (event.key.keysym.sym) {
            case SDL_KeyCode::SDLK_w: m_move_forward = value; break;
            case SDL_KeyCode::SDLK_a: m_move_left = value; break;
            case SDL_KeyCode::SDLK_d: m_move_right = value; break;
            case SDL_KeyCode::SDLK_s: m_move_reverse = value; break;
        }
    }
}

void Player::update(Level & level, float delta_time) {
    float rot_y = get_rotation()[1];
    mat_t<float> transform = rotation_y(rot_y);

    if (m_move_left || m_move_right) {
        float angular_velocity = (m_move_right) ? 1.0f : -1.0f;

        vec_t<float> rotation = get_rotation();
        rotation[1] = rotation[1] + delta_time * angular_velocity;

        set_rotation(rotation);
    }

    if (m_move_forward || m_move_reverse) {
        float velocity = (m_move_forward) ? 8.0f : -8.0f;

        vec_t<float> direction(1.0f, 0.0f, 0.0f);
        vec_t<float> current_pos = get_position();
        vec_t<float> next_pos = current_pos + delta_time * velocity * (transform * direction);

        if (level.can_move(next_pos)) {
            set_position(next_pos);
        }
    }

    // update camera
    vec_t<float> cam_direction(-1.0f, 0.0f, 0.0f);
    cam_direction = transform * cam_direction;

    level.set_camera_at(get_position());
    level.set_camera_eye(get_position() + cam_direction);
}

void Player::render(GraphicsContext& context, const mat_t<float> & projection, const mat_t<float> & view) {
    //Cube::render(context, projection, view);
}