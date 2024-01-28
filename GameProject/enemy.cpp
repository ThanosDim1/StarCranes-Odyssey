#include "enemy.h"
#include "util.h"
#include "level.h"
#include <cmath>
#include <iostream>



bool Enemy::enemysightleft(float player_x, float player_y, float enemy_x, float enemy_y) {
	return (int)player_y > (int)enemy_y - 2 && (int)player_y < (int)enemy_y + 2 && (int)player_x >= (int)enemy_x - 7 && (int)player_x <= (int)enemy_x;
}

bool Enemy::enemysightright(float player_x, float player_y, float enemy_x, float enemy_y) {
	return (int)player_y > (int)enemy_y - 2 && (int)player_y < (int)enemy_y + 2 && (int)player_x <= (int)enemy_x + 7 && (int)player_x >= (int)enemy_x;
}

void Enemy::update(float dt)
{
	float delta_time = dt / 1000.0f;
	hurtEnemy();
	
	graphics::getMouseState(mouse);

	if (this->m_isDeactivating || this->m_enemygameover)
	{
		this->m_enemyrunleft = false;

		// If the current deactivation sprite reaches the number of deactivation sprites, deactivate the enemy
		if (animationtimerfordeath >= 5)
		{
			delete this;
			this->setActive(false);
		}
	}
	else
	{
		this->m_enemyrunleft = false;
		this->m_enemyrunright = false;
		if (enemysightleft(m_state->getPlayer()->m_pos_x, m_state->getPlayer()->m_pos_y, this->m_pos_x, this->m_pos_y)) {
			this->m_enemyrunleft = true;
			// Move left at a constant speed
			this->m_pos_x -= (this->m_vx * delta_time);


			//Stimulate gravity
			this->m_vy += m_gravity * delta_time;
			this->m_pos_y += this->m_vy * delta_time;
		}
		else if (enemysightright(m_state->getPlayer()->m_pos_x, m_state->getPlayer()->m_pos_y, this->m_pos_x, this->m_pos_y)) {
			this->m_enemyrunright = true;
			// Move left at a constant speed
			this->m_pos_x += (this->m_vx * delta_time);


			//Stimulate gravity
			this->m_vy += m_gravity * delta_time;
			this->m_pos_y += this->m_vy * delta_time;
		}
			
	}	
	GameObject::update(dt);
}


void Enemy::draw()
{
	animationtimer += 0.13f;

	if (this->m_isDeactivating || this->m_enemygameover) {
		// Draw the current deactivation sprite
		int spritesdeactivation = (int)fmod(animationtimerfordeath, m_spritesdeactivation.size());
		m_brush_enemy.texture = m_spritesdeactivation[spritesdeactivation];
		animationtimerfordeath += 0.05f;
	}
	else if (this->isCollidingPlayerEnemy) {
		// Determine whether the player is to the left or right of the enemy
		if (m_state->getPlayer()->m_pos_x > m_pos_x) {
			// Player is to the right, draw the right attack sprite
			int spritesrightattack = (int)fmod(animationtimer, m_spritesrightattack.size());
			m_brush_enemy.texture = m_spritesrightattack[spritesrightattack];
			//graphics::playSound(m_state->getFullAssetPath("minecraft_hit_soundmp3converter.wav"), 0.08f);
			
		}
		else {
			// Player is to the left, draw the left attack sprite
			int spritesleftattack = (int)fmod(animationtimer, m_spritesleftattack.size());
			m_brush_enemy.texture = m_spritesleftattack[spritesleftattack];
			//graphics::playSound(m_state->getFullAssetPath("minecraft_hit_soundmp3converter.wav"), 0.08f);
		}
	}
	else {
		if (m_enemyrunleft) {
			// Draw the current enemy sprite
			int spritesenemy1 = (int)fmod(100.0f - m_pos_x * 4.0f, m_spritesenemy1.size());
			m_brush_enemy.texture = m_spritesenemy1[spritesenemy1];
		}
		else if(m_enemyrunright){
			int spritesenemy2 = (int)fmod(100.0f - m_pos_x * 4.0f, m_spritesenemy2.size());
			m_brush_enemy.texture = m_spritesenemy2[spritesenemy2];
		}
		else {
			int spritesidle = (int)fmod(animationtimeridle, m_spritesidle.size());
			m_brush_enemy.texture = m_spritesidle[spritesidle];
		}
	}

	animationtimeridle += 0.05f;

	// Draw the enemy
	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;
	graphics::drawRect(this->m_pos_x + offset_x, this->m_pos_y + offset_y, 1.0f, 1.0f, m_brush_enemy);

	if (m_state->m_debugging)
		this->debugDraw();
}


void Enemy::init()
{

	m_brush_enemy.fill_opacity = 1.0f;
	m_brush_enemy.outline_opacity = 0.0f;

	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_runleft1.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_runleft2.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_runleft3.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_runleft4.png"));
	m_spritesenemy1.push_back(m_state->getFullAssetPath("Punk_runleft5.png"));

	m_spritesenemy2.push_back(m_state->getFullAssetPath("Punk_runright1.png"));
	m_spritesenemy2.push_back(m_state->getFullAssetPath("Punk_runright2.png"));
	m_spritesenemy2.push_back(m_state->getFullAssetPath("Punk_runright3.png"));
	m_spritesenemy2.push_back(m_state->getFullAssetPath("Punk_runright4.png"));
	m_spritesenemy2.push_back(m_state->getFullAssetPath("Punk_runright5.png"));
	m_spritesenemy2.push_back(m_state->getFullAssetPath("Punk_runright6.png"));

	m_spritesdeactivation.push_back(m_state->getFullAssetPath("PunkDeath1.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("PunkDeath2.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("PunkDeath3.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("PunkDeath4.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("PunkDeath5.png"));

	m_spritesidle.push_back(m_state->getFullAssetPath("idlepunk1.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("idlepunk2.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("idlepunk3.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("idlepunk4.png"));

	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy1LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy2LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy3LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy4LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy5LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy6LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy7LookingLeft.png"));
	m_spritesleftattack.push_back(m_state->getFullAssetPath("Enemy8LookingLeft.png"));


	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy1LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy2LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy3LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy4LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy5LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy6LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy7LookingRight.png"));
	m_spritesrightattack.push_back(m_state->getFullAssetPath("Enemy8LookingRight.png"));

	// Adjust width for finer collision detection
	m_width = 0.7f;
	m_height = 1.0f;
}



void Enemy::debugDraw()
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

void Enemy::hurtEnemy() {
	if (this->isActive()) {
		if (this->isCollidingPlayerEnemy && mouse.button_left_pressed && !graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) {
			this->m_enemy_health -= 1; // enemy loses 2hp
			if (this->m_enemy_health <= 0) { // if player's health is 0 or less
				this->m_enemygameover = true; // game over
				this->m_isDeactivating = true;
			}
		}
	}
}

/* Jumping System For Enemy when hes colliding
void Enemy::moveEnemy(float dt) {
	float delta_time = dt / 1000.0f;

	if (this->isCollidingSidewaysEnemy) {
		this->m_vy = 2.0f;
	}
	this->m_vy += delta_time * m_gravity;
	this->m_pos_y += delta_time * m_vy;
}
*/
