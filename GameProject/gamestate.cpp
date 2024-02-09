#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "util.h"
#include "menu.h"
#include "aboutpage.h"
#include <thread>
#include <chrono>
#include <iostream>


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
	if (m_about_page) {
		m_about_page->draw();
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
	
	/*if (isPaused()) {
		return;
		if (m_menu->option_locked() == 1) {
			resume();
			m_menu->update(dt);
		}
	}*/

	if (m_menu) {
		m_menu->update(dt);
		switch (m_menu->option_locked())
		{
		case 1:
			delete m_menu;
			m_menu = nullptr;

			m_current_level = new Level("1.lvl");
			m_current_level->init();

			m_player = new Player("Player");
			m_player->init();
			
			break;
		case 2:
			delete m_menu;
			m_menu = nullptr;

			m_about_page = new AboutPage();
			m_about_page->init();
			break;
		case 3:
			delete m_menu;
			graphics::stopMessageLoop();
			break;
		default:
			break;
		}
	}

	if (m_about_page) {
		m_about_page->update(dt);
	}

	if (m_current_level) {
		if (m_current_level->lvl1_finished && init_lvl2 == false) {
			delete m_current_level;
			delete m_player;
			m_current_level = new Level("2.lvl");
			m_player = new Player("Player");
			m_current_level -> lvl1_finished = true;
			m_current_level->init();
			m_player->init();
			init_lvl2 = true;
		}

		m_current_level->update(dt);

		if (m_current_level->lvl2_finished) {
			delete m_current_level;
			m_current_level = nullptr;
			delete m_player;
			m_player = nullptr;
			m_menu = new Menu();
			m_menu->init();
		}
	}
 
	m_debugging = graphics::getKeyState(graphics::SCANCODE_0);
	
	if (m_dead && deathtimer<2300) {
		graphics::playSound(getFullAssetPath("kyriakos.wav"), 0.07f);
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
		
		
		
		deathtimer += dt;

	}else{
		if (m_dead) {
			m_menu = new Menu();
			m_menu->init();
		}
		m_dead = false;
		deathtimer = 0.0f;	
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

void GameState::switchToMenu() {
	if (m_about_page) {
		delete m_about_page;
		m_about_page = nullptr;
	}

	m_menu = new Menu();
	m_menu->init();
}

GameState* GameState::m_unique_instance = nullptr;