#include "player.h"
#include "util.h"
#include "level.h"
#include <cmath>
#include <iostream>
#include <memory>

// Updates the player's state with each game loop iteration.
void Player::update(float dt)
{
	// Convert delta time from milliseconds to seconds for consistency in physics calculations.
	float delta_time = dt / 1000.0f;

	// Move the player based on input and game physics.
	movePlayer(dt);

	// Iterate through the list of enemies to check and handle any collisions or interactions.
	for (int i = 0; i < m_state->getLevel()->enemies.size(); i++) {
		if (m_state->getLevel()->enemies[i] != nullptr) {
			// Check if the player gets hurt by an enemy and handle the interaction.
			hurtPlayer(m_state->getLevel()->enemies[i]);
		}
	}

	// Check the state of the mouse, looking for input related to player actions.
	graphics::getMouseState(mouse);

	// If the left mouse button is pressed and no attack animation is currently playing, and the player is not moving horizontally,
	// initiate the attack animation and play the associated sound effect.
	if (mouse.button_left_pressed && !isAnimationPlaying && !graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) {
		isAnimationPlaying = true; // Flag the start of an animation.
		animationtimerforattackwithknife = 0; // Reset the animation timer for the attack.
		// Play the slashing sound effect.
		graphics::playSound(m_state->getFullAssetPath("slashkut-108175.wav"), 0.24f);
	}

	// Check if the game over condition is met for the player.
	if (m_gameover) {
		// If the animation timer for death has reached its end, mark the player as dead and deactivate the player object.
		if (animationtimerfordeath >= 6)
		{
			m_state->m_dead = true; // Mark the player as dead in the game state.
			delete m_state->getPlayer(); // Remove the player object from memory.
			setActive(false); // Deactivate the player object, stopping updates and rendering.
		}
	}
	else {
		// If the player is not in a game over state, update the global offset variables to center the camera on the player.
		m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
		m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

		// Call the base class update method to handle any additional generic update logic.
		GameObject::update(dt);
	}
}


// Draws the player's current state or animation frame on the canvas.
void Player::draw()
{
	// Always draw the player in the center of the canvas.
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, 1.0f, 1.0f, m_brush_player);

	// Check if the game is over and draw the deactivation (death) animation.
	if (m_gameover) {
		// Calculate the current frame of the deactivation animation to draw.
		int spritesdeactivation = (int)fmod(animationtimerfordeath, m_spritesdeactivation.size());
		m_brush_player.texture = m_spritesdeactivation[spritesdeactivation];
		animationtimerfordeath += 0.05f; // Increment the timer for the next frame.
	}
	else {
		// Determine and draw the appropriate movement or idle sprite based on keyboard input.
		if (graphics::getKeyState(graphics::SCANCODE_W)) {
			// Jumping animations: choose right or left based on directional input.
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
			// Right movement animation.
			int sprite_right = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesright.size());
			m_brush_player.texture = m_spritesright[sprite_right];
		}
		else if (graphics::getKeyState(graphics::SCANCODE_A) && !graphics::getKeyState(graphics::SCANCODE_D)) {
			// Left movement animation.
			int sprite_left = (int)fmod(100.0f - m_pos_x * 1.5f, m_spritesleft.size());
			m_brush_player.texture = m_spritesleft[sprite_left];
		}
		else {
			// Idle animation when there is no movement input.
			int sprite_idle = (int)fmod(animationtimerforafk, m_spritesidle.size());
			m_brush_player.texture = m_spritesidle[sprite_idle];
		}
	}

	// Increment the timer for idle animation.
	animationtimerforafk += 0.05f;

	// Handle attack animations if an attack is currently playing.
	if (isAnimationPlaying) {
		for (int i = 0; i < m_state->getLevel()->enemies.size(); i++) {
			if (m_state->getLevel()->enemies[i] != nullptr) {
				// Choose the attack animation based on the player's position relative to the enemy.
				if (m_state->getPlayer()->m_pos_x >= m_state->getLevel()->enemies[i]->m_pos_x) {
					// Attack animation for attacking to the left.
					int spritesattackwithknifeleft = (int)fmod(animationtimerforattackwithknife, m_spritesattackwithknifeleft.size());
					m_brush_player.texture = m_spritesattackwithknifeleft[spritesattackwithknifeleft];
				}
				else {
					// Attack animation for attacking to the right.
					int spritesattackwithkniferight = (int)fmod(animationtimerforattackwithknife, m_spritesattackwithkniferight.size());
					m_brush_player.texture = m_spritesattackwithkniferight[spritesattackwithkniferight];
				}

				// Increment the timer for attack animations.
				animationtimerforattackwithknife += 0.05f;

				// Reset the animation flag if the attack animation completes.
				if (animationtimerforattackwithknife >= std::max(m_spritesattackwithkniferight.size(), m_spritesattackwithknifeleft.size())) {
					isAnimationPlaying = false;
				}
			}
		}
	}

	// If debugging is enabled, draw debug information.
	if (m_state->m_debugging)
		debugDraw();
}


