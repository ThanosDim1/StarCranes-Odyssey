#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "CollisionObject.h"


class AnimatedObjects : public GameObject, public CollisionObject {

    std::vector<std::string> m_spritesanimatedobjects;
    float animationtimer = 0.0f;
    graphics::Brush m_brush_animatedobjects;

    public:
        void update(float dt) override;
        void draw(float add);
        void init() override;
        AnimatedObjects(std::vector<std::string> m_spritesanimatedobjects, float m_pos_x, float m_pos_y) {
            this->m_pos_x = m_pos_x;
            this->m_pos_y = m_pos_y;
            this->m_spritesanimatedobjects = m_spritesanimatedobjects;
        }

    protected:
        void hurtPlayer();
        void debugDraw();

};
