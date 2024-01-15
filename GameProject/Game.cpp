#include "Game.h"
#include "level.h"

Game::Game() {
}

Game::~Game() {
}

void Game::init() {
	timer = 0.f;
	timerLimit = 4000.f;
	button_timer = .0f;
	arrow_offset = 0.f;
	if (state == MAIN_MENU) {
		graphics::stopMusic();
		graphics::playMusic(std::string(POKEMON_THEME_SONG), 0.05f);
	}

	if (state == CHOOSE_LEVEL) {
		graphics::playSound(std::string(MAKE_YOUR_SELECTION_NOW), 0.3f);
		level_button = BEACH;
	}


	if (state == PLAYING) {
		graphics::stopMusic();
		graphics::playMusic(std::string(JOJOS_MAIN_MUSIC), 0.05f);
	}

	m_current_level = new Level("1.lvl");
	m_current_level->init();
}

void Game::draw()
{

	switch (state) {
	case PLAYING: {

		// background
		Brush br;
		br.fill_opacity = 1;
		br.outline_opacity = 0;
		br.texture = level_asset;
		drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		// Score text
		Brush br0;
		br0.fill_opacity = 1;
		br0.outline_opacity = 0;
		br0.texture = "";
		br0.fill_color[0] = .0f;
		br0.fill_color[1] = .0f;
		br0.fill_color[2] = .0f;
		setFont(string(POCOYO_TV_FONT));
		drawText(160, 50, 30, "Score: " + to_string(score), br0);

		// Goku player picture
		Brush br1;
		br1.fill_opacity = 1;
		br1.outline_opacity = 0;
		br1.texture = string(GOKU_PROFILE_PIC);
		drawRect(65, 57, 75, 75, br1);

		// UI Hp bar
		float player_hp = (player->getHp() > 0) ? player->getHp() : 0;
		Brush br3;
		br3.fill_opacity = 1;
		br3.fill_color[0] = 0.f + (200 - player_hp) / 200; // RED
		br3.fill_color[1] = 1.f - (200 - player_hp) / 200; // GREEN
		br3.fill_color[2] = 0.f;					   // BLUE
		br3.texture = "";


		drawRect(236 - (((200 - player_hp) / 200) * 205 / 2), 75, (player_hp / 200) * 205, 27, br3);

		// Player UI
		Brush br2;
		br2.fill_opacity = 1;
		br2.outline_opacity = 0;
		br2.texture = string(PLAYER_UI);
		drawRect(180, 60, 360, 100, br2);

		// PowerUp time left

		br0.fill_color[0] = .0f;
		br0.fill_color[1] = .0f;
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH - 220, 45, 22, "PowerUp", br0);

		if (player->getUpgraded()) {
			//drawText(CANVAS_WIDTH - 260, 70, 22, "Time Left: "+to_string((int) player->getUpgradeDuration()/1000), br0);

			br2.texture = string(POWER_UP_STAR) + ".png";
			drawRect(CANVAS_WIDTH - 60, 55, 50, 50, br2);


			br2.texture = "";

			br2.fill_opacity = 1.f;
			br2.outline_opacity = 0.f;
			br2.fill_color[0] = 1.0f;
			br2.fill_color[1] = 0.84f;
			br2.fill_color[2] = .0f;
			drawRect(CANVAS_WIDTH - 211 + (((15000 - player->getUpgradeDuration()) / 15000) * 205 / 2), 75, (player->getUpgradeDuration() / 15000) * 205, 27, br2);

			br2.fill_opacity = .0f;
			br2.outline_opacity = 1;
			br2.outline_width = 4;
			br2.fill_color[0] = .0f;
			br2.fill_color[1] = .0f;
			br2.fill_color[2] = .0f;
			br2.outline_color[0] = .0f;
			br2.outline_color[1] = .0f;
			br2.outline_color[2] = .0f;
			drawRect(CANVAS_WIDTH - 210, 75, 205, 29, br2);

		}


		// Draw Power Up cage:
		br2.fill_opacity = 0.f;
		br2.outline_opacity = 1;
		br2.outline_width = 6.f;
		br2.outline_color[0] = .0f;
		br2.outline_color[1] = .0f;
		br2.outline_color[2] = .0f;
		br2.texture = "";
		drawRect(CANVAS_WIDTH - 60, 55, 68, 68, br2);


		// Draw Entities:
		for (list<Enemy*>::iterator it = enemy_list.begin(); it != enemy_list.end(); ++it) {
			(*it)->draw();
		}
		// Draw Player:
		if (player->getActiveStatus()) {
			player->draw();
		}
		// Draw PowerUp:
		if (pu && !pu->getCaptured()) {
			pu->draw();
		}
		break;
	}
	case MAIN_MENU: {

		// background
		Brush br;
		br.fill_opacity = 1;
		br.outline_opacity = 0;
		br.texture = string(MAIN_MENU_BACKGROUND);
		drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		Brush br0;
		br0.fill_opacity = 1;
		br0.outline_opacity = 1;
		br0.texture = "";
		br0.fill_color[0] = .0f;
		br0.fill_color[1] = .0f;
		br0.fill_color[2] = .0f;
		setFont(string(DRAGON_BALL_Z_FONT));
		drawText(CANVAS_WIDTH / 2 - 230, CANVAS_HEIGHT / 2 - 130, 100, "Zbit Arcade", br0);
		br0.fill_color[0] = 1.0f;
		br0.fill_color[1] = 0.84f;
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH / 2 - 240, CANVAS_HEIGHT / 2 - 120, 105, "Zbit Arcade", br0);

		if (buttonMM == PLAY) {
			br0.fill_color[0] = 1.0f;
			br0.fill_color[1] = 0.84f;
		}
		else {
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
		}
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH / 2 - 70, CANVAS_HEIGHT / 2 + 20, 50, "PLAY", br0);

		if (buttonMM == ABOUT) {
			br0.fill_color[0] = 1.0f;
			br0.fill_color[1] = 0.84f;
		}
		else {
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
		}
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH / 2 - 70, CANVAS_HEIGHT / 2 + 90, 50, "ABOUT", br0);


		if (buttonMM == EXIT) {
			br0.fill_color[0] = 1.0f;
			br0.fill_color[1] = 0.84f;
		}
		else {
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
		}
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH / 2 - 70, CANVAS_HEIGHT / 2 + 160, 50, "EXIT", br0);

		// ARROW
		Brush br1;
		br1.fill_opacity = 1;
		br1.outline_opacity = 0;
		br1.texture = string(_8BIT_ARROW);
		drawRect(CANVAS_WIDTH / 2 - 110, CANVAS_HEIGHT / 2 + 5 + arrow_offset, 50, 50, br1);

		break;
	}
	case RETRY: {

		// background
		Brush br;
		br.fill_opacity = 1;
		br.outline_opacity = 0;
		br.texture = level_asset;
		drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		// Draw Entities:
		for (list<Enemy*>::iterator it = enemy_list.begin(); it != enemy_list.end(); ++it) {
			(*it)->draw();
		}
		// Draw Player:
		if (player->getActiveStatus()) {
			player->draw();
		}
		// Draw PowerUp:
		if (pu && !pu->getCaptured()) {
			pu->draw();
		}

		Brush br0;
		br0.fill_opacity = 1;
		br0.outline_opacity = 1;
		br0.texture = "";
		br0.fill_color[0] = 1.0f;
		br0.fill_color[1] = .0f;
		br0.fill_color[2] = .0f;
		setFont(string(DRAGON_BALL_Z_FONT));
		drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 - 130, 100, "YOU LOST", br0);

		if (retry_choice == AGAIN) {
			br0.fill_color[0] = 1.0f;
			br0.fill_color[1] = 0.84f;
		}
		else {
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
		}
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH / 2 - 70, CANVAS_HEIGHT / 2 + 20, 50, "RETRY", br0);

		if (retry_choice == BACK_TO_MENU) {
			br0.fill_color[0] = 1.0f;
			br0.fill_color[1] = 0.84f;
		}
		else {
			br0.fill_color[0] = .0f;
			br0.fill_color[1] = .0f;
		}
		br0.fill_color[2] = .0f;
		drawText(CANVAS_WIDTH / 2 - 70, CANVAS_HEIGHT / 2 + 90, 50, "MAIN MENU", br0);

		// ARROW
		Brush br1;
		br1.fill_opacity = 1;
		br1.outline_opacity = 0;
		br1.texture = string(_8BIT_ARROW);
		drawRect(CANVAS_WIDTH / 2 - 110, CANVAS_HEIGHT / 2 + 5 + arrow_offset, 50, 50, br1);


		break;
	}
	case CHOOSE_LEVEL: {

		// background
		Brush br;
		br.fill_opacity = 1;
		br.outline_opacity = 0;
		br.texture = string(MAIN_MENU_BACKGROUND);
		drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		// Choose level text:
		Brush br0;
		br0.fill_opacity = 1;
		br0.outline_opacity = 1;
		br0.texture = "";
		br0.fill_color[0] = 1.0f;
		br0.fill_color[1] = 0.84f;
		br0.fill_color[2] = .0f;
		setFont(string(DRAGON_BALL_Z_FONT));
		drawText(CANVAS_WIDTH / 2 - 210, CANVAS_HEIGHT / 2 - 200, 70, "CHOOSE LEVEL:", br0);

		// Levels
		Brush br1;
		br1.fill_opacity = 1;
		br1.outline_opacity = 1;
		br1.outline_width = 4.f;
		if (level_button == BEACH) {
			br1.outline_color[0] = 1.0f;
			br1.outline_color[1] = 0.84f;
		}
		else {
			br1.outline_color[0] = .0f;
			br1.outline_color[1] = .0f;
		}
		br1.outline_color[2] = .0f;
		br1.texture = string(BACKGROUND_BEACH);
		drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 + 20 - 120, 200, 100, br1);

		if (level_button == MOUNTAIN) {
			br1.outline_color[0] = 1.0f;
			br1.outline_color[1] = 0.84f;
		}
		else {
			br1.outline_color[0] = .0f;
			br1.outline_color[1] = .0f;
		}
		br1.texture = string(BACKGROUND_MOUNTAIN);
		drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 + 20, 200, 100, br1);

		if (level_button == NIGHT) {
			br1.outline_color[0] = 1.0f;
			br1.outline_color[1] = 0.84f;
		}
		else {
			br1.outline_color[0] = .0f;
			br1.outline_color[1] = .0f;
		}
		br1.texture = string(BACKGROUND_NIGHT);
		drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 + 20 + 120, 200, 100, br1);

		// Arrow
		br1.fill_opacity = 1;
		br1.outline_opacity = 0;
		br1.texture = string(_8BIT_ARROW);
		drawRect(CANVAS_WIDTH / 2 - 150, CANVAS_HEIGHT / 2 - 100 + arrow_offset, 50, 50, br1);
		break;
	}
	case ABOUT_SCREEN: {

		// background
		Brush br;
		br.fill_opacity = 1;
		br.outline_opacity = 0;
		br.texture = string(EMPTY_MAIN_MENU_BACKGROUND);
		drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

		br.texture = string(HOW_TO);
		drawRect((CANVAS_WIDTH) / 2, (CANVAS_HEIGHT) / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);


		// How2Play text:
		Brush br0;
		br0.fill_opacity = 1;
		br0.outline_opacity = 1;
		br0.texture = "";
		br0.fill_color[0] = 1.0f;
		br0.fill_color[1] = 0.84f;
		br0.fill_color[2] = .0f;
		setFont(string(DRAGON_BALL_Z_FONT));
		drawText(CANVAS_WIDTH / 2 - 110, CANVAS_HEIGHT / 2 - 250, 50, "How to play", br0);



		// Back button
		br0.fill_color[0] = 1.0f;
		br0.fill_color[1] = 0.84f;
		br0.fill_color[2] = .0f;
		setFont(string(DRAGON_BALL_Z_FONT));
		drawText(CANVAS_WIDTH / 2 - 50, CANVAS_HEIGHT - 10, 50, "BACK", br0);


		// ARROW
		Brush br1;
		br1.fill_opacity = 1;
		br1.outline_opacity = 0;
		br1.texture = string(_8BIT_ARROW);
		drawRect(CANVAS_WIDTH / 2 - 90, CANVAS_HEIGHT - 25 + arrow_offset, 50, 50, br1);
		break;
	}

	}
}