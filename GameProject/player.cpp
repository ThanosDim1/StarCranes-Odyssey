#include "player.h"
#include "util.h"
#include <cmath>

//void Player::update(float dt)
//{
//	float delta_time = dt / 1000.0f;
//
//	movePlayer(dt);
//
//	// update offset for other game objects
//	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
//	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;
//
//	GameObject::update(dt);
//
//}
//
//void Player::draw()
//{
//	int sprite = (int)fmod(100.0f - m_pos_x * 9.0f, m_sprites.size());
//
//	m_brush_player.texture = m_sprites[sprite];
//	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
//
//	if (m_state->m_debugging)
//		debugDraw();
//}
