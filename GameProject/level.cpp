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

void Level::drawNonCollisionBlock(int i)
{
	NonCollisionObject& NonCollisionObject = m_non_collidable_blocks[i];

	std::string& name = m_non_collidable_block_names[i];

	float x = NonCollisionObject.m_pos_x + m_state->m_global_offset_x;
	float y = NonCollisionObject.m_pos_y + m_state->m_global_offset_y;

	m_block_brush.texture = m_state->getFullAssetPath(name);

	graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush);

	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);
}



void Level::update(float dt)
{

	for (int i = 0; i < saws.size(); i++)
	{
		if (saws[i]->isActive()) {
			saws[i]->update(dt);
		}
	}
	
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt);

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isActive()) {
			enemies[i]->update(dt);
		}
	}

	checkCollisionPlayerKey();
	checkCollisionPlayerDoor();
	checkCollisionsForEnemy();
	checkCollisions();
	checkCollisionPlayerSaw();
	checkCollisionsMovingObjects();
	GameObject::update(dt);
}

void Level::draw()
{
	

	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;

	//draw background
	graphics::drawRect(offset_x, offset_y+2.0f, 6.0f * w, 3.0f * h, m_brush_background);

	// draw static blocks
	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i);
	}

	// draw non collidable blocks
	for (int i = 0; i < m_non_collidable_blocks.size(); i++)
	{
		drawNonCollisionBlock(i);
	}

	for (int i = 0; i < saws.size(); i++)
	{
		if (saws[i]->isActive()) {
			saws[i]->draw(0.5f);
		}
	}

	mn_leveldoor1->draw(0.5f);

	mn_keylevel->draw(0.5f);

	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isActive())
			enemies[i]->draw();
	}

	// draw player
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	//draw health system
	
	animationtimerforhealthsystem += 0.05f;

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

void Level::checkCollisionPlayerSaw() {
	for (int i = 0; i < saws.size(); i++)
	{
		CollisionObject& saw = *saws[i];
		if (m_state->getPlayer()->intersectDown(saw)) {
			isCollidingSaw = true;
			break;
		}
		else {
			isCollidingSaw = false;
		}
	}
}

void Level::checkCollisionPlayerKey() {
	if (m_state->getPlayer()->intersect(*m_keylevel)) {
		if (m_keylevel->isActive()) {
			graphics::playSound(m_state->getFullAssetPath("keys_pickup.wav"), 0.4f);
			m_state->getPlayer()->m_player_has_key = true;
			mn_keylevel->m_KeyisDeactivating = true;
			m_keylevel->setActive(false);
		}
	}
}

void Level::checkCollisionPlayerDoor() {
	if (m_state->getPlayer()->intersect( *m_leveldoor1 )) {
		isCollidingLevelDoor1 =true;
		if (!m_state->getPlayer()->m_player_has_key) {
			if (graphics::getKeyState(graphics::SCANCODE_E)) {
				graphics::playSound(m_state->getFullAssetPath("door-lock-43124.wav"), 0.3f);
			}
		}
	}
	else {
		isCollidingLevelDoor1 = false;
	}
	
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
				enemies[i]->isCollidingSidewaysEnemy = true;
				enemies[i]->m_pos_x += offset;
				enemies[i]->m_vx = 0.0f;
				
				
				enemies[i]->m_isDeactivating = true;
				break;
				
				/*enemies[i]->collisionStartTime = graphics::getGlobalTime() / 1000.0f; // getGlobalTime returns milliseconds      Jumping system for enemy and deactivation after 5 seconds of not mobility
				if (enemies[i]->isCollidingSidewaysEnemy && graphics::getGlobalTime() / 1000.0f - enemies[i]->collisionStartTime >= 0.5f)
				{
					// Deactivate the enemy
					enemies[i]->m_isDeactivating = true;
					break;
				
				}*/

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
			CollisionObject& enemy = *enemies[i];
			if (enemies[i]->isActive()) {
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
			else {
				m_state->getPlayer()->m_vx = 5.0f;
				enemies[i]->m_vx = 2.0f;
				enemies[i]->isCollidingSidewaysEnemy = false;
				// Set isCollidingPlayerEnemy to false
				enemies[i]->isCollidingPlayerEnemy = false;
			}
	}
}

