#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <vector>
#include <chrono>

struct color_t {
    uint8_t r, g, b, a;
    color_t() : r(0), g(0), b(0), a(SDL_ALPHA_OPAQUE) { }
    color_t(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a) { }
    color_t(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b), a(SDL_ALPHA_OPAQUE) { }
};

class GraphicsContext {
    private:
        SDL_Window * m_window;
        SDL_Renderer * m_renderer;
        SDL_Texture * m_texture;
        TTF_Font * m_font;

        unsigned int m_width;
        unsigned int m_height;

        uint8_t * m_buffer = nullptr;
        float * m_depthBuffer = nullptr;
        
        std::chrono::steady_clock::time_point m_last_frame;
        unsigned int m_frames, m_frameTimer, m_fpsAvg;

        bool m_wireframe;

    public:
        unsigned int get_width();
        unsigned int get_height();

        void set_wireframe(bool value);
        bool is_wireframe();

    public:
        GraphicsContext(SDL_Window * window, unsigned int resX, unsigned int resY);
        ~GraphicsContext();

        GraphicsContext(const GraphicsContext&) = delete;
        GraphicsContext& operator=(const GraphicsContext&) = delete;

        void clear();
        void present();

    public: // drawing functions
        bool set_depth(unsigned int x, unsigned int y, float depth);
        void set_pixel(unsigned int x, unsigned int y, const color_t& color);
        void set_pixel_s(unsigned int x, unsigned int y, const color_t& color);
        void draw_line(int x0, int y0, int x1, int y1, const color_t& color);

    private:
        void setup_texture();
        void render_text(int x, int y, const char * text);
};