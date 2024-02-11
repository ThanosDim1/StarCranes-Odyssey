#pragma once
#include "sgg/graphics.h"
#include <string>

class GameState
{
private:
	static GameState* m_unique_instance;

	const std::string m_asset_path = "assets\\";

	const float m_canvas_width = 16.0f;
	const float m_canvas_height = 10.0f;

	class Menu* m_menu = 0;
	class Level* m_current_level = 0;
	class AboutPage* m_about_page = 0;
	class Player* m_player = 0;
	class EndScreen* m_endscreen = 0;

	graphics::Brush brush_background_dead;
	graphics::Brush brush_dead;

	GameState();

protected:
	bool m_active = true;
	float deathtimer = 0.0f;

public:
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	
	bool m_debugging = false;
	bool m_dead = false;
	bool init_lvl2 = false;
	bool endscreen_init = false;
	bool return_to_menu = false;
	
	int temp_stars;
	//bool m_paused = false;
	
	
public:
	~GameState();
	static GameState* getInstance();

	bool init();
	void draw();
	void update(float dt);

	std::string getFullAssetPath(const std::string& asset);
	std::string getAssetDir();

	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	class Player* getPlayer() { return m_player; }
	class Level* getLevel() { return m_current_level; }
	void setActive(bool a) { m_active = a; }

	void switchToMenu();
	
};