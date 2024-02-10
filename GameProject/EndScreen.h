#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"

class EndScreen : public GameObject {

	std::vector<std::string> m_spritesgui;
public:
	EndScreen();
	void init() override;
	void StarShow1(int stars);
	void StarShow2(int stars);
	void draw(int stars);
	void update(float dt) override;

	int option_selected() {
		return m_selected;
	}
	int option_locked() {
		if (this) {
			return m_option_locked;
		}
	}
	int set_option_locked(int option) {
		this->m_option_locked = option;
	}

private:
	graphics::Brush brush_gui;
	graphics::Brush brush_menu;
	graphics::Brush brush_next;

	int m_width;
	int m_height;
	int m_pos_x;
	int m_pos_y;

	int m_selected = 1;
	int m_option_locked;


};