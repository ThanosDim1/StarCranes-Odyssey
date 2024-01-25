#pragma once

#include "gameobject.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include "player.h"
#include "enemy.h"
#include <memory>
#include "NonCollisionObject.h"
#include "AnimatedObjects.h"
#include "saw.h"



class Level : public GameObject
{
public:
	graphics::Brush m_brush_background;
	graphics::Brush m_brush_health_system;


	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_enemy_objects;
	std::vector<std::string> m_spriteshealthsystemfull;
	std::vector<std::string> m_spriteshealthsystemalmosthalf;
	std::vector<std::string> m_spriteshealthsystemhalf;
	std::vector<std::string> m_spriteshealthsystemlastlife;
	std::vector<std::string> m_spriteshealthsystemdeath;

	// add some collidable blocks
	std::vector<CollisionObject> m_blocks;
	std::vector<std::string> m_block_names;

	//add some non collidable blocks
	std::vector<NonCollisionObject> m_non_collidable_blocks;
	std::vector<std::string> m_non_collidable_block_names;

	AnimatedObjects* m_saw = new saw(5.0f,6.0f);
	saw* mn_saw = dynamic_cast<saw*>(m_saw);

	//add some enemies
	std::vector<std::unique_ptr<Enemy>> enemies;
	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;
	float animationtimerforhealthsystem = 0.0f;



	// dedicated method to draw a block
	void drawBlock(int i);
	void drawNonCollisionBlock(int i);

	// detect collisions
	void checkCollisions();
	void checkCollisionsForEnemy();

	void checkCollisionsMovingObjects();


public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	Level(const std::string& name = "Level0");
	~Level() override;
};