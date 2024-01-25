#include "AnimatedObjects.h"
#include "gameobject.h"
#include <sgg/graphics.h>


void AnimatedObjects::update(float dt)
{
	GameObject::update(dt);
}

void AnimatedObjects::draw(float add)
{
	animationtimer += add;
	int spritesanimatedobjects = (int)fmod(animationtimer / 100, m_spritesanimatedobjects.size());
	m_brush_animatedobjects.texture = m_spritesanimatedobjects[spritesanimatedobjects];
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, m_brush_animatedobjects);
}

void AnimatedObjects::init()
{
	m_width = 1.0f;
	m_height = 1.0f;
}


void AnimatedObjects::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, debug_brush);

}

