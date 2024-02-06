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

public:
	void update(float dt) override;
	void draw() override;
	void init() override;

	Level2(const std::string& name = "Level2");
};
