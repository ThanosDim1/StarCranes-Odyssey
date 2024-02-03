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
private:
    int m_width;
    int m_height;
    int m_pos_x;
    int m_pos_y;
    int m_selected;
};
