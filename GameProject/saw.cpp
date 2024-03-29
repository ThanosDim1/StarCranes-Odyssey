#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "saw.h"

// Update function for the 'saw' class
void saw::update(float dt)
{
    AnimatedObjects::update(dt);
}

// Draw function for the 'saw' class
void saw::draw(float add)
{
    AnimatedObjects::draw(add, m_sprites_saw);

    if (m_state->m_debugging)
        debugDraw();
}

// Initialize function for the 'saw' class
void saw::init()
{
    // Load sprites for the rotating saw animation
    m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_01.png"));
    m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_02.png"));
    m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_03.png"));
    m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_04.png"));
    m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_05.png"));
    m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_06.png"));

    // Initialize the base class (AnimatedObjects)
    AnimatedObjects::init();
}

// Debug draw function for the 'saw' class
void saw::debugDraw()
{
    // Set up a debug brush for drawing outlines
    graphics::Brush debug_brush;
    SETCOLOR(debug_brush.fill_color, 0, 0, 1);
    SETCOLOR(debug_brush.outline_color, 0, 0, 1);
    debug_brush.fill_opacity = 0.1f;
    debug_brush.outline_opacity = 1.0f;

    // Adjust position with global rendering offset
    float offset_x = m_state->m_global_offset_x;
    float offset_y = m_state->m_global_offset_y;

    // Draw a rectangle representing the saw for debugging purposes
    graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, debug_brush);
}
