#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"

// EndScreen class inherits from GameObject to utilize game object properties and behaviors.
// It is designed to manage and display the end screen of the game.
class EndScreen : public GameObject {
    // Holds sprites or graphics for the GUI elements displayed on the end screen.
    std::vector<std::string> m_spritesgui;

public:
    // Constructor declaration.
    EndScreen();
    // Initializes end screen resources such as loading sprites.
    void init() override;
    // Displays a star rating or achievement level based on the number of stars earned, version 1.
    void StarShow1(int stars);
    // Displays a star rating or achievement level based on the number of stars earned, version 2.
    void StarShow2(int stars);
    // Draws the end screen, including stars, options, and any other GUI elements.
    void draw(int stars);
    // Updates the end screen, potentially animating stars or handling user input.
    void update(float dt) override;

    // Returns the currently selected option on the end screen.
    int option_selected() {
        return m_selected;
    }
    // Returns whether an option is locked or unavailable.
    int option_locked() {
        // Check if the instance exists before accessing member.
        if (this) {
            return m_option_locked;
        }
    }
    // Locks or unlocks an option based on the game's state or player's progress.
    int set_option_locked(int option) {
        this->m_option_locked = option;
    }

private:
    // Brushes for drawing various GUI elements on the end screen.
    graphics::Brush brush_gui;
    graphics::Brush brush_menu;
    graphics::Brush brush_next;
    graphics::Brush brush_text;

    // Dimensions and position of the end screen.
    int m_width;
    int m_height;
    int m_pos_x;
    int m_pos_y;

    // Tracks the currently selected menu option.
    int m_selected = 1;
    // Indicates if an option is locked.
    int m_option_locked;
};
