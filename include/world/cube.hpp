#pragma once
#include "world/entity.hpp"
#include "graphics/model.hpp"

class Cube : public Entity {
	private:
		Model m_model;

	public:
		Cube();

		void update(Level & level, float delta_time) override;
		void render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) override;
};