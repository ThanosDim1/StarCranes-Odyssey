#include "AnimatedObjects.h"
#include "gameobject.h"
#include <sgg/graphics.h>

// Update method for AnimatedObjects
void AnimatedObjects::update(float dt)
{
    // Call the base class update method
    GameObject::update(dt);
}

// Draw method for AnimatedObjects
void AnimatedObjects::draw(float add, std::vector<std::string> m_spritesanimatedobjects)
{
    // Update the animation timer based on the provided time increment
    animationtimer += add;

    // Calculate the index of the current sprite in the animation sequence
    int spritesanimatedobjects = (int)fmod(animationtimer, m_spritesanimatedobjects.size());

    // Set the texture for the brush to the current sprite
    m_brush_animatedobjects.texture = m_spritesanimatedobjects[spritesanimatedobjects];
    m_brush_animatedobjects.outline_opacity = 0.0f;

    // Apply global offset to position
    float offset_x = m_state->m_global_offset_x;
    float offset_y = m_state->m_global_offset_y;

    // Draw the animated object using the graphics library
    graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, m_brush_animatedobjects);
}

// Initialization method for AnimatedObjects
void AnimatedObjects::init()
{
    // Set default dimensions for the animated object
    m_width = 1.0f;
    m_height = 1.0f;
}
