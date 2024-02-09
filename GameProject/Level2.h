#pragma once

#include "gameobject.h"
#include "gamestate.h"
#include <vector>
#include <list>
#include <string>
#include <sgg/graphics.h>
#include <memory>
#include "level.h" 

class Level2 : public GameObject, public Level
{
	graphics::Brush m_brush_background;

	std::vector<CollisionObject> m_blocks2;
	std::vector<NonCollisionObject> m_non_collidable_blocks2;

	std::vector<std::string> m_block_names2;
	std::vector<std::string> m_non_collidable_block_names2;

	AnimatedObjects* m_leveldoor2 = new LevelDoor1(45.0f, 5.0f);
	LevelDoor1* mn_leveldoor2 = dynamic_cast<LevelDoor1*>(m_leveldoor2);

	AnimatedObjects* m_keylevel2 = new KeyLevel(43.0f, 9.0f);
	KeyLevel* mn_keylevel2 = dynamic_cast<KeyLevel*>(m_keylevel2);


	std::vector<Enemy*> enemies2;
	std::vector<saw*> saws2;
	std::vector<Star*> stars2;
	std::vector<Spikes*> spikes2;

	void ArrayCheck(const char* lvl[20][74], const char* non_coll[20][74]) ;
public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	

	Level2(const std::string& name = "Level2");
};
