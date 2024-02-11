// Header guard to ensure the file is included only once
#pragma once

// Include necessary headers
#include "AnimatedObjects.h" 

// LevelDoor1 class derived from AnimatedObjects
class LevelDoor1 : public AnimatedObjects {
	// Sprite vector for the animated level door
	std::vector<std::string> m_sprites_leveldoor1;

	// Flag to track whether the door animation has been played
	bool animationPlayed = false;

	// Timer for the door animation
	float animationtimerfordoor = 0.0f;

public:
	// Update method to handle animation logic
	void update(float dt) override;

	// Draw method to render the level door
	void draw(float add);

	// Initialization method
	void init() override;

	// Constructor with position parameters
	LevelDoor1(float m_pos_x, float m_pos_y) : AnimatedObjects(m_pos_x, m_pos_y) {}

protected:
	// Debug draw method
	void debugDraw();
};
