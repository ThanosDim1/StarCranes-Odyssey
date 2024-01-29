#pragma once

#include "AnimatedObjects.h" 

class Spikes : public AnimatedObjects {
	std::vector<std::string> m_sprites_spikes;


public:
	void update(float dt) override;
	void draw(float add);
	void init() override;
	Spikes(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}

protected:
	void debugDraw();

};
