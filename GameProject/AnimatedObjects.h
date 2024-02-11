#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "CollisionObject.h"

// AnimatedObjects class: Represents objects in the game that are animated and can collide with other objects.
class AnimatedObjects : public GameObject, public CollisionObject {

    float animationtimer = 0.0f; // Timer to manage the animation frames.
    graphics::Brush m_brush_animatedobjects; // Brush to draw the animated objects.

public:
    // Override the update method from GameObject to implement custom update logic, such as animation progression.
    void update(float dt) override;

    // Draw method to render the animated object. Takes an additional float and a vector of sprite paths.
    void draw(float add, std::vector<std::string> m_spritesanimatedobjects);

    // Override the init method from GameObject to perform initialization tasks specific to animated objects.
    void init() override;

    // Constructor to initialize an animated object with position coordinates.
    AnimatedObjects(float m_pos_x, float m_pos_y) {
        this->m_pos_x = m_pos_x; // Set the x position of the object.
        this->m_pos_y = m_pos_y; // Set the y position of the object.
    }
};
