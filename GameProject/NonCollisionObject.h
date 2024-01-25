struct NonCollisionObject
{
    float m_pos_x;
    float m_pos_y;
    float m_width;
    float m_height;

    NonCollisionObject() {}

    NonCollisionObject(float x, float y, float w, float h)
        : m_pos_x(x), m_pos_y(y), m_width(w), m_height(h) {}
};
