#pragma once

#include "AnimatedObjects.h" 

class saw : public AnimatedObjects {
	std::vector<std::string> m_sprites_saw;
	float add = 0.5f;

public:
	bool isCollidingSaw = false;

public:
	void update(float dt) override;
	void draw(float add);
	void init() override;
	saw(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}

};

