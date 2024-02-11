#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "LevelDoor1.h"
#include "player.h"

// Function to update the LevelDoor1 object
void LevelDoor1::update(float dt)
{
    // Call the base class's update function
    AnimatedObjects::update(dt);
}

// Function to draw the LevelDoor1 object
void LevelDoor1::draw(float add)
{
    // Check if the player is opening the door and adjust the animation accordingly
    if (m_state->getPlayer()->openDoor()) {
        AnimatedObjects::draw(add, m_sprites_leveldoor1);
    }
    else {
        AnimatedObjects::draw(0, m_sprites_leveldoor1);
    }

    // Debug drawing if debugging is enabled
    if (m_state->m_debugging)
        debugDraw();
}

// Function to initialize the LevelDoor1 object
void LevelDoor1::init()
{
    // Load door sprites
    m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door1.png"));
    m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door2.png"));
    m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door3.png"));
    m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door4.png"));
    m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door5.png"));

    // Call the base class's init function
    AnimatedObjects::init();
}

// Function to draw debug information for LevelDoor1 object
void LevelDoor1::debugDraw()
{
    // Debug drawing settings
    graphics::Brush debug_brush;
    SETCOLOR(debug_brush.fill_color, 0, 1, 1);   
    SETCOLOR(debug_brush.outline_color, 0, 1, 0);
    debug_brush.fill_opacity = 0.3f;              
    debug_brush.outline_opacity = 1.0f;           

    // Calculate drawing position with global offset
    float offset_x = m_state->m_global_offset_x;
    float offset_y = m_state->m_global_offset_y;

    // Draw a rectangle around the door for debugging purposes
    graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, debug_brush);
}
