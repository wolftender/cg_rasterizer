#pragma once
#include "world/entity.hpp"
#include "graphics/model.hpp"

#include <SDL2/SDL.h>

class Sphere : public Entity {
	private:
		Model m_model;

	private:
		Model generate_sphere_model(const std::string& texture);

	public:
		Sphere();
		Sphere(const std::string& texture);

		void event(const SDL_Event& event) override;
		void update(Level & level, float delta_time) override;
		void render(GraphicsContext & context, const mat_t<float> & projection, const mat_t<float> & view) override;
};