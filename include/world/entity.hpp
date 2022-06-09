#pragma once
#include "math/vector.hpp"
#include "math/matrix.hpp"

#include "graphics/context.hpp"

class Level;
class Entity {
	private:
		vec_t<float> m_position;
		vec_t<float> m_rotation;
		vec_t<float> m_scale;

		mat_t<float> m_world;
		bool m_recalculate_matrix = true;

	public:
		const vec_t<float> & get_position();
		const vec_t<float> & get_rotation();
		const vec_t<float> & get_scale();
		const mat_t<float> & get_world();

		void set_position(const vec_t<float> & position);
		void set_rotation(const vec_t<float> & rotation);
		void set_scale(const vec_t<float> & scale);

		virtual void update(Level & level, float delta_time) = 0;
		virtual void render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) = 0;
};