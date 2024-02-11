#pragma once

#include "gameobject.h"
#include <sgg/graphics.h>
#include "CollisionObject.h"

// Enemy class inherits from CollisionObject for collision detection and from GameObject for game-related properties and methods.
class Enemy : public CollisionObject, public GameObject {
    // Vectors holding the paths to sprite images for different enemy states and actions.
    std::vector<std::string> m_spritesenemy1;
    std::vector<std::string> m_spritesenemy2;
    std::vector<std::string> m_spritesdeactivation;
    std::vector<std::string> m_spritesleftattack;
    std::vector<std::string> m_spritesrightattack;
    std::vector<std::string> m_spritesidle;

    // Variables for managing animations and sprite selection.
    int m_currentDeactivationSprite = 0;
    float animationtimerfordeath = 0.0f;
    float animationtimer = 0.0f;
    float animationtimeridle = 0.0f;

    // Graphics and physics properties.
    graphics::MouseState mouse;
    graphics::Brush m_brush_enemy;
    const float m_accel_horizontal = 20.0f;
    const float m_accel_vertical = 300.1f;
    const float m_max_velocity = 2.0f;
    const float m_gravity = 12.0f;

    // Enemy status variables.
    int m_enemy_health = 1;
    bool m_enemygameover = false;
    bool m_enemyrunleft = false;
    bool m_enemyrunright = false;

public:
    // Velocity and state flags.
    float m_vx = 2.0f; // Horizontal velocity.
    float m_vy = 0.0f; // Vertical velocity.
    bool m_isDeactivating = false; // Is the enemy in the process of being deactivated?
    bool isCollidingSidewaysEnemy = false; // Collision flag for horizontal interactions.
    bool isCollidingDownEnemy = false; // Collision flag for ground interactions.
    bool isCollidingPlayerEnemy = false; // Collision flag for interactions with the player.

public:
    // Constructor initializes the enemy with a name and position.
    Enemy(std::string name, float m_pos_x, float m_pos_y) : GameObject(name) {
        this->m_pos_x = m_pos_x;
        this->m_pos_y = m_pos_y;
    }

    // Override methods from GameObject to provide specific functionality for the Enemy class.
    void update(float dt) override; // Update the enemy's state.
    void draw() override; // Draw the enemy on the screen.
    void init() override; // Initialize enemy resources.

protected:
    // Debugging, damage, and sight detection methods.
    void debugDraw(); // Optional method for debugging purposes.
    void hurtEnemy(); // Applies damage to the enemy.
    bool enemysightleft(float player_x, float player_y, float enemy_x, float enemy_y); // Detects the player to the left.
    bool enemysightright(float player_x, float player_y, float enemy_x, float enemy_y); // Detects the player to the right.
    //void moveEnemy(float dt); // Method to handle enemy movement (commented out, possibly for future use or refinement).
};
