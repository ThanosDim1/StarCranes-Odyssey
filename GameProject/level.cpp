#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "util.h"


void Level::drawBlock(int i)
{
	Box& box = m_blocks[i];
	std::string& name = m_block_names[i];

	float x = box.m_pos_x + m_state->m_global_offset_x;
	float y = box.m_pos_y + m_state->m_global_offset_y;

	m_block_brush.texture = m_state->getFullAssetPath(name);

	graphics::drawRect(x,y, 2.0f * m_block_size, 2.0f * m_block_size, m_block_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

}

void Level::update(float dt)
{
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);


	GameObject::update(dt);
}

void Level::draw()
{
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x  + w / 2.0f;
	float offset_y = m_state->m_global_offset_y  + h / 2.0f;

	//draw background
	graphics::drawRect(2.8f * offset_x, offset_y, 4.0f * w, h, m_brush_background);

	// draw blocks
	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

}

Level::Level(const std::string& name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background6.png");

}

Level::~Level()
{
	for (auto p_go : m_static_objects)
		delete p_go;
	for (auto p_go : m_dynamic_objects)
		delete p_go;
}

void Level::checkCollisions()
{

	for (auto& box : m_blocks)
	{
		if (m_state->getPlayer()->intersect(box))
			printf("*");
	}


	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(block))
		{
			m_state->getPlayer()->m_pos_y += offset;

			m_state->getPlayer()->m_vy = 0.0f;

			break;
		}
	}

	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(block))
		{
			m_state->getPlayer()->m_pos_x += offset;

			m_state->getPlayer()->m_vx = 0.0f;
			break;
		}
	}
}

void Level::init()
{

	for (auto p_gob : m_static_objects)
		if (p_gob) p_gob->init();

	for (auto p_gob : m_dynamic_objects)
		if (p_gob) p_gob->init();

	// initialize some collidable blocks here.
	// normally, you should build a Block class 
	// and automate the process more.

	m_blocks.push_back(Box(1, 1, 1, 1));
	m_blocks.push_back(Box(1, 2, 1, 1));
	m_blocks.push_back(Box(1, 3, 1, 1));
	m_blocks.push_back(Box(1, 4, 1, 1));
	m_blocks.push_back(Box(1, 5, 1, 1));
	m_blocks.push_back(Box(1, 6, 1, 1));
	m_blocks.push_back(Box(1, 7, 1, 1));
	m_blocks.push_back(Box(1, 8, 1, 1));

	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");
	m_block_names.push_back("tile_13.png");

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
}
