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

	m_spritesmoney.push_back("money1.png");
	m_spritesmoney.push_back("money2.png");
	m_spritesmoney.push_back("money3.png");
	m_spritesmoney.push_back("money4.png");
	m_spritesmoney.push_back("money5.png");
	m_spritesmoney.push_back("money6.png");

	m_spritesstar.push_back("RotatingStar1.png");
	m_spritesstar.push_back("RotatingStar2.png");
	m_spritesstar.push_back("RotatingStar3.png");
	m_spritesstar.push_back("RotatingStar4.png");
	m_spritesstar.push_back("RotatingStar5.png");
	m_spritesstar.push_back("RotatingStar6.png");
	m_spritesstar.push_back("RotatingStar7.png");
	m_spritesstar.push_back("RotatingStar8.png");

}

void Menu::draw() {
	graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, brush_background_menu);
	graphics::setFont(m_state->getFullAssetPath("Aerologica.ttf"));
	SETCOLOR(brush_menu.fill_color, 0, 0, 0);
	graphics::drawText(3.0f, 2.0f, 0.8f, "STARCRANE'S ODYSSEY", brush_menu);
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
	graphics::drawText(7.0f, 6.0f, 1, "PLAY", brush_play);
	graphics::drawText(6.6f, 7.0f, 1, "ABOUT", brush_about);
	graphics::drawText(7.0f, 8.0f, 1, "EXIT", brush_exit);
	graphics::setFont(m_state->getFullAssetPath("ThaleahFat.ttf"));
	graphics::drawText(2.3f, 9.4f, 0.5f, "Use the arrow keys to navigate and press ENTER to select.", brush_instructions);
	graphics::drawText(5.0f, 9.7f, 0.5f, "ESC BUTTON CLOSES THE GAME!", brush_instructions);


	drawAnimationStar(6.3f, 5.65f);
	drawAnimationStar(10.3f, 5.65f);
	drawAnimationStar(13.57f, 1.7f);
	drawAnimationStar(2.2f, 1.65f);
	drawAnimationMoney(6.3f, 6.65f);
	drawAnimationMoney(10.3f, 6.65f);

}

void Menu::update(float dt) {
	static std::chrono::steady_clock::time_point lastKeyPressTime = std::chrono::steady_clock::now();

	m_frameDelay++;
	if (m_frameDelay >= 8) { // Change this value to adjust the speed
		m_currentFrameMoney = (m_currentFrameMoney + 1) % m_spritesmoney.size();
		m_currentFrameStar = (m_currentFrameStar + 1) % m_spritesstar.size();
		m_frameDelay = 0;
	}



	// Check if enough time has passed since the last key press
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastKeyPressTime).count();
	if (elapsedTime < 160) {
		return;
	}

	if (graphics::getKeyState(graphics::SCANCODE_UP)) {
		if (m_selected != 1) {
			m_selected--;
		}
		else {
			m_selected = 3;
		}
		graphics::playSound(m_state->getFullAssetPath("option.wav"), 0.5f);
		std::cout << m_selected << std::endl;
		lastKeyPressTime = currentTime;
	}
	else if (graphics::getKeyState(graphics::SCANCODE_DOWN)) {
		if (m_selected != 3) {
			m_selected++;
		}
		else {
			m_selected = 1;

		}
		graphics::playSound(m_state->getFullAssetPath("option.wav"), 0.5f);
		std::cout << m_selected << std::endl;  // Print the selected value
		lastKeyPressTime = currentTime;  // Update the last key press time
	}

	if (graphics::getKeyState(graphics::SCANCODE_RETURN)) {
		m_option_locked = m_selected;
		graphics::playSound(m_state->getFullAssetPath("optionselected.wav"), 0.5f);
	}
}

void Menu::drawAnimationMoney(float x, float y) {
	graphics::Brush br;
	br.texture = m_state->getFullAssetPath(m_spritesmoney[m_currentFrameMoney]);
	br.outline_opacity = 0.0f;
	graphics::drawRect(x, y, 0.8f, 0.8f, br);
}

void Menu::drawAnimationStar(float x, float y) {
	graphics::Brush br;
	br.texture = m_state->getFullAssetPath(m_spritesstar[m_currentFrameStar]);
	br.outline_opacity = 0.0f;
	graphics::drawRect(x, y, 0.8f, 0.8f, br);
}