// Initialize the player
void Player::init()
{
	// stage 1
	m_pos_x = -10.0f;
	m_pos_y = 11.0f;


	m_state->m_global_offset_x = m_state->getCanvasWidth() / 2.0f - m_pos_x;
	m_state->m_global_offset_y = m_state->getCanvasHeight() / 2.0f - m_pos_y;

	m_brush_player.fill_opacity = 1.0f;
	m_brush_player.outline_opacity = 0.0f;


	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run5.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run4.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run3.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run2.png"));
	m_spritesright.push_back(m_state->getFullAssetPath("Biker_run1.png"));

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

// Draw debug information for the player
void Player::debugDraw()
{
	graphics::Brush debug_brush;
	SETCOLOR(debug_brush.fill_color, 1, 0, 0);
	SETCOLOR(debug_brush.outline_color, 1, 0.1f, 0);
	debug_brush.fill_opacity = 0.1f;
	debug_brush.outline_opacity = 1.0f;
	graphics::drawRect(m_state->getCanvasWidth() * 0.5f, m_state->getCanvasHeight() * 0.5f, m_width + 0.2f, m_height, debug_brush);
}

// Updates the player's position and velocity based on user input and game physics.
void Player::movePlayer(float dt)
{
	// Only update the player's position if the player is active.
	if (m_active) {
		// Convert the time delta from milliseconds to seconds for physics calculations.
		float delta_time = dt / 1000.0f;

		// If the game is over, reset the player's velocities to zero.
		if (m_gameover) {
			m_vx = 0.0f;
			m_vy = 0.0f;
		}

		// If both left (A) and right (D) keys are pressed simultaneously, stop horizontal movement.
		if (graphics::getKeyState(graphics::SCANCODE_A) && graphics::getKeyState(graphics::SCANCODE_D)) {
			m_vx = 0.0f;
		}

		// Implements a smart jumping system that adjusts the jump force based on the player's situation.
		if (graphics::getKeyState(graphics::SCANCODE_W)) {
			// Allow jumping only if the player is colliding with the ground.
			if (isCollidingDown) {
				// Play jump sound effect.
				graphics::playSound(m_state->getFullAssetPath("jumps.wav"), 0.3f);
				// Adjust the vertical velocity for the jump based on the offset from the ground.
				if (offsetmvy == 2) {
					m_vy -= 5.0f;
				}
				else if (offsetmvy == 1) {
					m_vy -= 2.0f;
				}
				else {
					m_vy -= 7.0f;
				}
			}
			// Reset the vertical offset after jumping.
			offsetmvy = 0.0f;
		}

		// If colliding with spikes, apply a horizontal force to simulate knockback.
		if (m_state->getLevel()->isCollidingSpike) {
			m_vx = 3.0f;
		}

		// Move the player left or right based on horizontal velocity and delta time.
		// Decrease position for left movement.
		if (graphics::getKeyState(graphics::SCANCODE_A)) {
			m_pos_x -= (delta_time * m_vx);
		}
		// Increase position for right movement.
		else if (graphics::getKeyState(graphics::SCANCODE_D)) {
			m_pos_x += (delta_time * m_vx);
		}

		// Apply gravity to the player's vertical velocity.
		m_vy += delta_time * m_gravity;

		// Update the player's vertical position based on the vertical velocity.
		m_pos_y += delta_time * m_vy;
	}
}


// Function to handle the player taking damage from various hazards like enemies, saws, and spikes.
void Player::hurtPlayer(Enemy* enemy) {

	// Check if the player collides with a saw and apply damage and sound effect accordingly.
	if (m_state->getLevel()->isCollidingSaw) {
		m_player_health -= 5; // Decrease player health by 5.
		// Play sound effect for saw damage.
		graphics::playSound(m_state->getFullAssetPath("ElectricSaw.wav"), 0.04f, false);
		m_gameover = true; // Set game over condition if hit by a saw.
	}

	// Timer to control damage frequency from enemy attacks.
	static float timerenemy = 0.0f;
	if (enemy->isActive()) {
		if (enemy->isCollidingPlayerEnemy) {
			timerenemy += 0.32f; // Increment timer to throttle damage application.
			if (timerenemy >= 30.0f) {
				m_player_health -= 1; // Decrease player health by 1.
				// Play sound effect for taking damage.
				graphics::playSound(m_state->getFullAssetPath("classic_hurt.wav"), 0.4f);
				if (m_player_health <= 0) {
					m_gameover = true; // Trigger game over condition if health drops to 0 or below.
				}
				timerenemy = 0.0f; // Reset timer after applying damage.
			}
		}
	}

	// Timer to control damage frequency from spike collisions.
	static float timerspike = 0.0f;
	if (m_state->getLevel()->isCollidingSpike) {
		timerspike += 0.32f; // Increment timer to throttle damage application from spikes.
		if (timerspike >= 100.0f) {
			m_player_health -= 1; // Decrease player health by 1.
			// Play sound effect for taking damage.
			graphics::playSound(m_state->getFullAssetPath("classic_hurt.wav"), 0.4f);
			if (m_player_health <= 0) {
				m_gameover = true; // Trigger game over condition if health drops to 0 or below.
			}
			timerspike = 0.0f; // Reset timer after applying damage.
		}
	}
}


// Attempts to open a door when the player interacts with it, given certain conditions are met.
bool Player::openDoor() {
	bool soundPlayed = false; // Flag to ensure the sound is played only once.

	// Only proceed if the player is active.
	if (this->m_active) {
		// Check if the player is colliding with the level's door and has the key.
		if (m_state->getLevel()->isCollidingLevelDoor1 && m_player_has_key) {
			// Verify that the door is not already open.
			if (dooropen == false) {
				// If the player presses the 'E' key to interact with the door.
				if (graphics::getKeyState(graphics::SCANCODE_E)) {
					// Play the door opening sound effect once.
					if (!soundPlayed) {
						graphics::playSound(m_state->getFullAssetPath("level-up-bonus-sequence-3-186892.wav"), 0.15f);
						soundPlayed = true; // Prevent the sound from playing multiple times.
					}
					// Special logic for level progression.
					// If the current level is the first level and it's finished.
					if (m_state->getLevel()->lvl1_finished == true) {
						// Then mark the second level as finished and reset the first level's flag.
						m_state->getLevel()->lvl2_finished = true;
						m_state->getLevel()->lvl1_finished = false;
						dooropen = true; // Mark the door as open.
						return true; // Indicate successful door opening.
					}
					// If the condition above is not met, simply mark the first level as finished and open the door.
					m_state->getLevel()->lvl1_finished = true;
					dooropen = true;
					return true;
				}
			}
		}
	}
	return false; // Return false if the door cannot be opened or the conditions are not met.
}
