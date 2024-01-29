#pragma once

#include "AnimatedObjects.h" 

class LevelDoor1 : public AnimatedObjects {
	std::vector<std::string> m_sprites_leveldoor1;
	bool animationPlayed = false;
	float animationtimerfordoor = 0.0f;
public:
	void update(float dt) override;
	void draw(float add);
	void init() override;
	LevelDoor1(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}
	
protected:
	void debugDraw();

};

