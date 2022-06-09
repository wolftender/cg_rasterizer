#include "math/matrix.hpp"
#include "math/vector.hpp"

mat_t<float> identity();
mat_t<float> translation(vec_t<float> v);
mat_t<float> rotation_x(float angle);
mat_t<float> rotation_y(float angle);
mat_t<float> rotation_z(float angle);
mat_t<float> scale(vec_t<float> s);
mat_t<float> perspective(unsigned int width, unsigned int height, float fov);
mat_t<float> look_at(vec_t<float> pos, vec_t<float> at);