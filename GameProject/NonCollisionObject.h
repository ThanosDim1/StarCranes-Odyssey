// Define a struct for non-collidable objects
struct NonCollisionObject {
    // Position of the object on the x-axis
    float m_pos_x;

    // Position of the object on the y-axis
    float m_pos_y;

    // Width of the object
    float m_width;

    // Height of the object
    float m_height;

    // Default constructor
    NonCollisionObject() {}

    // Parameterized constructor to initialize object properties
    NonCollisionObject(float x, float y, float w, float h)
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h) {}
};
