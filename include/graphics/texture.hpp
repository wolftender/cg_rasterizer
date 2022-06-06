#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <vector>

class Texture {
	private:
		std::vector<uint8_t> m_buffer;
		unsigned int m_width, m_height;

	public:
		unsigned int get_width() const;
		unsigned int get_height() const;

		const std::vector<uint8_t>& get_buffer() const;
		std::vector<uint8_t>& get_buffer();

		Texture();
		Texture(const std::string & filename);
};