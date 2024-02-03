#include "Menu.h"
#include "util.h"
#include <iostream>
#include <chrono>

Menu::Menu()
{
	this->GameObject::setActive(true);
	m_width = GameState::getInstance()->getCanvasWidth();
	m_height = GameState::getInstance()->getCanvasHeight();
	m_pos_x = GameState::getInstance()->getCanvasWidth() / 2.0f;
	m_pos_y = GameState::getInstance()->getCanvasHeight() / 2.0f;
}

void Menu::init() {
	brush_background_menu.texture = m_state->getFullAssetPath("backgroundmenu.png");
	brush_background_menu.outline_opacity = 0.0f;
}
 
void Menu::draw() {
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, brush_background_menu);
	graphics::setFont(m_state->getFullAssetPath("Aerologica.ttf"));
	SETCOLOR(brush_menu.fill_color, 0, 0, 0);
	graphics::drawText(4.0f , 2.0f , 1 , "PLATFORM GAME", brush_menu);
	if (m_selected == 1) {
		SETCOLOR(brush_play.fill_color, 1.f, 1.f, 0.f);
		SETCOLOR(brush_about.fill_color, 1.f, 1.f, 1.f);
		SETCOLOR(brush_exit.fill_color, 1.f, 1.f, 1.f);
	}
	else if (m_selected == 2) {
		SETCOLOR(brush_about.fill_color, 1.f, 1.f, 0.f);
		SETCOLOR(brush_play.fill_color, 1.f, 1.f, 1.f);
		SETCOLOR(brush_exit.fill_color, 1.f, 1.f, 1.f);
	}
	else {
		SETCOLOR(brush_exit.fill_color, 1.f, 1.f, 0.f);
		SETCOLOR(brush_play.fill_color, 1.f, 1.f, 1.f);
		SETCOLOR(brush_about.fill_color, 1.f, 1.f, 1.f);
	}
	graphics::drawText(7.0f , 6.0f, 1 , "PLAY", brush_play);
	graphics::drawText(6.6f, 7.0f, 1, "ABOUT", brush_about);
	graphics::drawText(7.0f, 8.0f, 1, "EXIT", brush_exit);
}

void Menu::update(float dt) {
	static std::chrono::steady_clock::time_point lastKeyPressTime = std::chrono::steady_clock::now();

	// Check if enough time has passed since the last key press
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastKeyPressTime).count();
	if (elapsedTime < 500) {  // Adjust the pause duration (500 milliseconds in this example)
		return;  // Pause for 500 milliseconds
	}

	if (graphics::getKeyState(graphics::SCANCODE_UP)) {
		if (m_selected != 1) {
			m_selected--;
		}
		else {
			m_selected = 3;
		}
		std::cout << m_selected << std::endl;  // Print the selected value
		lastKeyPressTime = currentTime;  // Update the last key press time
	}
	else if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
		if (m_selected != 3) {
			m_selected++;
		}
		else {
			m_selected = 1;
		}
		std::cout << m_selected << std::endl;  // Print the selected value
		lastKeyPressTime = currentTime;  // Update the last key press time
	}
}