#include "world/maze.hpp"

#include <stdexcept>
#include <stack>
#include <random>

Maze::Maze(unsigned int width, unsigned int height) {
	m_width = width;
	m_height = height;

	if (m_width < 5 || m_height < 5) {
		throw std::runtime_error("map size too small, minimum 4x4");
	}

	generate_maze();
}

void Maze::render(GraphicsContext & context, const mat_t<float> & projection) {
}

void Maze::update(float delta_time) {
}

inline bool Maze::valid_neighbor(const map_square_pos_t & pos) {
	if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height) return false;
	if (m_map_buffer[get_index(pos)] != wall) return false;

	return true;
}

inline unsigned int Maze::get_index(const map_square_pos_t & pos) {
	return (pos.y * m_width) + pos.x;
}

void Maze::generate_maze() {
	// c++ random gen
	std::random_device dev;
	std::mt19937 rng(dev());

	std::stack<map_square_pos_t> stack; 
	map_square_pos_t curr = map_square_pos_t(1, m_height - 2);

	std::vector<map_square_pos_t> neighbors;
	neighbors.reserve(4);

	m_map_buffer.resize(m_width * m_height);
	stack.push(curr);

	while (!stack.empty()) {
		curr = stack.top();

		m_map_buffer[get_index(curr)] = empty;

		map_square_pos_t top = map_square_pos_t(curr.x, curr.y - 2);
		map_square_pos_t left = map_square_pos_t(curr.x - 2, curr.y);
		map_square_pos_t right = map_square_pos_t(curr.x + 2, curr.y);
		map_square_pos_t bottom = map_square_pos_t(curr.x, curr.y + 2);

		if (valid_neighbor(top)) neighbors.push_back(top);
		if (valid_neighbor(left)) neighbors.push_back(left);
		if (valid_neighbor(right)) neighbors.push_back(right);
		if (valid_neighbor(bottom)) neighbors.push_back(bottom);

		if (neighbors.size() > 0) {
			std::uniform_int_distribution<> dist(0, neighbors.size() - 1);
			int index = dist(rng);

			map_square_pos_t next = neighbors[index];
			neighbors.clear();

			map_square_pos_t wall_idx = map_square_pos_t((curr.x + next.x) / 2, (curr.y + next.y) / 2);
			m_map_buffer[get_index(wall_idx)] = empty;

			stack.push(next);
		} else {
			stack.pop();
		}
	}
}

std::ostream & operator<<(std::ostream & stream, const Maze & maze) {
	for (int i = 0; i < maze.m_map_buffer.size(); ++i) {
		stream << (maze.m_map_buffer[i] == maze.wall ? '#' : ' ');

		if ((i + 1) % maze.m_width == 0) {
			stream << std::endl;
		}
	}

	return stream;
}
