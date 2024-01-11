#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"

class Enemy : public Box, public GameObject
{
	std::vector<std::string> m_spritesenemy1;

	graphics::Brush m_brush_enemy;
	const float m_accel_horizontal = 20.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 2.0f;
	const float m_gravity = 12.0f;

public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	Enemy(std::string name) : GameObject(name) {}

protected:
	void debugDraw();

};