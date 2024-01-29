#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "Star.h"
#include "player.h"


void Star::update(float dt)
{

	AnimatedObjects::update(dt);
}

void Star::draw(float add)
{
	AnimatedObjects::draw(add, m_sprites_star);
	
}

void Star::init()
{

	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar1.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar2.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar3.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar4.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar5.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar6.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar7.png"));
	m_sprites_star.push_back(m_state->getFullAssetPath("RotatingStar8.png"));


	AnimatedObjects::init();
}

void Star::debugDraw()
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