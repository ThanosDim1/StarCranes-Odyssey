#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "enemy.h"
#include "util.h"
#include <iostream>



void Level::drawBlock(int i)
{
	CollisionObject& CollisionObject = m_blocks[i];
	std::string& name = m_block_names[i];

	float x = CollisionObject.m_pos_x + m_state->m_global_offset_x;
	float y = CollisionObject.m_pos_y + m_state->m_global_offset_y;

	m_block_brush.texture = m_state->getFullAssetPath(name);

	graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);

}


void Level::update(float dt)

{
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isActive()) {
			enemies[i]->update(dt);
		}
	}


	checkCollisionsForEnemy();
	checkCollisions();
	checkCollisionsMovingObjects();
	GameObject::update(dt);
}

void Level::draw()
{
	animationtimerforhealthsystem += 0.05f;

	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;

	//draw background
	graphics::drawRect(offset_x, offset_y, 6.0f * w, 3.0f * h, m_brush_background);


	// draw blocks
	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isActive())
			enemies[i]->draw();
	}

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	//draw health system
	if (m_state->getPlayer()->m_player_health == 5) {
		int spriteshealthsystemfull = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemfull.size());
		m_brush_health_system.texture = m_spriteshealthsystemfull[spriteshealthsystemfull];
	}
	else if (m_state->getPlayer()->m_player_health == 4) {
		int spriteshealthsystemalmostfull = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemalmosthalf.size());
		m_brush_health_system.texture = m_spriteshealthsystemalmosthalf[spriteshealthsystemalmostfull];
	}
	else if (m_state->getPlayer()->m_player_health == 3) {
		int spriteshealthsystemhalf = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemhalf.size());
		m_brush_health_system.texture = m_spriteshealthsystemhalf[spriteshealthsystemhalf];
	}
	else if (m_state->getPlayer()->m_player_health == 2 || m_state->getPlayer()->m_player_health == 1) {
		int spriteshealthsystemlastlife = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemlastlife.size());
		m_brush_health_system.texture = m_spriteshealthsystemlastlife[spriteshealthsystemlastlife];
	}
	else {
		int spriteshealthsystemdeath = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemdeath.size());
		m_brush_health_system.texture = m_spriteshealthsystemdeath[spriteshealthsystemdeath];
	}

	graphics::drawRect(1.3, 1.1, 1.8, 0.5, m_brush_health_system);
}

Level::Level(const std::string& name)
	: GameObject(name)
{
	m_brush_background.outline_opacity = 0.0f;
	m_brush_background.texture = m_state->getFullAssetPath("background7.png");

	m_brush_health_system.outline_opacity = 0.0f;

}

Level::~Level()
{

}

void Level::checkCollisions()
{

	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectSideways(block))
		{
			m_state->getPlayer()->isCollidingSideways = true;
			m_state->getPlayer()->m_pos_x += offset;
			m_state->getPlayer()->m_vx = 0.0f;


			break;
		}

	}

	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		if (offset = m_state->getPlayer()->intersectDown(block))
		{
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("land.wav"), 0.2f);

			m_state->getPlayer()->isCollidingDown = true;
			m_state->getPlayer()->m_vy = 0.0f;

			if ((m_state->getPlayer()->m_vy == 0 || !m_state->getPlayer()->isCollidingSideways) && m_state->getPlayer()->isCollidingDown)
			{
				if (m_state->getPlayer()->isCollidingSideways)
				{
					m_state->getPlayer()->m_vx = 0.0f;
					m_state->getPlayer()->m_vy = 0.0f;
					m_state->getPlayer()->isCollidingSideways = false;

				}

				m_state->getPlayer()->m_pos_y += offset;

				break;
			}
		}
		else
		{
			m_state->getPlayer()->isCollidingDown = false;
		}
	}

	for (auto& block : m_blocks)
	{

		if (m_state->getPlayer()->intersectUp(block))
		{
			m_state->getPlayer()->offsetmvy = m_state->getPlayer()->intersectUp(block);
		}
	}
}

