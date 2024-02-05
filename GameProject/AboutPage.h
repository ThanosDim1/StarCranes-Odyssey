#pragma once
#include "gameobject.h"
#include "sgg/graphics.h"

class AboutPage : public GameObject {


public:
    AboutPage();
    ~AboutPage();
    void init();
    void draw();
    void update(float dt);

private:
    graphics::Brush brush_about_background;
    graphics::Brush brush_about_text;
    graphics::Brush brush_about_text2;
    std::string about_text1;
    std::string about_text2;
    std::string about_text3;
    std::string about_text4;

    graphics::Brush brush_wasd;
    graphics::Brush brush_left_click;


    int m_width;
    int m_height;
    int m_pos_x;
    int m_pos_y;
};
