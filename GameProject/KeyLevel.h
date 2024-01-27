#pragma once

#include "AnimatedObjects.h" 

class KeyLevel : public AnimatedObjects {
	std::vector<std::string> m_sprites_keylevel;

public:
	void update(float dt) override;
	void draw(float add);
	void init() override;
	KeyLevel(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}
	bool m_KeyisDeactivating = false;

protected:
	void debugDraw();

};

