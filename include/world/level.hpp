#pragma once

#include "math/vector.hpp"
#include "math/matrix.hpp"
#include "graphics/context.hpp"
#include "world/entity.hpp"

#include <vector>
#include <array>
#include <memory>

#include <SDL2/SDL.h>

class Level {
	private:
		static constexpr std::size_t max_entities = 200;

	private:
		std::array<std::unique_ptr<Entity>, max_entities> m_entities;
		std::size_t m_last_id;

		vec_t<float> m_camera_eye;
		vec_t<float> m_camera_at;
		mat_t<float> m_view_matrix;

		bool m_recalculate_matrix = true;

	public:
		const vec_t<float>& get_camera_eye();
		const vec_t<float>& get_camera_at();

		void set_camera_eye(const vec_t<float>& pos);
		void set_camera_at(const vec_t<float>& pos);

	public:
		Level();
		virtual ~Level();

		// delete copy
		Level(const Level&) = delete;
		Level& operator=(const Level&) = delete;

		virtual void event(const SDL_Event& event);
		virtual void render(GraphicsContext & context, const mat_t<float> & projection);
		virtual void update(float delta_time);

		virtual bool can_move(const vec_t<float>& position);
		virtual vec_t<float> get_start_pos();

		const mat_t<float> & get_view_matrix();

		template<class T, class... Args> T & add_entity(Args&&... args) {
			static_assert(std::is_base_of<Entity, T>{}, "T has to be a derived type of Entity");

			std::size_t entity_id = 0;
			while (m_entities[entity_id]) entity_id++;

			if (entity_id > max_entities) {
				throw std::runtime_error("too many entities");
			}

			m_entities[entity_id] = std::make_unique<T>(args...);
			return *(reinterpret_cast<T*>(m_entities[entity_id].get()));
		}
};