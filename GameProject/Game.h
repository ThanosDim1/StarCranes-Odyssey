#pragma once

#include <string>
#include <list>
#include <sgg/graphics.h>

enum GAMESTATE {
	MAIN_MENU, PLAYING, RETRY, CHOOSE_LEVEL, ABOUT_SCREEN
};

enum MAIN_MENU_BUTTON {
	PLAY, ABOUT, EXIT
};

enum RETRY_MENU_BUTTON {
	AGAIN, BACK_TO_MENU
};

enum LEVEL_SELECTION {
	BEACH, MOUNTAIN, NIGHT
};

class Game {

private:
	class Level* m_current_level = 0;

public:

	int waveVariable = 8;

	GAMESTATE state = MAIN_MENU;
	MAIN_MENU_BUTTON buttonMM = PLAY;
	RETRY_MENU_BUTTON retry_choice = AGAIN;
	LEVEL_SELECTION level_button = BEACH;
	float arrow_offset;
	float button_timer;
	float timer;
	float timerLimit;

	Game();
	~Game();

	void init();
	void draw();
	void update();

	void updateTimers();

	void clearCollections();
};