void Level::init()
{
	const char* lvl1[20][74]{
	{ "10", "04", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05",  "05", "05", "05", "05", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "14", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "14", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "14", "18", "31", "32", "32", "33", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "27", "27", "27", "27", "27", "27", "27", "27", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "19", "13", "14", "45", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0", "27", "27", "0", "0", "0", "0", "0", "31", "32", "32", "33", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "54", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "05", "05", "05", "0",  "0",  "0",  "0",  "74", "75", "75", "76", "05", "06", "0",  "0",  "0",  "0",  "0",  "0",  "61", "0",  "0",   "54", "0",  "0",  "61", "27", "18", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "42", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "54", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "17", "0",  "0",  "0",  "101","101","101","101","0",  "0",  "0",  "0",  "0",  "16", "06", "0",  "0",  "0",  "0",  "0",  "61", "0",  "0",   "54", "0",  "0",  "61", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "42", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "19", "13", "26", "54", "49", "50", "50", "51", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "17", "58", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "16", "06", "0",  "0",  "0",  "0",  "61", "0",  "0",   "63", "0",  "0",  "61", "0", "0", "72", "0", "0", "0", "0", "0", "0", "0", "0", "0", "49", "50", "50", "51", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "63", "36", "62", "47", "11", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "17", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "03", "16", "05", "0",  "0",  "0",  "61", "0",  "0",   "63", "0",  "0",  "61", "0", "0", "0", "0", "72", "72", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "19", "13", "03", "63", "36", "62", "47", "11", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "70", "101","101", "54", "101","101","70", "0", "0", "0", "0", "0", "0", "0", "0", "72", "72", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "19", "13", "14", "55", "56", "57", "11", "11", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "12", "11", "26", "11", "03", "14", "03", "14", "03", "14", "03", "03", "14", "03", "14", "03", "14", "14", "03", "03", "03", "14", "03", "14", "14", "14",  "54", "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "14", "55", "02", "57", "61", "71", "71", "71", "71", "71", "71", "71", "71", "71", "71", "71", "61", "34", "50", "35", "25", "25", "25", "21", "21", "41", "41", "73", "25", "73", "41", "41", "25", "73", "10", "73", "25", "41", "21", "25", "73", "73",  "54", "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "03", "55", "02", "57", "61", "21", "21", "21", "21", "21", "21", "21", "21", "21", "21", "21", "61", "42", "73", "40", "21", "41", "41", "25", "21", "41", "41", "25", "21", "25", "41", "41", "25", "25", "19", "73", "73", "41", "25", "73", "25", "21",  "63", "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "01", "13", "26", "55", "02", "57", "70", "48", "48", "48", "48", "48", "48", "48", "48", "48", "48", "48", "70", "43", "32", "44", "73", "41", "41", "21", "25", "25", "25", "48", "48", "48", "73", "73", "21", "73", "28", "25", "38", "29", "20", "29", "30", "25",  "63", "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "28", "22", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23",  "63", "0",  "0",  "0",  "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"}
	
	};

	int sawctr = 0;
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 74; y++) {
			if (lvl1[x][y] != "0") {
				if (lvl1[x][y] == "101") {
					saws.push_back(std::make_unique<saw>(y - 19, x - 2));
					saws[sawctr]->init();
					sawctr++;
				}
				else {
					m_blocks.push_back(CollisionObject(y - 19, x - 2, 1, 1));
					m_block_names.push_back("IndustrialTile_" + std::string(lvl1[x][y]) + ".png");
				}
			}
		}
	}
	const char* non_coll1[20][74]{
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "21", "0", "32", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "16", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "13", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "25", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "26", "0", "0", "0", "0", "0", "14", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "18", "0", "0", "10", "11", "12", "0", "0", "0", "0", "0", "0", "0", "19", "20", "0", "0", "0", "6", "7", "0", "0", "0", "15", "0", "0", "31", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "22", "26", "24", "0", "23", "0", "25", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "27", "29", "28", "0", "0", "5", "6", "31", "30", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
	{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"}
	
	};


	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 74; y++) {
			if (non_coll1[x][y] != "0") {
				m_non_collidable_blocks.push_back(NonCollisionObject(y - 19, x - 2, 1, 1));
				m_non_collidable_block_names.push_back("NonCollidableTile_" + std::string(non_coll1[x][y]) + ".png");
			}
		}
	}
	
	mn_leveldoor1->init();
	mn_keylevel->init();
	
	for (int i = 0; i <= 2; i++) {
		enemies.push_back(std::make_unique<Enemy>("Enemy" + std::to_string(i), 5.0f * (i + 1), 12.0f));
		enemies[i]->init();
	}

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);

	spitesinit();

}

void Level::spitesinit() {
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

