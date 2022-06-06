#include "graphics/texture.hpp"

#include <stdexcept>
#include <exception>

unsigned int Texture::get_width() const {
	return m_width;
}

unsigned int Texture::get_height() const {
	return m_height;
}

const std::vector<uint8_t> & Texture::get_buffer() const {
	return m_buffer;
}

std::vector<uint8_t> & Texture::get_buffer() {
	return m_buffer;
}

Texture::Texture() {
	m_buffer = { 255, 255, 255, 255 };
	m_width = m_height = 1;
}

Texture::Texture(const std::string & filename) {
	SDL_Surface * loaded_surface = IMG_Load(filename.c_str());

	if (!loaded_surface) {
		throw std::runtime_error("failed to load texture: " + filename + ", error: " + IMG_GetError());
	}

	// convert the surface to raw bytes
	// we want it to be in rgba format because it is easier to do things with it later on
	SDL_Surface * new_surface = SDL_ConvertSurfaceFormat(loaded_surface, SDL_PIXELFORMAT_RGBA8888, 0);

	if (new_surface == NULL) {
		SDL_FreeSurface(loaded_surface);
		throw std::runtime_error("failed to convert texture to internal format: " + std::string(SDL_GetError()));
	}

	SDL_FreeSurface(loaded_surface);
	loaded_surface = new_surface;

	SDL_LockSurface(loaded_surface);

	uint8_t r, g, b, a;
	
	m_width = loaded_surface->w;
	m_height = loaded_surface->h;

	m_buffer.resize(m_width * m_height * 4);

	for (int i = 0; i < loaded_surface->w * loaded_surface->h; ++i) {
		SDL_GetRGBA(((uint32_t*)loaded_surface->pixels)[i], loaded_surface->format, &r, &g, &b, &a);
		
		m_buffer[4 * i + 0] = r;
		m_buffer[4 * i + 1] = g;
		m_buffer[4 * i + 2] = b;
		m_buffer[4 * i + 3] = a;
	}

	SDL_UnlockSurface(loaded_surface);
	SDL_FreeSurface(loaded_surface);
}
