#include "player.h"
#include "util.h"
#include <cmath>
#include <iostream>

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	movePlayer(dt);

	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x ;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;


	GameObject::update(dt);

}

void Player::draw()
{
	graphics::drawRect(m_state->getCanvasWidth() * 0.5, m_state->getCanvasHeight() * 0.8f, 2.0f, 2.0f, m_brush_player);
	
	if (m_state->m_debugging)
		debugDraw();
}

void Player::init()
{
	// stage 1
	m_pos_x = 5.0f;
	m_pos_y = 5.0f;

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;
	m_brush_player.texture = m_state->getFullAssetPath("Attack_1temp.png");

	// Adjust width for finer collision detection
	m_width = 0.3f;
	m_height = 1.0f;
}



void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth()*0.5f, m_state->getCanvasHeight() * 0.8f, m_width, m_height, debug_brush);
	
	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f - 0.6f, 0.15f, s, debug_brush);
}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;

	const float velocity = 5.0f;
	if (graphics::getKeyState(graphics::SCANCODE_A))
		m_pos_x -= (delta_time * velocity);
	if (graphics::getKeyState(graphics::SCANCODE_D))
		(m_pos_x += delta_time * velocity);
	if (graphics::getKeyState(graphics::SCANCODE_W))
		(m_pos_y -= delta_time * velocity);
	if (graphics::getKeyState(graphics::SCANCODE_S))
		(m_pos_y += delta_time * velocity);
	




}


