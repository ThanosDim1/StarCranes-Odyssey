#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "LevelDoor1.h"
#include "player.h"


void LevelDoor1::update(float dt)
{
	AnimatedObjects::update(dt);
}

void LevelDoor1::draw(float add)
{
	if (m_state->getPlayer()->openDoor()){
		AnimatedObjects::draw(add, m_sprites_leveldoor1);
	}
	else {
		AnimatedObjects::draw(0, m_sprites_leveldoor1);
	}

	if (m_state->m_debugging)
		debugDraw();
}

void LevelDoor1::init()
{

	m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door1.png"));
	m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door2.png"));
	m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door3.png"));
	m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door4.png"));
	m_sprites_leveldoor1.push_back(m_state->getFullAssetPath("Door5.png"));

	AnimatedObjects::init();

}

void LevelDoor1::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 0, 1, 1);
	SETCOLOR(debug_brush.outline_color, 0, 1, 0);
	debug_brush.fill_opacity = 0.3f;
	debug_brush.outline_opacity = 1.0f;
	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;
	graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, debug_brush);
}