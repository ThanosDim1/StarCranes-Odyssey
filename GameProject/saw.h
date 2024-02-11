#pragma once

#include "AnimatedObjects.h" 

// Saw class derived from AnimatedObjects class
class saw : public AnimatedObjects {
    // Vector to store paths of sprite frames for the saw animation
    std::vector<std::string> m_sprites_saw;

public:
    // Override the update function from the base class
    void update(float dt) override;

    // Draw function for the saw animation
    void draw(float add);

    // Override the initialization function from the base class
    void init() override;

    // Constructor for the saw class, initializing base class with position parameters
    saw(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}

protected:
    // Function for debugging purposes to draw outlines
    void debugDraw();
};

