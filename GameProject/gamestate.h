// Header guard to ensure the file is included only once
#pragma once

// Include necessary headers
#include "sgg/graphics.h"
#include <string>

// Forward declarations of related classes
class Menu;
class Level;
class AboutPage;
class Player;
class EndScreen;

// GameState class for managing game state
class GameState
{
private:
	// Singleton instance
	static GameState* m_unique_instance;

	// Asset path and canvas dimensions
	const std::string m_asset_path = "assets\\";
	const float m_canvas_width = 16.0f;
	const float m_canvas_height = 10.0f;

	// Pointers to various game components
	class Menu* m_menu = nullptr;
	class Level* m_current_level = nullptr;
	class AboutPage* m_about_page = nullptr;
	class Player* m_player = nullptr;
	class EndScreen* m_endscreen = nullptr;

	// Brushes for dead background and text
	graphics::Brush brush_background_dead;
	graphics::Brush brush_dead;

	// Private constructor to enforce singleton pattern
	GameState();

protected:
	// Flags and timer for game state management
	bool m_active = true;
	float deathtimer = 0.0f;

public:
	// Global offsets, debugging, and death flags
	float m_global_offset_x = 0.0f;
	float m_global_offset_y = 0.0f;
	bool m_debugging = false;
	bool m_dead = false;

	// Flags for level initialization, end screen, and returning to the menu
	bool init_lvl2 = false;
	bool endscreen_init = false;
	bool return_to_menu = false;

	// Temporary variable for storing stars
	int temp_stars;

public:
	// Destructor
	~GameState();

	// Singleton instance retrieval
	static GameState* getInstance();

	// Initialization method
	bool init();

	// Drawing and updating methods
	void draw();
	void update(float dt);

	// Methods for handling asset paths and directories
	std::string getFullAssetPath(const std::string& asset);
	std::string getAssetDir();

	// Getters for canvas dimensions
	float getCanvasWidth() { return m_canvas_width; }
	float getCanvasHeight() { return m_canvas_height; }

	// Getters for player and current level instances
	class Player* getPlayer() { return m_player; }
	class Level* getLevel() { return m_current_level; }

	// Setter for game activity
	void setActive(bool a) { m_active = a; }

	// Method to switch to the main menu
	void switchToMenu();
	
};