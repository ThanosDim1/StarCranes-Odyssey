#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "box.h"

class Player : public Box, public GameObject
{
	// animated player
	std::vector<std::string> m_spritesright;
	std::vector<std::string> m_spritesleft;
	std::vector<std::string> m_spritesidle;
	std::vector<std::string> m_spritesjumpright;
	std::vector<std::string> m_spritesjumpleft;

	graphics::Brush m_brush_player;
	const float m_accel_horizontal = 20.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 5.0f;
	const float m_gravity = 10.0f;

public:
	float m_vx = 0.0f;
	float m_vy = 0.0f;


public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	Player(std::string name) : GameObject(name) {}

protected:
	void debugDraw();

	// dynamic motion control
	void movePlayer(float dt);
};