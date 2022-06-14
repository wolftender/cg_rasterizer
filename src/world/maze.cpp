#include "world/maze.hpp"
#include "math/transform.hpp"

#include <stdexcept>
#include <stack>
#include <random>

Maze::Maze(int width, int height) {
	m_width = width;
	m_height = height;

	if (m_width < 5 || m_height < 5) {
		throw std::runtime_error("map size too small, minimum 4x4");
	}

	generate_maze();
	generate_mesh();
}

Maze::Maze(int width, int height, std::vector<unsigned int> map) {
	m_width = width;
	m_height = height;

	if (m_width * m_height != map.size()) {
		throw std::runtime_error("invalid map data");
	}

	m_map_buffer.resize(map.size());
	for (int i = 0; i < map.size(); ++i) {
		m_map_buffer[i] = (map[i]) ? wall : empty;
	}

	generate_mesh();
}

void Maze::event(const SDL_Event& event) {
	Level::event(event);
}

void Maze::render(GraphicsContext & context, const mat_t<float> & projection) {
	Level::render(context, projection);

	m_model->render(context, projection, get_view_matrix());
}

void Maze::update(float delta_time) {
	Level::update(delta_time);
}

inline bool Maze::valid_neighbor(const map_square_pos_t & pos) {
	if (pos.x < 0 || pos.x >= m_width || pos.y < 0 || pos.y >= m_height) return false;
	if (m_map_buffer[get_index(pos)] != wall) return false;

	return true;
}

inline unsigned int Maze::get_index(const map_square_pos_t & pos) {
	return (pos.y * m_width) + pos.x;
}

bool Maze::can_move(const vec_t<float>& position) {
	float x = position[0];
	float y = position[2];

	int map_x = x / tile_width;
	int map_y = y / tile_height;

	if (map_x < 0 || map_x > m_height || map_y < 0 || map_y >= m_height) return true;
	return (m_map_buffer[map_y * m_width + map_x] != wall);
}

vec_t<float> Maze::get_start_pos() {
	return vec_t<float>(1.5f * tile_width, 0.0f, 1.5f * tile_width);
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

void Maze::generate_mesh() {
	std::vector<float> mesh_positions;
	std::vector<float> mesh_tex_coords;
	std::vector<unsigned int> mesh_indices;

	const float tw = tile_width; // tile width
	const float th = tile_width; // tile height
	const float wh = tile_height; // wall height

	for (int i = 0, j = 0; i < m_map_buffer.size(); ++i) {
		int square_x = i % m_width;
		int square_y = i / m_width;

		float tx = square_x * tw;
		float ty = square_y * th;

		if (m_map_buffer[i] == empty) {
			std::vector<float> positions = {
				tx, 	 0.0f, ty + th,
				tx + tw, 0.0f, ty + th,
				tx, 	 0.0f, ty,
				tx + tw, 0.0f, ty,

				// top vertices
				tx, 	 wh,   ty + th,
				tx + tw, wh,   ty + th,
				tx, 	 wh,   ty,
				tx + tw, wh,   ty
			};

			std::vector<float> tex_coords = {
				1.0f, 0.0f,
				0.0f, 1.0f,
				1.0f, 1.0f,
				1.0f, 0.0f,
				0.0f, 0.0f,
				0.0f, 1.0f
			};

			std::vector<unsigned int> indices;

			if (m_map_buffer[i + m_width] == wall) { // top wall
				indices.insert(indices.end(), { 5, 0, 1, 5, 4, 0 });
				mesh_tex_coords.insert(mesh_tex_coords.end(), tex_coords.begin(), tex_coords.end());
			}

			if (m_map_buffer[i - 1] == wall) { // left wall
				indices.insert(indices.end(), { 4, 2, 0, 4, 6, 2 });
				mesh_tex_coords.insert(mesh_tex_coords.end(), tex_coords.begin(), tex_coords.end());
			}

			if (m_map_buffer[i - m_width] == wall) { // bottom wall
				indices.insert(indices.end(), { 6, 3, 2, 6, 7, 3 });
				mesh_tex_coords.insert(mesh_tex_coords.end(), tex_coords.begin(), tex_coords.end());
			}

			if (m_map_buffer[i + 1] == wall) { // right wall
				indices.insert(indices.end(), { 7, 1, 3, 7, 5, 1 });
				mesh_tex_coords.insert(mesh_tex_coords.end(), tex_coords.begin(), tex_coords.end());
			}

			for (int k = 0; k < indices.size(); ++k) {
				indices[k] = (j * 8) + indices[k];
			}

			mesh_positions.insert(mesh_positions.end(), positions.begin(), positions.end());
			mesh_indices.insert(mesh_indices.end(), indices.begin(), indices.end());

			j = j + 1;
		}
	}

	m_model = std::make_unique<Model>(mesh_positions, mesh_indices, mesh_tex_coords, Texture("assets/bricks.png"));
}

std::ostream & operator<<(std::ostream & stream, const Maze & maze) {
	for (unsigned int i = 0; i < maze.m_map_buffer.size(); ++i) {
		stream << (maze.m_map_buffer[i] == maze.wall ? '#' : ' ');

		if ((i + 1) % maze.m_width == 0) {
			stream << std::endl;
		}
	}

	return stream;
}
