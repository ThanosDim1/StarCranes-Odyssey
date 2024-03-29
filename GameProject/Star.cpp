#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "Star.h"
#include "player.h"

// Update function for the 'Star' class
void Star::update(float dt)
{
    // Call the update function of the base class (AnimatedObjects)
    AnimatedObjects::update(dt);
}

// Draw function for the 'Star' class
void Star::draw(float add)
{
    // Call the draw function of the base class (AnimatedObjects) to draw the star animation
    AnimatedObjects::draw(add, m_sprites_star);
}

// Initialize function for the 'Star' class
void Star::init()
{
    // Load sprite frames for the star animation
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar1.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar2.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar3.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar4.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar5.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar6.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar7.png"));
    m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar8.png"));

    // Initialize the base class (AnimatedObjects)
    AnimatedObjects::init();
}

// Debug draw function for the 'Star' class
void Star::debugDraw()
{
    // Set up a debug brush for drawing outlines
    graphics::Brush debug_brush;
    SETCOLOR(debug_brush.fill_color, 0, 1, 1);
    SETCOLOR(debug_brush.outline_color, 0, 1, 0);
    debug_brush.fill_opacity = 0.3f;
    debug_brush.outline_opacity = 1.0f;

    // Adjust position with global rendering offset
    float offset_x = m_state->m_global_offset_x;
    float offset_y = m_state->m_global_offset_y;

    // Draw a rectangle representing the star for debugging purposes
    graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, debug_brush);
}
