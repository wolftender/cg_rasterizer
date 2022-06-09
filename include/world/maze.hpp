#pragma once
#include "world/level.hpp"
#include <vector>

class Maze : public Level {
	private:
		enum map_square_t {
			wall = 0,
			empty = 1
		};

		struct map_square_pos_t {
			int x, y;
			map_square_pos_t(int x, int y) : x(x), y(y) { }
		};

		std::vector<map_square_t> m_map_buffer;
		unsigned int m_width;
		unsigned int m_height;

	public:
		Maze(unsigned int width, unsigned int height);

		void render(GraphicsContext & context, const mat_t<float> & projection) override;
		void update(float delta_time) override;

	private:
		inline bool valid_neighbor(const map_square_pos_t& pos);
		inline unsigned int get_index(const map_square_pos_t& pos);
		void generate_maze();

	friend std::ostream & operator<<(std::ostream & stream, const Maze & maze);
};

std::ostream & operator<<(std::ostream & stream, const Maze & maze);