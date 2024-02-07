/*#include "EndScreen.h"
#include "util.h"
#include "GameState.h"
#include <iostream>
#include <chrono>
#include "player.h"
#include "level.h"
#include "gamestate.h"


EndScreen::EndScreen()
{
	this->GameObject::setActive(true);
	m_width = GameState::getInstance()->getCanvasWidth();
	m_height = GameState::getInstance()->getCanvasHeight();
	m_pos_x = GameState::getInstance()->getCanvasWidth() / 2.0f;
	m_pos_y = GameState::getInstance()->getCanvasHeight() / 2.0f;
}

void EndScreen::init() {

	brush_menu.texture = m_state->getFullAssetPath("Menu_BTNinactive.png");
	brush_menu.outline_opacity = 0.0f;

	brush_replay.texture = m_state->getFullAssetPath("Replayinactive_BTN.png");
	brush_replay.outline_opacity = 0.0f;

	brush_next.texture = m_state->getFullAssetPath("Play_BTNinactive.png");
	brush_next.outline_opacity = 0.0f;

	m_spritesgui.push_back("level1stars0.png");
	m_spritesgui.push_back("level1stars1.png");
	m_spritesgui.push_back("level1stars2.png");
	m_spritesgui.push_back("level1stars3.png");
	m_spritesgui.push_back("level2stars0.png");
	m_spritesgui.push_back("level2stars1.png");
	m_spritesgui.push_back("level2stars2.png");
	m_spritesgui.push_back("level2stars3.png");
}

void EndScreen::draw() {
	if (m_state->getLevel()->isCollidingLevelDoor1 && m_state->getPlayer()->m_player_has_key && m_state->getPlayer()->m_player_has_star == 0) {
		graphics::Brush br;
		br.texture = m_state->getFullAssetPath("level1stars0.png");
		br.outline_opacity = 0.0f; // No outline
		graphics::drawRect(m_pos_x, m_pos_y, m_width, m_height, br);
	}
	if (m_selected == 1) {
		SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 0.f);
		SETCOLOR(brush_replay.fill_color, 1.f, 1.f, 1.f);
		SETCOLOR(brush_next.fill_color, 1.f, 1.f, 1.f);
	}
	else if (m_selected == 2) {
		SETCOLOR(brush_replay.fill_color, 1.f, 1.f, 0.f);
		SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 1.f);
		SETCOLOR(brush_next.fill_color, 1.f, 1.f, 1.f);
	}
	else {
		SETCOLOR(brush_next.fill_color, 1.f, 1.f, 0.f);
		SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 1.f);
		SETCOLOR(brush_replay.fill_color, 1.f, 1.f, 1.f);
	}

}

void EndScreen::update(float dt) {
	static std::chrono::steady_clock::time_point lastKeyPressTime = std::chrono::steady_clock::now();
	// Check if enough time has passed since the last key press
	auto currentTime = std::chrono::steady_clock::now();
	auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastKeyPressTime).count();
	if (elapsedTime < 160) {
		return;
	}

	if (graphics::getKeyState(graphics::SCANCODE_LEFT)) {
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
	else if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) {
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
		if (m_option_locked == 1) {
			GameState::getInstance()->switchToMenu();
		}
		if (m_option_locked == 2) {
			GameState::getInstance()->new Level("1.lvl");
			GameState::getInstance()->getLevel()->init();
			GameState::getInstance()->new Player("Player");
			GameState::getInstance()->getPlayer()->init();

		}
		if (m_option_locked == 3) {
			GameState::getInstance()->new Level("2.lvl");
			GameState::getInstance()->getLevel()->init();
			GameState::getInstance()->new Player("Player");
			GameState::getInstance()->getPlayer()->init();
		}
	}
}*/