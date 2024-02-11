#include "EndScreen.h"
#include "util.h"
#include "GameState.h"
#include <iostream>
#include <chrono>
#include "player.h"
#include "level.h"
#include "gamestate.h"

// Constructor initializes the end screen with default values and setups based on the game state.
EndScreen::EndScreen()
{
    // Activates the GameObject functionality for the end screen.
    this->GameObject::setActive(true);
    // Set dimensions and position to center the end screen based on the game canvas size.
    m_width = GameState::getInstance()->getCanvasWidth();
    m_height = GameState::getInstance()->getCanvasHeight();
    m_pos_x = GameState::getInstance()->getCanvasWidth() / 2.0f;
    m_pos_y = GameState::getInstance()->getCanvasHeight() / 2.0f;
}

// Initializes the end screen with specific textures and setups for UI elements.
void EndScreen::init() {
    // Configure textures for menu buttons and set their outlines to be invisible.
    brush_menu.texture = m_state->getFullAssetPath("Menu_BTNinactive.png");
    brush_menu.outline_opacity = 0.0f;

    brush_next.texture = m_state->getFullAssetPath("Play_BTNinactive.png");
    brush_next.outline_opacity = 0.0f;

    brush_text.outline_opacity = 0.0f;

    // Setup sprite names for GUI elements representing stars collected at different levels.
    m_spritesgui = { "level1stars0.png", "level1stars1.png", "level1stars2.png", "level1stars3.png",
                    "level2stars0.png", "level2stars1.png", "level2stars2.png", "level2stars3.png" };
}

// Shows the number of stars collected for Level 1.
void EndScreen::StarShow1(int stars) {
    graphics::Brush br;
    br.texture = m_state->getFullAssetPath("level1stars" + std::to_string(stars) + ".png");
    br.outline_opacity = 0.0f; // No outline for simplicity.
    graphics::drawRect(m_pos_x, m_pos_y / 1.5f, 7.0f, 7.0f, br); // Draw the star status with adjusted size and position.
}

// Shows the number of stars collected for Level 2 in a similar fashion.
void EndScreen::StarShow2(int stars) {
    graphics::Brush br;
    br.texture = m_state->getFullAssetPath("level2stars" + std::to_string(stars) + ".png");
    br.outline_opacity = 0.0f; // No outline for simplicity.
    graphics::drawRect(m_pos_x, m_pos_y / 1.5f, 7.0f, 7.0f, br); // Draw the star status with adjusted size and position.
}

// Main drawing function for the end screen, displaying the results and options for the player.
void EndScreen::draw(int stars) {
    // Only proceed if a level is finished and the end screen should be displayed.
    if (m_state->getLevel() && (m_state->getLevel()->lvl1_finished || m_state->getLevel()->lvl2_finished)) {
        // Display stars for level 1 or level 2 based on the game state.
        if (m_state->getLevel()->lvl1_finished && !m_state->init_lvl2) {
            StarShow1(stars);
        }
        if (m_state->init_lvl2) {
            StarShow2(stars);
        }

        // Draw buttons for menu and next options.
        graphics::drawRect(6.5f, 8.0f, 1.0f, 1.0f, brush_menu);
        graphics::drawRect(9.5f, 8.0f, 1.0f, 1.0f, brush_next);

        // Highlight the currently selected button to indicate focus.
        // "SETCOLOR" macro or function assumed to set fill color of the brushes.
        if (m_selected == 1) {
            SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 0.f); // Highlight menu button.
            SETCOLOR(brush_next.fill_color, 1.f, 1.f, 1.f); // Normal next button.
        }
        else {
            SETCOLOR(brush_menu.fill_color, 1.f, 1.f, 1.f); // Normal menu button.
            SETCOLOR(brush_next.fill_color, 1.f, 1.f, 0.f); // Highlight next button.
        }

        // Display text to instruct the player on how to select an option.
        graphics::setFont(m_state->getFullAssetPath("ThaleahFat.ttf"));
        graphics::drawText(6.5f, 9.9f, 0.5f, "Press F to select", brush_text);
    }
}

// Updates the end screen, allowing the player to select an option.
void EndScreen::update(float dt) {
    if (m_state->getLevel() && (m_state->getLevel()->lvl1_finished || m_state->getLevel()->lvl2_finished)) {
        // Implement a delay mechanism to prevent rapid selection changes.
        static std::chrono::steady_clock::time_point lastKeyPressTime = std::chrono::steady_clock::now();
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastKeyPressTime).count();

        if (elapsedTime < 200) { // Short delay to manage input responsiveness.
            return;
        }

        // Navigate through options with left and right arrow keys.
        if (graphics::getKeyState(graphics::SCANCODE_LEFT)) {
            m_selected = (m_selected != 1) ? m_selected - 1 : 2;
            graphics::playSound(m_state->getFullAssetPath("option.wav"), 0.5f); // Sound feedback for option change.
            lastKeyPressTime = currentTime; // Reset the timer for key press delay.
        }
        if (graphics::getKeyState(graphics::SCANCODE_RIGHT)) {
            m_selected = (m_selected != 2) ? m_selected + 1 : 1;
            graphics::playSound(m_state->getFullAssetPath("option.wav"), 0.5f); // Sound feedback for option change.
            lastKeyPressTime = currentTime; // Reset the timer for key press delay.
        }

        // Lock in the selected option when the F key is pressed.
        if (graphics::getKeyState(graphics::SCANCODE_F)) {
            m_option_locked = m_selected; // Lock the currently selected option.
            graphics::playSound(m_state->getFullAssetPath("optionselected.wav"), 0.5f); // Sound feedback for selection.
        }
    }
}

