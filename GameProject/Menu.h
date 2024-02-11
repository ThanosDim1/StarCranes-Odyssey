// Header guard to ensure the file is included only once
#pragma once

// Include necessary headers
#include "gameobject.h"
#include "sgg/graphics.h"
#include "Star.h"

// Menu class derived from GameObject
class Menu : public GameObject {
    // Brushes for menu background and options
    graphics::Brush brush_background_menu;
    graphics::Brush brush_menu;
    graphics::Brush brush_play;
    graphics::Brush brush_about;
    graphics::Brush brush_exit;
    graphics::Brush brush_instructions;

    // Sprite vectors for animated money and star
    std::vector<std::string> m_spritesmoney;
    std::vector<std::string> m_spritesstar;

public:
    // Constructor
    Menu();

    // Initialization method
    void init() override;

    // Draw method to render the menu
    void draw() override;

    // Update method for menu logic
    void update(float dt) override;

    // Method to draw animated money at specified position
    void drawAnimationMoney(float x, float y);

    // Method to draw animated star at specified position
    void drawAnimationStar(float x, float y);

    // Getter for the currently selected menu option
    int option_selected() {
        return m_selected;
    }

    // Getter for the locked menu option
    int option_locked() {
        if (this) {
            return m_option_locked;
        }
    }

    // Setter for the locked menu option
    void set_option_locked(int option) {
        this->m_option_locked = option;
    }

private:
    // Private members for menu dimensions, position, and animation frames
    int m_width;
    int m_height;
    int m_pos_x;
    int m_pos_y;
    int m_selected = 1;
    int m_option_locked;
    int m_frameDelay = 0;
    int m_currentFrameMoney = 0;
    int m_currentFrameStar = 0;
};
