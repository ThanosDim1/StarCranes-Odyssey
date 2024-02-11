#pragma once

#include "AnimatedObjects.h" 

// Star class derived from AnimatedObjects class
class Star : public AnimatedObjects {
    // Vector to store paths of sprite frames for the star animation
    std::vector<std::string> m_sprites_star;

public:
    // Override the update function from the base class
    void update(float dt) override;

    // Draw function for the star animation
    void draw(float add);

    // Override the initialization function from the base class
    void init() override;

    // Constructor for the Star class, initializing base class with position parameters
    Star(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}

    // Flag to track whether the star is in the process of deactivating
    bool m_StarisDeactivating = false;

protected:
    // Function for debugging purposes to draw outlines
    void debugDraw();
};

