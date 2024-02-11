// Header guard to ensure the file is included only once
#pragma once

// Include necessary header
#include "AnimatedObjects.h"

// KeyLevel class derived from AnimatedObjects
class KeyLevel : public AnimatedObjects {
	std::vector<std::string> m_sprites_keylevel;

public:
	// Override update method from AnimatedObjects
	void update(float dt) override;

	// Override draw method from AnimatedObjects
	void draw(float add);

	// Override init method from AnimatedObjects
	void init() override;

	// Constructor with initial position
	KeyLevel(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}

	// Flag to check if the key is deactivating
	bool m_KeyisDeactivating = false;

protected:
	// Debug drawing method
	void debugDraw();

};

