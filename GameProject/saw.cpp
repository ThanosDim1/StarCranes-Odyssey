#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "saw.h"


void saw::update(float dt)
{
	AnimatedObjects::update(dt);
}

void saw::draw(float add)
{
	add = 0.5f;
	AnimatedObjects::draw(add,m_sprites_saw);
}

void saw::init()
{
	
	m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_01.png"));
	m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_02.png"));
	m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_03.png"));
	m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_04.png"));
	m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_05.png"));
	m_sprites_saw.push_back(m_state->getFullAssetPath("rotating_saw_06.png"));

	AnimatedObjects::init();

}
