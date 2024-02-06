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
#include "LevelDoor1.h"
#include "KeyLevel.h"
#include "Star.h"
#include "Spikes.h"



class Level : public GameObject
{
public:
	graphics::Brush m_brush_background;
	graphics::Brush m_brush_health_system;
	graphics::Brush m_brush_star_system;
	graphics::Brush m_brush_key_system;


	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_enemy_objects;
	std::vector<std::string> m_spriteshealthsystemfull;
	std::vector<std::string> m_spriteshealthsystemalmosthalf;
	std::vector<std::string> m_spriteshealthsystemhalf;
	std::vector<std::string> m_spriteshealthsystemlastlife;
	std::vector<std::string> m_spriteshealthsystemdeath;
	std::vector<std::string> m_spritesstarsystem;
	std::vector<std::string> m_spriteskeycollected;

	// add some collidable blocks
	std::vector<CollisionObject> m_blocks;
	std::vector<std::string> m_block_names;

	//add some non collidable blocks
	std::vector<NonCollisionObject> m_non_collidable_blocks;
	std::vector<std::string> m_non_collidable_block_names;

	AnimatedObjects* m_leveldoor1= new LevelDoor1(45.0f, 5.0f);
	LevelDoor1* mn_leveldoor1 = dynamic_cast<LevelDoor1*>(m_leveldoor1);

	//add some keys
	AnimatedObjects* m_keylevel = new KeyLevel(43.0f, 9.0f);
	KeyLevel* mn_keylevel = dynamic_cast<KeyLevel*>(m_keylevel);

	bool soundPlayed = false;


	//add some enemies
	std::vector<Enemy*> enemies;
	std::vector<saw*> saws;
	std::vector<Star*> stars;
	std::vector<Spikes*> spikes;
	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	float m_center_x = 5.0f;
	float m_center_y = 5.0f;
	float animationtimerforhealthsystem = 0.0f;
	bool isCollidingLevelDoor1 = false;
	bool isCollidingSaw = false;
	bool isCollidingSpike = false;


	// dedicated method to draw a block
	void drawBlock(int i, std::vector<CollisionObject> m_blocks);
	void drawNonCollisionBlock(int i, std::vector<NonCollisionObject> m_non_collidable_blocks);

	

	// detect collisions
	void EnemiesCheck(float dt, std::vector<Enemy*> enemies);
	void ArrayCheck(const char* lvl1[20][74], const char* non_coll1[20][74]);
	
	void checkCollisionPlayerSpike();

	void checkCollisionPlayerSaw(std::vector<saw*> saws);

	void checkCollisionPlayerStar(std::vector<Star*> stars);

	void checkCollisionPlayerKey();

	void checkCollisionPlayerDoor();

	void checkCollisions(std::vector<CollisionObject> m_blocks);

	void checkCollisionsForEnemy(std::vector<CollisionObject> m_blocks, std::vector<Enemy*> enemies);

	void checkCollisionsMovingObjects(std::vector<Enemy*> enemies);


public:
	void update(float dt);
	void draw();
	void drawKey();
	void drawStar();
	void drawHealthSystem();
	void init();

	void spitesinit();

	Level(const std::string& name = "Level0");
	~Level() override;
	
};


