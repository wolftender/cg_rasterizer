#pragma once
#include "world/entity.hpp"
#include "graphics/model.hpp"

#include <SDL2/SDL.h>

class Cube : public Entity {
	private:
		Model m_model;

	private:
		Model generate_cube_model(std::string texture);

	public:
		Cube();

		void event(const SDL_Event& event) override;
		void update(Level & level, float delta_time) override;
		void render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) override;
};