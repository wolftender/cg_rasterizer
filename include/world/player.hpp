#include "world/cube.hpp"

class Player : public Cube {
    private:
        bool m_move_left, m_move_right, m_move_forward, m_move_reverse;

    public:
        void event(const SDL_Event& event) override;
        void update(Level & level, float delta_time) override;
        void render(GraphicsContext& context, const mat_t<float> & projection, const mat_t<float> & view) override;
};