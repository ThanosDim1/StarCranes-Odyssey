
#pragma once

#include "AnimatedObjects.h" 

class Star : public AnimatedObjects {
	std::vector<std::string> m_sprites_star;

public:
	void update(float dt) override;
	void draw(float add);
	void init() override;
	Star(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}
	bool m_StarisDeactivating = false;

protected:
	void debugDraw();

};