void Level::checkCollisionsForEnemy()
{
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		for (int i = 0; i < enemies.size(); i++)
		{
			if (offset = enemies[i]->intersectSideways(block))
			{
				std::cout << "enemy is colliding sideways" << std::endl;
				enemies[i]->isCollidingSidewaysEnemy = true;
				enemies[i]->m_pos_x += offset;
				enemies[i]->m_vx = 0.0f;

				// Deactivate the enemy
				enemies[i]->m_isDeactivating = true;
				break;
			}

		}

	}

	for (auto& block : m_blocks)
	{
		for (int i = 0; i < enemies.size(); i++) {
			float offset = 0.0f;
			if (offset = enemies[i]->intersectDown(block))
			{
				enemies[i]->isCollidingDownEnemy = true;
				enemies[i]->m_vy = 0.0f;

				if ((enemies[i]->m_vy == 0 || !enemies[i]->isCollidingSidewaysEnemy) && enemies[i]->isCollidingDownEnemy)
				{
					if (enemies[i]->isCollidingSidewaysEnemy)
					{
						enemies[i]->m_vx = 0.0f;
						enemies[i]->m_vy = 0.0f;
						enemies[i]->isCollidingSidewaysEnemy = false;

					}

					enemies[i]->m_pos_y += offset;

				}
			}
			else
			{
				enemies[i]->isCollidingDownEnemy = false;
			}
		}
	}
}

void Level::checkCollisionsMovingObjects() {

	for (int i = 0; i < enemies.size(); i++) {
		if (enemies[i] != nullptr) {
			CollisionObject& enemy = *enemies[i];
			if (m_state->getPlayer()->intersectSideways(enemy)) {

				float delta_time = graphics::getDeltaTime() / 1000.0f;
				enemies[i]->m_vx = 0.0f;
				// Set isCollidingPlayerEnemy to true
				enemies[i]->isCollidingPlayerEnemy = true;
				// Apply deceleration force to player's velocity
				if (graphics::getKeyState(graphics::SCANCODE_A)) {
					m_state->getPlayer()->m_vx = 3.0f;
				}
				else if (graphics::getKeyState(graphics::SCANCODE_D)) {
					m_state->getPlayer()->m_vx = 2.0f;
				}
				break;
			}
			else {
				m_state->getPlayer()->m_vx = 5.0f;
				enemies[i]->m_vx = 2.0f;
				enemies[i]->isCollidingSidewaysEnemy = false;
				// Set isCollidingPlayerEnemy to false
				enemies[i]->isCollidingPlayerEnemy = false;
			}
		}
	}
}

void Level::init()
{
	char lvl1[14][74]{

	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '2', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '4', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '3', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'},
	{ '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'}

	};


	for (int x = 0; x < 14; x++) {
		for (int y = 0; y < 74; y++) {
			if (lvl1[x][y] != '0') {
				m_blocks.push_back(CollisionObject(y - 19, x - 2, 1, 1));
				m_block_names.push_back("Tile_" + std::string(1, lvl1[x][y]) + ".png");


			}
		}
	}

	for (int i = 0; i <= 2; i++) {
		enemies.push_back(std::make_unique<Enemy>("Enemy" + std::to_string(i), 5.0f * (i + 1), 5.0f));
		enemies[i]->init();
	}

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full1.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full2.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full3.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full4.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full5.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full6.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full7.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full8.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full9.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full10.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full11.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full12.png"));
	m_spriteshealthsystemfull.push_back(m_state->getFullAssetPath("full13.png"));

	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf1.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf2.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf3.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf4.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf5.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf6.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf7.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf8.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf9.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf10.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf11.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf12.png"));
	m_spriteshealthsystemalmosthalf.push_back(m_state->getFullAssetPath("almosthalf13.png"));

	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half1.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half2.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half3.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half4.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half5.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half6.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half7.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half8.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half9.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half10.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half11.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half12.png"));
	m_spriteshealthsystemhalf.push_back(m_state->getFullAssetPath("half13.png"));

	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife1.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife2.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife3.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife4.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife5.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife6.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife7.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife8.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife9.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife10.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife11.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife12.png"));
	m_spriteshealthsystemlastlife.push_back(m_state->getFullAssetPath("lastlife13.png"));

	m_spriteshealthsystemdeath.push_back(m_state->getFullAssetPath("death1.png"));
	m_spriteshealthsystemdeath.push_back(m_state->getFullAssetPath("death2.png"));

}

