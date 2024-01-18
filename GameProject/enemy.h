#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "CollisionObject.h"

class Enemy : public CollisionObject, public GameObject
{
	std::vector<std::string> m_spritesenemy1;
	std::vector<std::string> m_spritesdeactivation;
	std::vector<std::string> m_spritesleftattack;
	std::vector<std::string> m_spritesrightattack;
	int m_currentDeactivationSprite = 0;
	float animationtimerfordeath =0.0f;
	float animationtimer = 0.0f;
	
	graphics::Brush m_brush_enemy;
	const float m_accel_horizontal = 20.0f;
	const float m_accel_vertical = 300.1f;
	const float m_max_velocity = 2.0f;
	const float m_gravity = 12.0f;

public:
	float m_vx = 2.0f;
	float m_vy = 0.0f;

public:
	void update(float dt) override;
	void draw() override;
	void init() override;
	Enemy(std::string name) : GameObject(name) {}
	bool m_isDeactivating = false;


protected:
	void debugDraw();

};


