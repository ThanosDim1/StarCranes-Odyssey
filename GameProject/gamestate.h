#pragma once

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
	class Player* m_player = 0;


	GameState();

protected:
	bool m_active = true;

public:
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	
	bool m_debugging = false;
	

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

};