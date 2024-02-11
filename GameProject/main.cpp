#include <sgg/graphics.h>
#include "gamestate.h"

// Function to handle drawing operations
void draw()
{
    // Delegate drawing to the GameState instance
    GameState::getInstance()->draw();
}

// Function to handle update operations with a time step (dt)
void update(float dt)
{
    // Delegate updating to the GameState instance
    GameState::getInstance()->update(dt);
}

// Main function
int main(int argc, char** argv)
{
    // Create a window with specified dimensions and title
    graphics::createWindow(1000, 700, "Maou");

    // Initialize the GameState singleton instance
    GameState::getInstance()->init();

    // Set drawing and updating functions for the graphics library
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    // Set canvas size, scale mode, and initialize the graphics library message loop
    graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), GameState::getInstance()->getCanvasHeight());
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);
    graphics::startMessageLoop();

    return 0;
}
