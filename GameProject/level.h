// Header guard to ensure the file is included only once
#pragma once

// Include necessary headers
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

// Level class derived from GameObject
class Level : public GameObject
{
public:
	// Brushes for background, health system, star system, key system, and text
	graphics::Brush m_brush_background;
	graphics::Brush m_brush_health_system;
	graphics::Brush m_brush_star_system;
	graphics::Brush m_brush_key_system;
	graphics::Brush brush_text;

	// Lists and vectors to store various game objects and sprites
	std::vector<GameObject*> m_static_objects;
	std::list<GameObject*> m_enemy_objects;
	std::vector<std::string> m_spriteshealthsystemfull;
	std::vector<std::string> m_spriteshealthsystemalmosthalf;
	std::vector<std::string> m_spriteshealthsystemhalf;
	std::vector<std::string> m_spriteshealthsystemlastlife;
	std::vector<std::string> m_spriteshealthsystemdeath;
	std::vector<std::string> m_spritesstarsystem;
	std::vector<std::string> m_spriteskeycollected;

	// Vector and names for collidable blocks
	std::vector<CollisionObject> m_blocks;
	std::vector<std::string> m_block_names;

	// Vector and names for non-collidable blocks
	std::vector<NonCollisionObject> m_non_collidable_blocks;
	std::vector<std::string> m_non_collidable_block_names;

	// LevelDoor1 object and pointer
	AnimatedObjects* m_leveldoor1 = new LevelDoor1(45.0f, 5.0f);
	LevelDoor1* mn_leveldoor1 = dynamic_cast<LevelDoor1*>(m_leveldoor1);

	// KeyLevel object and pointer
	AnimatedObjects* m_keylevel1 = new KeyLevel(43.0f, 9.0f);
	KeyLevel* mn_keylevel1 = dynamic_cast<KeyLevel*>(m_keylevel1);

	// Flags and variables for sound, collisions, and level progression
	bool soundPlayed = false;
	bool isCollidingLevelDoor1 = false;
	bool isCollidingSaw = false;
	bool isCollidingSpike = false;
	bool lvl1_finished = false;
	bool lvl2_finished = false;
	bool text_showed = false;

	// Brush and variables for block drawing
	const float m_block_size = 1.0f;
	graphics::Brush m_block_brush;
	graphics::Brush m_block_brush_debug;

	// Center coordinates and timers
	float m_center_x = 5.0f;
	float m_center_y = 5.0f;
	float animationtimerforhealthsystem = 0.0f;

	// Vectors for enemies, saws, stars, and spikes
	std::vector<Enemy*> enemies;
	std::vector<saw*> saws;
	std::vector<Star*> stars;
	std::vector<Spikes*> spikes;

	// Methods for drawing various game elements
	void drawBlock(int i, std::vector<CollisionObject> m_blocks);
	void drawNonCollisionBlock(int i, std::vector<NonCollisionObject> m_non_collidable_blocks);

	// Methods for collision detection
	void EnemiesCheck(float dt, std::vector<Enemy*> enemies);
	void ArrayCheck(const char* lvl1[20][74], const char* non_coll1[20][74]);
	void checkCollisionPlayerSpike(std::vector<Spikes*> spikes);
	void checkCollisionPlayerSaw(std::vector<saw*> saws);
	void checkCollisionPlayerStar(std::vector<Star*> stars);
	void checkCollisionPlayerKey(AnimatedObjects* m_keylevel, KeyLevel* mn_keylevel);
	void checkCollisionPlayerDoor();
	void checkCollisions(std::vector<CollisionObject> m_blocks);
	void checkCollisionsForEnemy(std::vector<CollisionObject> m_blocks, std::vector<Enemy*> enemies);
	void checkCollisionsMovingObjects(std::vector<Enemy*> enemies);

public:
	// Update, draw, and initialization methods
	void update(float dt);
	void draw();
	void drawKey();
	void drawStar();
	void drawHealthSystem();
	void init();

	// Method to initialize sprites
	void spitesinit();

	// Constructor with default name
	Level(const std::string& name = "Level0");

	// Destructor
	~Level() override;
};
