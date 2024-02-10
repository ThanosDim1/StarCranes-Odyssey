#include "EndScreen.h"
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

void EndScreen::StarShow1(int stars) {
	graphics::Brush br;
	br.texture = m_state->getFullAssetPath("level1stars" + std::to_string(stars) +  ".png");
	br.outline_opacity = 0.0f; // No outline
	graphics::drawRect(m_pos_x / 2.0f, m_pos_y / 2.0f, 7.0f, 7.0f, br);
}

void EndScreen::StarShow2(int stars) {
	graphics::Brush br;
	br.texture = m_state->getFullAssetPath("level2stars" + std::to_string(stars) + ".png");
	br.outline_opacity = 0.0f; // No outline
	graphics::drawRect(m_pos_x / 2.0f, m_pos_y / 2.0f, 7.0f, 7.0f, br);
}


void EndScreen::draw(int stars) {
	if (m_state->getLevel() && (m_state->getLevel()->lvl1_finished || m_state->getLevel()->lvl2_finished)) {

		if (m_state->getLevel()->lvl1_finished && !m_state->init_lvl2) {
			StarShow1(stars);
		}
		if (m_state->init_lvl2) {
			StarShow2(stars);
		}

		graphics::drawRect(5.0f, 8.0f, 1.0f, 1.0f, brush_menu);
		graphics::drawRect(10.0f, 8.0f, 1.0f, 1.0f, brush_next);

		if (m_selected == 1) {
			SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 0.f);
			SETCOLOR(brush_next.fill_color, 1.f, 1.f, 1.f);
		}
		else {
			SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 1.f);
			SETCOLOR(brush_next.fill_color, 1.f, 1.f, 0.f);
		}
	}
}

void EndScreen::update(float dt) {
	if (m_state->getLevel() && (m_state->getLevel()->lvl1_finished || m_state->getLevel()->lvl2_finished)) {
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
				m_selected = 2;
			}
			graphics::playSound(m_state->getFullAssetPath("option.wav"), 0.5f);
			lastKeyPressTime = currentTime;
		}
		if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) {
			if (m_selected != 2) {
				m_selected++;
			}
			else {
				m_selected = 1;

			}
			graphics::playSound(m_state->getFullAssetPath("option.wav"), 0.5f);
			lastKeyPressTime = currentTime;  // Update the last key press time
		}
		if (graphics::getKeyState(graphics::SCANCODE_F)) {
			m_option_locked = m_selected;
			graphics::playSound(m_state->getFullAssetPath("optionselected.wav"), 0.5f);
		}
	}
}
