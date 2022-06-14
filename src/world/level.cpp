#include "world/level.hpp"
#include "math/transform.hpp"

const vec_t<float> & Level::get_camera_eye() {
	return m_camera_eye;
}

const vec_t<float> & Level::get_camera_at() {
	return m_camera_at;
}

void Level::set_camera_eye(const vec_t<float> & pos) {
	m_recalculate_matrix = true;
	m_camera_eye = pos;
}

void Level::set_camera_at(const vec_t<float> & pos) {
	m_recalculate_matrix = true;
	m_camera_at = pos;
}

Level::Level() : 
	m_camera_at(0.0f, 0.0f, 1.0f), 
	m_camera_eye(0.0f), 
	m_view_matrix(identity()) {

	m_last_id = 0;

	for (std::size_t i = 0; i < max_entities; ++i) {
		m_entities[i] = nullptr;
	}
}

Level::~Level() {}

void Level::event(const SDL_Event& event) {
	for (int i = 0; i < max_entities; ++i) {
		if (m_entities[i]) {
			m_entities[i]->event(event);
		}
	}
}

void Level::render(GraphicsContext & context, const mat_t<float>& projection) {
	mat_t<float> view_matrix = get_view_matrix();

	for (int i = 0; i < max_entities; ++i) {
		if (m_entities[i]) {
			m_entities[i]->render(context, projection, view_matrix);
		}
	}
}

void Level::update(float delta_time) {
	for (int i = 0; i < max_entities; ++i) {
		if (m_entities[i]) {
			m_entities[i]->update(*this, delta_time);
		}
	}
}

bool Level::can_move(const vec_t<float>& position) {
	return true;
}

vec_t<float> Level::get_start_pos() {
	return vec_t<float>(0.0f);
}

const mat_t<float> & Level::get_view_matrix() {
	if (m_recalculate_matrix) {
		m_view_matrix = look_at(m_camera_eye, m_camera_at);
		m_recalculate_matrix = false;
	}

	return m_view_matrix;
}
