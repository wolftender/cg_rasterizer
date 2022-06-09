#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>

#include "graphics/context.hpp"

unsigned int GraphicsContext::get_width() {
    return m_width;
}
        
unsigned int GraphicsContext::get_height() {
    return m_height;
}

GraphicsContext::GraphicsContext(SDL_Window * window, unsigned int resX, unsigned int resY) {
    m_window = window;

    m_width = resX;
    m_height = resY;

    m_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    m_texture = nullptr;

    setup_texture();

    m_last_frame = std::chrono::steady_clock::now();
    m_frameTimer = 0;
    m_frames = 0;
    m_fpsAvg = 0;

    m_font = TTF_OpenFont("assets/font.ttf", 32);
    if (!m_font) {
        throw std::runtime_error("failed to initialize font");
    }

    if (!m_renderer) {
        throw std::runtime_error("failed to initialize renderer: " + std::string(SDL_GetError()));
    }
}

GraphicsContext::~GraphicsContext() {
    if (m_font) {
        TTF_CloseFont(m_font);
    }

    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
    }

    delete m_buffer;
    delete m_depthBuffer;
}

void GraphicsContext::clear() {
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(m_renderer);

    std::fill(m_buffer, m_buffer + m_width * m_height * 4, 0);
    std::fill(m_depthBuffer, m_depthBuffer + m_width * m_height, std::numeric_limits<float>::max());
}

void GraphicsContext::present() {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_last_frame).count();

    m_last_frame = now;
    m_frameTimer += elapsed;

    if (m_frameTimer > 1000) {
        m_fpsAvg = m_frames;
        m_frames = 0;
        m_frameTimer = m_frameTimer - 1000;
    }

    // render frame
    SDL_UpdateTexture(m_texture, nullptr, m_buffer, m_width * 4);
    SDL_RenderCopy(m_renderer, m_texture, nullptr, nullptr);

    render_text(30, 30, std::string("fps: " + std::to_string(m_fpsAvg)).c_str());
    SDL_RenderPresent(m_renderer);

    m_frames = m_frames + 1;
}

void GraphicsContext::setup_texture() {
    if (m_texture) {
        SDL_DestroyTexture(m_texture);
    }

    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, m_width, m_height);

    if (m_depthBuffer) {
        delete [] m_depthBuffer;
    }

    if (m_buffer) {
        delete [] m_buffer;
    }

    m_depthBuffer = new float[m_width * m_height];
    m_buffer = new uint8_t[m_width * m_height * 4];
}

bool GraphicsContext::set_depth(unsigned int x, unsigned int y, float depth) {
    if (depth < 19) return false;
    int index = x + y * m_width;

    if (depth < m_depthBuffer[index]) {
        m_depthBuffer[index] = depth;
        return true;
    }

    return false;
}

// graphics drawing
void GraphicsContext::set_pixel(unsigned int x, unsigned int y, const color_t& color) {
    unsigned int index = (x + (y * m_width)) * 4;

    m_buffer[index + 3] = color.a;
    m_buffer[index + 2] = color.r;
    m_buffer[index + 1] = color.g;
    m_buffer[index + 0] = color.b;
}

void GraphicsContext::set_pixel_s(unsigned int x, unsigned int y, const color_t& color) {
    if (x < 0 || y < 0 || x >= m_width || y >= m_width) return;
    set_pixel(x, y, color);
}

void GraphicsContext::draw_line(int x0, int y0, int x1, int y1, const color_t& color) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = (dx > dy ? dx : -dy) / 2, e2;

    for (;;) {
        set_pixel_s(x0, y0, color);
        if (x0 == x1 && y0 == y1) break;
        e2 = err;
        if (e2 >-dx) { err -= dy; x0 += sx; }
        if (e2 < dy) { err += dx; y0 += sy; }
    }
}

void GraphicsContext::render_text(int x, int y, const char * text) {
    SDL_Surface * surface;
    SDL_Texture * texture;

    surface = TTF_RenderText_Solid(m_font, text, {255, 255, 255, SDL_ALPHA_OPAQUE});
    texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}