#pragma once
#include "world/maze.hpp"
#include "world/sphere.hpp"

class Room : public Maze {
    private:
        Sphere & m_sphere1;
        Sphere & m_sphere2;

    public:
        Room();
        void update(float delta_time) override;
};