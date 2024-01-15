#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"
#include "enemy.h"


class Level : public GameObject
{
public:
	graphics::Brush m_brush_background;

	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_enemy_objects;

	// add some collidable blocks
	std::vector<CollisionObject> m_blocks;
	std::vector<std::string> m_block_names;
	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;
	bool isCollidingSideways = false;
	bool isCollidingDown = false;
	bool isCollidingSidewaysEnemy = false;
	bool isCollidingDownEnemy = false;

	// dedicated method to draw a block
	void drawBlock(int i);

	// detect collisions
	void checkCollisions();
	void checkCollisionsForEnemy();

	void checkCollisionsMovingObjects();
	

public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	bool getCollSideways() { return isCollidingSideways; }
	bool getCollDown() { return isCollidingDown; }

	Level(const std::string& name = "Level0");
	~Level() override;
};