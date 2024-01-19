#include "player.h"
#include "util.h"
#include "level.h"
#include <cmath>
#include <iostream>


void Player::update(float dt)
{
	float delta_time = dt / 1000.0f;

	movePlayer(dt);
	hurtPlayer();
	graphics::getMouseState(mouse);
	
	if (mouse.button_left_pressed && !isAnimationPlaying && !graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) {
		isAnimationPlaying = true;
		animationtimerforattackwithknife = 0; // Reset the animation timer
	}

	if (m_gameover) {
		// If the current deactivation sprite reaches the number of deactivation sprites, deactivate the player
		if (animationtimerfordeath >= 6)
		{
			delete m_state->getPlayer();
			setActive(false);
		}
	}
	else {
	// update offset for other game objects
	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;


	GameObject::update(dt);
	}

}

void Player::draw()
{	
	animationtimerforafk+= 0.05f;

	if (isAnimationPlaying) {
		if (m_state->getPlayer()->m_pos_x > m_state->getEnemy()->m_pos_x) {
			int spritesattackwithknifeleft = (int)fmod(animationtimerforattackwithknife, m_spritesattackwithknifeleft.size());
			m_brush_player.texture = m_spritesattackwithknifeleft[spritesattackwithknifeleft];
		}
		else {
			int spritesattackwithkniferight = (int)fmod(animationtimerforattackwithknife, m_spritesattackwithkniferight.size());
			m_brush_player.texture = m_spritesattackwithkniferight[spritesattackwithkniferight];
		}

		// Increment the animation timer
		animationtimerforattackwithknife += 0.1f;

		// If the animation has finished, reset the isAnimationPlaying variable
		if (animationtimerforattackwithknife >= std::max(m_spritesattackwithkniferight.size(), m_spritesattackwithknifeleft.size())) {
			isAnimationPlaying = false;
		}
	}
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);
	if (m_gameover) {
		// Draw the current deactivation sprite
		int spritesdeactivation = (int)fmod(animationtimerfordeath, m_spritesdeactivation.size());
		m_brush_player.texture = m_spritesdeactivation[spritesdeactivation];
		animationtimerfordeath += 0.05f;
	}
	else
	if (graphics::getKeyState(graphics::SCANCODE_W)) {
		if (graphics::getKeyState(graphics::SCANCODE_D) && !graphics::getKeyState(graphics::SCANCODE_A)) {
			int sprite_jumpright = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesjumpright.size());
			m_brush_player.texture = m_spritesjumpright[sprite_jumpright];
		}
		else if (graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) {
			int sprite_jumpleft = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesjumpleft.size());
			m_brush_player.texture = m_spritesjumpleft[sprite_jumpleft];
		}
	}
	else if (graphics::getKeyState(graphics::SCANCODE_D) && !graphics::getKeyState(graphics::SCANCODE_A)) {
		int sprite_right = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesright.size());
		m_brush_player.texture = m_spritesright[sprite_right];
	}
	else if (graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) {
		int sprite_left = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesleft.size());
		m_brush_player.texture = m_spritesleft[sprite_left];
	}
	else {
		int sprite_idle = (int)fmod(animationtimerforafk, m_spritesidle.size());
		m_brush_player.texture = m_spritesidle[sprite_idle];
	}

	//Draw Player
	

	if (m_state->m_debugging)
		debugDraw();

	
}

void Player::init()
{
	// stage 1
	m_pos_x = -6.0f;
	m_pos_y = 0.0f;

	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;

	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run1.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run2.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run3.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run4.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run5.png"));

	m_spritesidle.push_back(m_state->getFullAssetPath("Biker_idle2.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("Biker_idle3.png"));
	m_spritesidle.push_back(m_state->getFullAssetPath("Biker_idle4.png"));

	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump1.png"));
	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump2.png"));
	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump3.png"));
	m_spritesjumpright.push_back(m_state->getFullAssetPath("Biker_jump4.png"));

	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft1.png"));
	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft2.png"));
	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft3.png"));
	m_spritesjumpleft.push_back(m_state->getFullAssetPath("Biker_jumpleft4.png"));

	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left1.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left2.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left3.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left4.png"));
	m_spritesleft.push_back(m_state->getFullAssetPath("Biker_left5.png"));

	m_spritesdeactivation.push_back(m_state->getFullAssetPath("BikerDeath1.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("BikerDeath2.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("BikerDeath3.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("BikerDeath4.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("BikerDeath5.png"));
	m_spritesdeactivation.push_back(m_state->getFullAssetPath("BikerDeath6.png"));

	m_spritesattackwithkniferight.push_back(m_state->getFullAssetPath("AttackKnife1R.png"));
	m_spritesattackwithkniferight.push_back(m_state->getFullAssetPath("AttackKnife2R.png"));
	m_spritesattackwithkniferight.push_back(m_state->getFullAssetPath("AttackKnife3R.png"));
	m_spritesattackwithkniferight.push_back(m_state->getFullAssetPath("AttackKnife4R.png"));
	m_spritesattackwithkniferight.push_back(m_state->getFullAssetPath("AttackKnife5R.png"));
	m_spritesattackwithkniferight.push_back(m_state->getFullAssetPath("AttackKnife6R.png"));

	m_spritesattackwithknifeleft.push_back(m_state->getFullAssetPath("AttackKnifeL1.png"));
	m_spritesattackwithknifeleft.push_back(m_state->getFullAssetPath("AttackKnifeL2.png"));
	m_spritesattackwithknifeleft.push_back(m_state->getFullAssetPath("AttackKnifeL3.png"));
	m_spritesattackwithknifeleft.push_back(m_state->getFullAssetPath("AttackKnifeL4.png"));
	m_spritesattackwithknifeleft.push_back(m_state->getFullAssetPath("AttackKnifeL5.png"));
	m_spritesattackwithknifeleft.push_back(m_state->getFullAssetPath("AttackKnifeL6.png"));

	// Adjust width for finer collision detection
	m_width = 0.5f;
	m_height = 1.0f;
}



void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0.3f, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width , m_height, debug_brush);

}

void Player::movePlayer(float dt)
{
	float delta_time = dt / 1000.0f;
	
	if (m_gameover) {
		m_vx = 0.0f;
		m_vy = 0.0f;
	}

	
	if (graphics::getKeyState(graphics::SCANCODE_A) && graphics::getKeyState(graphics::SCANCODE_D)) {
		m_vx = 0.0f;
	}

	if (graphics::getKeyState(graphics::SCANCODE_W))
		if (m_state->getLevel()->getCollDown())
			m_vy -= 7.0f;

	if (graphics::getKeyState(graphics::SCANCODE_A)) {
		m_pos_x -= (delta_time * m_vx);
	}
	else if (graphics::getKeyState(graphics::SCANCODE_D)) {
		m_pos_x += (delta_time * m_vx);
	}

	m_vy += delta_time * m_gravity;

	m_pos_y += delta_time * m_vy;
}

void Player::hurtPlayer() {
	static float timer = 0.0f; // timer to keep track of time since collision started
	if (m_state->getLevel()->isCollidingPlayerEnemy) {
		timer += 0.5f; // increment timer by the elapsed time
		if (timer >= 30.0f) { // if timer exceeds the enemy's attack animation duration
			m_player_health -= 1; // player loses 1hp
			if (m_player_health <= 0) { // if player's health is 0 or less
				m_state->getPlayer()->m_gameover = true; // game over
			}
			timer = 0.0f; // reset timer
		}
		std::cout << "Player Health: " << m_player_health;
	}
	else {
		timer = 0.0f; // reset timer if player is not colliding with enemy
	}
}

