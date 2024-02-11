#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "Spikes.h"

// Update function for the 'Spikes' class
void Spikes::update(float dt)
{
    AnimatedObjects::update(dt);
}

// Draw function for the 'Spikes' class
void Spikes::draw(float add)
{
    AnimatedObjects::draw(add, m_sprites_spikes);

    if (m_state->m_debugging)
        debugDraw();
}

// Initialize function for the 'Spikes' class
void Spikes::init()
{
    // Load sprite for the spikes animation
    m_sprites_spikes.push_back(m_state->getFullAssetPath("Spikes.png"));

    // Initialize the base class (AnimatedObjects)
    AnimatedObjects::init();
}

// Debug draw function for the 'Spikes' class
void Spikes::debugDraw()
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

    // Draw a rectangle representing the spikes for debugging purposes
    graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, debug_brush);
}
