#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"

// Class AboutPage inherits from GameObject and is responsible for displaying the about page in the game.
// This page might contain information about the game's story, controls, credits, etc.
class AboutPage : public GameObject {
public:
    // Constructor
    AboutPage();
    // Destructor
    ~AboutPage();

    // Initializes the about page, setting up necessary resources like graphics and text.
    void init();
    // Draws the about page content to the screen.
    void draw();
    // Updates the about page, such as responding to user input or animations. 'dt' represents delta time between frames.
    void update(float dt);

private:
    // Brush for drawing the background of the about page.
    graphics::Brush brush_about_background;
    // Brush for drawing the primary text on the about page.
    graphics::Brush brush_about_text;
    // Brush for drawing the secondary text on the about page.
    graphics::Brush brush_about_text2;
    // Strings to hold various texts for the about page. These could include game information, instructions, etc.
    std::string about_text1;
    std::string about_text2;
    std::string about_text3;
    std::string about_text4;

    // Brush for drawing control instructions, e.g., WASD keys for movement.
    graphics::Brush brush_wasd;
    // Brush for drawing mouse control instructions, e.g., using the left mouse click for actions.
    graphics::Brush brush_left_click;

    // Dimensions and position of the about page on the screen.
    int m_width;  // Width of the about page.
    int m_height; // Height of the about page.
    int m_pos_x;  // X position of the about page.
    int m_pos_y;  // Y position of the about page.
};
