#include "AnimatedObjects.h"
#include "gameobject.h"
#include <sgg/graphics.h>


void AnimatedObjects::update(float dt)
{
	GameObject::update(dt);
}

void AnimatedObjects::draw(float add, std::vector<std::string> m_spritesanimatedobjects)
{
	animationtimer += add;
	int spritesanimatedobjects = (int)fmod(animationtimer , m_spritesanimatedobjects.size());
	m_brush_animatedobjects.texture = m_spritesanimatedobjects[spritesanimatedobjects];
	m_brush_animatedobjects.outline_opacity = 0.0f;
	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;
	graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, m_brush_animatedobjects);
}

void AnimatedObjects::init()
{
	m_width = 1.0f;
	m_height = 1.0f;
}