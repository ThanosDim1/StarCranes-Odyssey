#include "AnimatedObjects.h"
#include <sgg/graphics.h>
#include "Spikes.h"


void Spikes::update(float dt)
{
	AnimatedObjects::update(dt);
}

void Spikes::draw(float add)
{
	AnimatedObjects::draw(add, m_sprites_spikes);

	if (m_state->m_debugging)
		debugDraw();
}

void Spikes::init()
{
	
	m_sprites_spikes.push_back(m_state->getFullAssetPath("Spikes.png"));
	AnimatedObjects::init();

}
void Spikes::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 0, 0, 1);
	SETCOLOR(debug_brush.outline_color, 0, 0, 1);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;
	graphics::drawRect(m_pos_x + offset_x, m_pos_y + offset_y, m_width, m_height, debug_brush);
}