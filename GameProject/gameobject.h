#pragma once
#include <string>
#include "gamestate.h"
#include "util.h"

// Base class for all game objects.
class GameObject {
    // Static member to keep track of the next unique ID for instances of GameObject.
    static int m_next_id;

protected:
    // Pointer to the game state, allowing the game object to access global game data or state.
    class GameState* m_state;

    // Name of the game object, useful for identification or debugging.
    std::string m_name;

    // Unique identifier for the game object, assigned automatically.
    int m_id = 0;

    // Flag indicating whether the game object is active and should be updated/drawn.
    bool m_active = true;

public:
    // Constructor that optionally takes a name for the game object.
    GameObject(const std::string& name = "");

    // Virtual update method to be overridden by derived classes for implementing behavior.
    virtual void update(float dt) {}

    // Virtual init method for any initialization required by the game object.
    virtual void init() {}

    // Virtual draw method to be overridden by derived classes to handle rendering.
    virtual void draw() {}

    // Virtual destructor to ensure proper cleanup of derived classes.
    virtual ~GameObject() {}

    // Method to check if the game object is active.
    bool isActive() { return m_active; }

    // Method to set the active state of the game object.
    void setActive(bool a) { m_active = a; }
};
