#pragma once
#include "world/entity.hpp"

class Cube : public Entity {
	public:
		Cube();

		void update(Level & level, float delta_time) override;
		void render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) override;
};