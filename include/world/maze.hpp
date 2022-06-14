#pragma once
#include "world/level.hpp"
#include "graphics/model.hpp"

#include <vector>
#include <memory>

class Maze : public Level {
	private:
		static constexpr float tile_width = 6.0f;
		static constexpr float tile_height = 6.0f;

		enum map_square_t {
			wall = 0,
			empty = 1
		};

		struct map_square_pos_t {
			int x, y;
			map_square_pos_t(int x, int y) : x(x), y(y) { }
		};

		std::vector<map_square_t> m_map_buffer;
		int m_width;
		int m_height;

		std::unique_ptr<Model> m_model;

	public:
		Maze(int width, int height);
		Maze(int width, int height, std::vector<unsigned int> map);

		void event(const SDL_Event& event) override;
		void render(GraphicsContext & context, const mat_t<float> & projection) override;
		void update(float delta_time) override;

		bool can_move(const vec_t<float>& position) override;
		vec_t<float> get_start_pos() override;

	private:
		inline bool valid_neighbor(const map_square_pos_t& pos);
		inline unsigned int get_index(const map_square_pos_t& pos);
		void generate_maze();
		void generate_mesh();

	friend std::ostream & operator<<(std::ostream & stream, const Maze & maze);
};

std::ostream & operator<<(std::ostream & stream, const Maze & maze);