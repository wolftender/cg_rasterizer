#include "world/transform.hpp"
#include <cmath>

mat_t<float> identity() {
    return mat_t<float> (
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat_t<float> translation(vec_t<float> v) {
    return mat_t<float> (
        1.0f, 0.0f, 0.0f, v[0],
        0.0f, 1.0f, 0.0f, v[1],
        0.0f, 0.0f, 1.0f, v[2],
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat_t<float> rotation_x(float theta) {
    const float s = sin(theta);
    const float c = cos(theta);

    return mat_t<float> (
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, c,    -s,   0.0f,
        0.0f, s,    c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat_t<float> rotation_y(float theta) {
    const float s = sin(theta);
    const float c = cos(theta);

    return mat_t<float> (
        c,    0.0f, s,    0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        -s,   0.0f, c,    0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat_t<float> rotation_z(float theta) {
    const float s = sin(theta);
    const float c = cos(theta);

    return mat_t<float> (
        c,    -s,   0.0f, 0.0f,
        s,    c,    0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

mat_t<float> perspective(unsigned int width, unsigned int height, float fov) {
    const float d = width / (2.0f * tan(fov / 2.0f));
    const float cx = width / 2.0f;
    const float cy = height / 2.0f;

    return mat_t<float> (
        -d,    0.0f, cx,   0.0f,
        0.0f, d,    cy,   0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, 0.0f
    );
}

mat_t<float> look_at(vec_t<float> pos, vec_t<float> at) {
    const vec_t<float> cam_up(0.0f, 1.0f, 0.0f);
    const vec_t<float> dir = pos - at;

    vec_t<float> forward = dir;
    forward.normalize();

    vec_t<float> right = cross(cam_up, forward);
    right.normalize();

    vec_t<float> up = cross(forward, right);
    up.normalize();

    return mat_t<float>(
        right[0], right[1], right[2], 0.0f,
        up[0], up[1], up[2], 0.0f,
        forward[0], forward[1], forward[2], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ) * translation(vec_t<float>(-pos[0], -pos[1], -pos[2]));
}