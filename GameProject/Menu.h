#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"


class Menu : public GameObject 
{
    graphics::Brush brush_background_menu;
    graphics::Brush brush_menu;
    graphics::Brush brush_play;
    graphics::Brush brush_about;
    graphics::Brush brush_exit;

public:
    Menu();
    void init() override;
    void draw() override;
    void update(float dt) override;


    int option_selected() {
        return m_selected;
    }
    int option_locked() {
        if (this) {
            return m_option_locked;
        }
    }
    int set_option_locked(int option) {
        this->m_option_locked = option;
    }
private:
    int m_width;
    int m_height;
    int m_pos_x;
    int m_pos_y;
    int m_selected = 1;
    int m_option_locked;
};
