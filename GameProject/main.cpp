#include "sgg/graphics.h"
#include "gamestate.h"

void draw()
{
    GameState::getInstance()->draw();
}

void update(float dt)
{
    GameState::getInstance()->update(dt);
}

int main(int argc, char** argv) {

    graphics::createWindow(850, 500, "Maou");

    GameState::getInstance()->init();

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(GameState::getInstance()->getCanvasWidth(), GameState::getInstance()->getCanvasHeight());
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    graphics::startMessageLoop();
    return 0;
}
sdasdasdddddd


