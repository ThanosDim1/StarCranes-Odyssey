#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"
#include "Star.h"


class Menu : public GameObject
{
    graphics::Brush brush_background_menu;
    graphics::Brush brush_menu;
    graphics::Brush brush_play;
    graphics::Brush brush_about;
    graphics::Brush brush_exit;
    graphics::Brush brush_instructions;
    std::vector<std::string> m_spritesmoney;
    std::vector<std::string> m_spritesstar;

public:
    Menu();
    void init() override;
    void draw() override;
    void update(float dt) override;
    void drawAnimationMoney(float x, float y);
    void drawAnimationStar(float x, float y);

    int option_selected() {
        return m_selected;
    }
    int option_locked() {
        if (this) {
            return m_option_locked;
        }
    }

    void set_option_locked(int option) {
        this->m_option_locked = option;
    }
private:
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
