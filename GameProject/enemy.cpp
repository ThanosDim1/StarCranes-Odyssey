#include "enemy.h"
#include "util.h"
#include "level.h"
#include <cmath>
#include <iostream>


void Enemy::update(float dt)
{
	float delta_time = dt / 1000.0f;

	std::printf("%f" , m_pos_y);
	// Move left at a constant speed
	m_pos_x -= (m_max_velocity * delta_time);
	

	//Stimulate gravity
	m_vy += m_gravity * delta_time;
	m_pos_y += m_vy * delta_time;

	GameObject::update(dt);

}

void Enemy::draw()
{
	int spritesenemy1 = (int)fmod(100.0f - m_pos_x * 3.0f, m_spritesenemy1.size());
	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;
	m_brush_enemy.texture = m_spritesenemy1[spritesenemy1];
	graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, 1.0f, 1.0f, m_brush_enemy);


	if (m_state->m_debugging)
		debugDraw();
}

void Enemy::init()
{
	// stage 1
	m_pos_x = 5.0f;
	m_pos_y = 5.0f;


	m_brush_enemy.fill_opacity = 1.0f;
	m_brush_enemy.outline_opacity = 0.0f;

	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_run1.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_run2.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_run3.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_run4.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_run5.png"));


	// Adjust width for finer collision detection
	m_width = 0.5f;
	m_height = 1.05f;
}



void Enemy::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, debug_brush);
}
