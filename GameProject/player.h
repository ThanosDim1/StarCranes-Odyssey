#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "CollisionObject.h"
#include <memory>
#include "enemy.h"

// Player class derived from CollisionObject and GameObject
class Player : public CollisionObject, public GameObject {
    // Sprite vectors for various player animations
    std::vector<std::string> m_spritesright;
    std::vector<std::string> m_spritesleft;
    std::vector<std::string> m_spritesidle;
    std::vector<std::string> m_spritesjumpright;
    std::vector<std::string> m_spritesjumpleft;
    std::vector<std::string> m_spritesdeactivation;
    std::vector<std::string> m_spritesattackwithkniferight;
    std::vector<std::string> m_spritesattackwithknifeleft;

    // Mouse state for player input
    graphics::MouseState mouse;

    // Brush for player rendering
    graphics::Brush m_brush_player;

    // Constants for player motion and animation
    const float m_accel_horizontal = 20.0f;
    const float m_accel_vertical = 300.1f;
    const float m_max_velocity = 5.0f;
    const float m_gravity = 12.0f;

    // Animation timers and control variables
    float animationtimerfordeath = 0.0f;
    float animationtimerforafk = 0.0f;
    float animationtimerforattackwithknife = 0.0f;
    bool isAnimationPlaying = false;

public:
    // Velocity components for player motion
    float m_vx = 5.0f;
    float m_vy = 0.0f;

    // Player health and game over state
    int m_player_health = 5;
    bool m_gameover = false;

    // Offset for vertical motion during jumping
    float offsetmvy;

    // Collision flags for different directions
    bool isCollidingSideways = false;
    bool isCollidingDown = false;
    bool isCollidingUp = false;

    // Control variables for animations and game progress
    bool playanimation = false;
    bool m_player_has_key = false;
    bool dooropen = false;
    int m_player_has_star = 0;
    float timer = 0.0f;

public:
    // Override the update function from the base class
    void update(float dt) override;

    // Override the draw function from the base class
    void draw() override;

    // Override the initialization function from the base class
    void init() override;

    // Constructor for the Player class, initializing base class with a name
    Player(std::string name) : GameObject(name) {}

    // Function to handle door opening logic
    bool openDoor();

protected:
    // Function for debugging purposes to draw outlines
    void debugDraw();

    // Function for controlling player motion
    void movePlayer(float dt);

    // Function for handling player health and damage
    void hurtPlayer(Enemy* enemy);
};
