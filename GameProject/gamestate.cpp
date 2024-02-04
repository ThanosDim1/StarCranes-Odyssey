#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "util.h"
#include "menu.h"
#include <thread>
#include <chrono>
#include <iostream>
using namespace std::chrono_literals;

GameState::GameState()
{
}

GameState::~GameState()
{
	if (m_current_level)
		delete m_current_level;
}

GameState* GameState::getInstance()
{
	if (!m_unique_instance)
	{
		m_unique_instance = new GameState();
	}
	return m_unique_instance;
}


bool GameState::init()
{
	m_menu = new Menu();
	m_menu->init();
	
	graphics::preloadBitmaps(getAssetDir());

	return true;
}

void GameState::draw()
{

	if (m_menu) {
		m_menu->draw();
	}
	
	if (m_current_level) {
		m_current_level->draw();
	}
}

void GameState::update(float dt)
{
	// Skip an update if a long delay is detected 
	// to avoid messing up the collision simulation
	if (dt > 500) // ms
		return;

	// Avoid too quick updates
	float sleep_time = std::max(17.0f - dt, 0.0f);
	if (sleep_time > 0.0f)
	{
		std::this_thread::sleep_for(std::chrono::duration<float, std::milli>(sleep_time));
	}

	if (m_menu) {
		m_menu->update(dt);
		if (m_menu->option_locked() == 1) {

			delete m_menu;
			m_menu = nullptr;

			m_current_level = new Level("1.lvl");
			m_current_level->init();

			m_player = new Player("Player");
			m_player->init();

		}
		if (m_menu->option_locked() == 3) {
			graphics::stopMessageLoop();
		}
	}

	if (m_current_level) {
		m_current_level->update(dt);
	}
 

	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);

	
	if (m_dead) {
		delete m_current_level;
		m_current_level = nullptr;
		m_player = nullptr;

		float m_pos_x = m_canvas_width / 2.0f;
		float m_pos_y = m_canvas_height / 2.0f;

		SETCOLOR(brush_background_dead.fill_color, 0.0f, 0.0f, 0.0f);
		graphics::drawRect(m_pos_x, m_pos_y, m_canvas_width, m_canvas_height, brush_background_dead);
		graphics::setFont(getFullAssetPath("Aerologica.ttf"));
		SETCOLOR(brush_dead.fill_color, 1.f, 0, 0);
		graphics::drawText(4.0f, 5.5f, 2, "YOU DIED", brush_dead);

		m_dead = false;
		m_menu = new Menu();
		m_menu->init();
	}
}

std::string GameState::getFullAssetPath(const std::string& asset)
{
	return m_asset_path + asset;
}

std::string GameState::getAssetDir()
{
	return m_asset_path;
}


GameState* GameState::m_unique_instance = nullptr;