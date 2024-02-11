#include "gamestate.h"
#include "level.h"
#include "player.h"
#include "enemy.h"
#include "util.h"
#include "menu.h"
#include "aboutpage.h"
#include "endscreen.h"
#include <thread>
#include <chrono>
#include <iostream>

// Constructor for GameState class
GameState::GameState()
{
}

// Destructor for GameState class
GameState::~GameState()
{
    // Cleanup current level if exists
    if (m_current_level)
        delete m_current_level;
}

// Singleton pattern: Get instance of GameState
GameState* GameState::getInstance()
{
    if (!m_unique_instance)
    {
        m_unique_instance = new GameState();
    }
    return m_unique_instance;
}

// Initialize GameState
bool GameState::init()
{
    // Initialize menu
    m_menu = new Menu();
    m_menu->init();

    // Preload bitmaps
    graphics::preloadBitmaps(getAssetDir());

    return true;
}

// Draw function for GameState
void GameState::draw()
{
    // Draw menu if exists
    if (m_menu) {
        m_menu->draw();
    }

    // Draw about page if exists
    if (m_about_page) {
        m_about_page->draw();
    }

    // Draw current level if exists
    if (m_current_level) {
        m_current_level->draw();
    }

    // Draw end screen if exists
    if (m_endscreen) {
        m_endscreen->draw(temp_stars);
    }
}

// Update function for GameState
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

    // Update end screen if exists
    if (m_endscreen) {
        m_endscreen->update(dt);

        // Handle end screen options after level completion
        if (m_current_level->lvl1_finished) {
            // Handle options based on user choice
            switch (m_endscreen->option_locked())
            {
            case 1:
                // Go back to menu
                delete m_endscreen;
                m_endscreen = nullptr;
                delete m_current_level;
                endscreen_init = false;
                m_current_level = nullptr;
                delete m_player;
                m_player = nullptr;
                m_menu = new Menu();
                m_menu->init();
                break;
            case 2:
                // Proceed to level 2
                delete m_endscreen;
                m_endscreen = nullptr;
                endscreen_init = false;
                delete m_current_level;
                init_lvl2 = true;
                m_current_level = new Level("2.lvl");
                m_current_level->lvl1_finished = true;
                m_current_level->init();
                delete m_player;
                m_player = new Player("Player");
                m_player->init();
                break;
            }
        }
        else if (m_current_level->lvl2_finished) {
            // Handle options based on user choice
            switch (m_endscreen->option_locked())
            {
            case 1:
                // Go back to menu
                delete m_endscreen;
                m_endscreen = nullptr;
                delete m_current_level;
                endscreen_init = false;
                m_current_level = nullptr;
                delete m_player;
                m_player = nullptr;
                init_lvl2 = false;
                m_menu = new Menu();
                m_menu->init();
                break;
            case 2:
                // Go back to menu
                delete m_endscreen;
                m_endscreen = nullptr;
                delete m_current_level;
                endscreen_init = false;
                m_current_level = nullptr;
                delete m_player;
                m_player = nullptr;
                init_lvl2 = false;
                m_menu = new Menu();
                m_menu->init();
                break;
            }
        }
    }

    // Update current level if exists
    if (m_current_level) {
        if (m_current_level->lvl1_finished && init_lvl2 == false) {
            if (!endscreen_init) {
                m_current_level->text_showed = true;
                m_endscreen = new EndScreen();
                m_endscreen->init();
                temp_stars = m_player->m_player_has_star;
                endscreen_init = true;
                m_player->setActive(false);
            }
        }
        else if (m_current_level->lvl2_finished) {
            if (!endscreen_init) {
                m_current_level->text_showed = true;
                m_endscreen = new EndScreen();
                m_endscreen->init();
                temp_stars = m_player->m_player_has_star;
                endscreen_init = true;
                m_player->setActive(false);
            }
        }

        // Update current level
        m_current_level->update(dt);
    }

    // Update menu if exists
    if (m_menu) {
        m_menu->update(dt);

        // Handle menu options
        switch (m_menu->option_locked())
        {
        case 1:
            // Start the game (initialize level 1)
            delete m_menu;
            m_menu = nullptr;

            m_current_level = new Level("1.lvl");
            m_current_level->init();

            m_player = new Player("Player");
            m_player->init();
            break;
        case 2:
            // Show about page
            delete m_menu;
            m_menu = nullptr;

            m_about_page = new AboutPage();
            m_about_page->init();
            break;
        case 3:
            // Exit the game
            delete m_menu;
            m_menu = nullptr;
            graphics::stopMessageLoop();
            break;
        default:
            break;
        }
    }

    // Update about page if exists
    if (m_about_page) {
        m_about_page->update(dt);
    }

    // Check if debugging is enabled
    m_debugging = graphics::getKeyState(graphics::SCANCODE_0);

    // Handle player death scenario
    if (m_dead && deathtimer < 2300) {
        // Play death sound
        graphics::playSound(getFullAssetPath("kyriakos.wav"), 0.07f);

        // Cleanup current level and player
        delete m_current_level;
        m_current_level = nullptr;
        m_player = nullptr;

        // Draw death screen
        float m_pos_x = m_canvas_width / 2.0f;
        float m_pos_y = m_canvas_height / 2.0f;

        // Set background color to black
        SETCOLOR(brush_background_dead.fill_color, 0.0f, 0.0f, 0.0f);
        graphics::drawRect(m_pos_x, m_pos_y, m_canvas_width, m_canvas_height, brush_background_dead);

        // Draw death message
        graphics::setFont(getFullAssetPath("Aerologica.ttf"));
        SETCOLOR(brush_dead.fill_color, 1.f, 0, 0);
        graphics::drawText(4.0f, 5.5f, 2, "YOU DIED", brush_dead);

        // Increment death timer
        deathtimer += dt;
    }
    else {
        // Reset game state after death
        if (m_dead) {
            m_menu = new Menu();
            m_menu->init();
        }
        m_dead = false;
        deathtimer = 0.0f;
    }
}

// Get full asset path with asset name
std::string GameState::getFullAssetPath(const std::string& asset)
{
    return m_asset_path + asset;
}

// Get asset directory
std::string GameState::getAssetDir()
{
    return m_asset_path;
}

// Switch to the main menu
void GameState::switchToMenu()
{
    // Cleanup about page if exists
    if (m_about_page) {
        delete m_about_page;
        m_about_page = nullptr;
    }

    // Initialize and switch to the main menu
    m_menu = new Menu();
    m_menu->init();
}

// Initialize static variable for the singleton pattern
GameState* GameState::m_unique_instance = nullptr;
