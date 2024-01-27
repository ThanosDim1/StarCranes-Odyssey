#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "KeyLevel.h"
#include "player.h"


void KeyLevel::update(float dt)
{
	
	AnimatedObjects::update(dt);
}

void KeyLevel::draw(float add)
{
	if (m_state->getPlayer()->m_player_has_key)
	{
		m_KeyisDeactivating = true;
		setActive(false);
	}
	else {
		AnimatedObjects::draw(add, m_sprites_keylevel);
	}
	

	if (m_state->m_debugging)
		debugDraw();
}

void KeyLevel::init()
{

	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey1.png"));	
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey2.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey3.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey4.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey5.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey6.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey7.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey8.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey9.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey10.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey11.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey12.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey13.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey14.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey15.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey16.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey17.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey18.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey19.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey20.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey21.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey22.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey23.png"));
	m_sprites_keylevel.push_back(m_state->getFullAssetPath("AnimatedKey24.png"));
	
	
	AnimatedObjects::init();
}

void KeyLevel::debugDraw()
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