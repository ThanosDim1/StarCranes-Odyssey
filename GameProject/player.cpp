#include "player.h"
#include "util.h"
#include <cmath>
#include <iostream>

void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	movePlayer(dt);

	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;


	GameObject::update(dt);

}

void Player::draw()
{

	if (!graphics::getKeyState(graphics::SCANCODE_D) && !graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_W)) {
		int sprite_idle = (int)fmod(100.0f - m_pos_x * 3.0f, m_spritesidle.size());
		m_brush_player.texture = m_spritesidle[sprite_idle];
		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	}

	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		if (graphics::getKeyState(graphics::SCANCODE_D)) {
			int sprite_jumpright = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesjumpright.size());
			m_brush_player.texture = m_spritesjumpright[sprite_jumpright];
			graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
		}
		else if (graphics::getKeyState(graphics::SCANCODE_A)) {
			int sprite_jumpleft = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesjumpleft.size());
			m_brush_player.texture = m_spritesjumpleft[sprite_jumpleft];
			graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
		}
		else {
			int sprite_jumpright = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesjumpright.size());
			m_brush_player.texture = m_spritesjumpright[sprite_jumpright];
			graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
		}
	}
	else if (graphics::getKeyState(graphics::SCANCODE_D)) {
		int sprite_right = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesright.size());
		m_brush_player.texture = m_spritesright[sprite_right];
		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	}
	else if (graphics::getKeyState(graphics::SCANCODE_A)) {
		int sprite_left = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesleft.size());
		m_brush_player.texture = m_spritesleft[sprite_left];
		graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	}


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

	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run1.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run2.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run3.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run4.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run5.png"));

	m_spritesidle.push_back(m_state->getFullAssetPath("Biker_idle2.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("Biker_idle3.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("Biker_idle4.png"));

	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump1.png"));
	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump2.png"));
	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump3.png"));
	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump4.png"));

	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft1.png"));
	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft2.png"));
	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft3.png"));
	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft4.png"));

	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left1.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left2.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left3.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left4.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left5.png"));


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
	graphics::drawRect(m_state->getCanvasWidth() * 0.495f, m_state->getCanvasHeight() * 0.5f, m_width * 2.0f, m_height * 1.25f, debug_brush);

	char s[20];
	sprintf_s(s, "(%5.2f, %5.2f)", m_pos_x, m_pos_y);
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	debug_brush.fill_opacity = 1.0f;
	graphics::drawText(m_state->getCanvasWidth() * 0.5f - 0.4f, m_state->getCanvasHeight() * 0.5f, 0.15f, s, debug_brush);
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

	m_vy += delta_time * m_gravity;

	m_pos_y += delta_time * m_vy;






}