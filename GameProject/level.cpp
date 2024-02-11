#include "level.h"
#include <sgg/graphics.h>
#include "player.h"
#include "enemy.h"
#include "util.h"
#include <iostream>


// Draws a specific block from the level's collection of blocks.
void Level::drawBlock(int i, std::vector<CollisionObject> m_blocks)
{
	// Reference to the specific CollisionObject from the vector of blocks.
	CollisionObject& CollisionObject = m_blocks[i];

	// Retrieve the name of the texture associated with this block.
	std::string& name = m_block_names[i];

	// Calculate the actual position of the block by applying the global offset.
	// This allows the block to be drawn in the correct position relative to the game world's current view.
	float x = CollisionObject.m_pos_x + m_state->m_global_offset_x;
	float y = CollisionObject.m_pos_y + m_state->m_global_offset_y;

	// Set the texture for the block using the full asset path of the texture name.
	m_block_brush.texture = m_state->getFullAssetPath(name);

	// Draw the block at its calculated position with a specified size.
	graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush);

	// If debugging is enabled, draw the block again with a debugging brush.
	// This could be used to highlight blocks in a different color or style for debugging purposes.
	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);
}


// Draws a specific non-collidable block from the level's collection of such blocks.
void Level::drawNonCollisionBlock(int i, std::vector<NonCollisionObject> m_non_collidable_blocks)
{
	// Reference to the specific NonCollisionObject from the vector of non-collidable blocks.
	NonCollisionObject& NonCollisionObject = m_non_collidable_blocks[i];

	// Retrieve the name of the texture associated with this non-collidable block.
	std::string& name = m_non_collidable_block_names[i];

	// Calculate the actual position of the non-collidable block by applying the global offset.
	// This adjustment ensures the block is drawn in the correct position relative to the game world's current view.
	float x = NonCollisionObject.m_pos_x + m_state->m_global_offset_x;
	float y = NonCollisionObject.m_pos_y + m_state->m_global_offset_y;

	// Set the texture for the block using the full asset path of the texture name.
	m_block_brush.texture = m_state->getFullAssetPath(name);

	// Draw the non-collidable block at its calculated position with a specified size.
	graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush);

	// If debugging is enabled, draw the block again with a debugging brush.
	// This could be used to highlight non-collidable blocks in a different color or style for debugging purposes,
	// helping to visually distinguish them from collidable blocks and other game elements.
	if (m_state->m_debugging)
		graphics::drawRect(x, y, m_block_size, m_block_size, m_block_brush_debug);
}


// Updates all relevant game objects and checks for collisions.
void Level::update(float dt)
{
	// Update the player if they are active.
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->update(dt); // Call the player's update function with the delta time.

	// Update the enemies within the level.
	EnemiesCheck(dt, enemies); // Custom function to update all enemies based on delta time.

	// Perform collision checks between the player and various game elements.
	checkCollisionPlayerSpike(spikes); // Check for collisions between the player and spikes.
	checkCollisionPlayerDoor(); // Check if the player is trying to open a door.
	checkCollisionsForEnemy(m_blocks, enemies); // Check for collisions between enemies and blocks.
	checkCollisions(m_blocks); // Check for collisions between the player and blocks.
	checkCollisionPlayerSaw(saws); // Check for collisions between the player and saws.
	checkCollisionPlayerStar(stars); // Check for collisions between the player and stars (collectibles).
	checkCollisionsMovingObjects(enemies); // Check for collisions involving moving objects, presumably enemies.

	checkCollisionPlayerKey(m_keylevel1, mn_keylevel1); // Check if the player has collided with a key object.

	GameObject::update(dt); // Call the base class update method to handle any additional generic logic.
}


// Iterates through the vector of enemies and updates each active enemy based on the delta time (dt).
void Level::EnemiesCheck(float dt, std::vector<Enemy*> enemies) {
	// Loop through all enemies in the provided vector.
	for (int i = 0; i < enemies.size(); i++)
	{
		// Check if the current enemy in the iteration is active.
		if (enemies[i]->isActive()) {
			// If the enemy is active, call its update function, passing the delta time.
			// This allows the enemy to perform actions such as moving, attacking, or any other logic defined in its update method.
			enemies[i]->update(dt);
		}
	}
}


// Renders all components of the current game level.
void Level::draw()
{
	// Obtain the canvas dimensions from the game state.
	float w = m_state->getCanvasWidth();
	float h = m_state->getCanvasHeight();

	// Retrieve the global offset values which are used to align the game's view.
	float offset_x = m_state->m_global_offset_x;
	float offset_y = m_state->m_global_offset_y;

	// Draw the background with a specified size and position, adjusting with global offsets.
	// This creates a large background that spans beyond the canvas size for a parallax or scrolling effect.
	graphics::drawRect(offset_x, offset_y + 2.0f, 6.0f * w, 3.0f * h, m_brush_background);

	// Iterate through and draw all static (collidable) blocks in the level.
	for (int i = 0; i < m_blocks.size(); i++)
	{
		drawBlock(i, m_blocks);
	}

	// Iterate through and draw all non-collidable blocks in the level.
	for (int i = 0; i < m_non_collidable_blocks.size(); i++)
	{
		drawNonCollisionBlock(i, m_non_collidable_blocks);
	}

	// Iterate through and draw all saws in the level, if they are active.
	for (int i = 0; i < saws.size(); i++)
	{
		if (saws[i]->isActive()) {
			saws[i]->draw(0.30f);
		}
	}

	// Iterate through and draw all stars in the level, if they are active.
	for (int i = 0; i < stars.size(); i++) {
		if (stars[i]->isActive()) {
			stars[i]->draw(0.17f);
		}
	}

	// Iterate through and draw all spikes in the level, if they are active.
	for (int i = 0; i < spikes.size(); i++) {
		if (spikes[i]->isActive()) {
			spikes[i]->draw(0.0f);
		}
	}

	// Draw the level door, with an additional parameter perhaps indicating a scaling factor or animation state.
	mn_leveldoor1->draw(0.12f);

	// Draw the key object for the level, if it exists.
	if (m_keylevel1 != nullptr) {
		mn_keylevel1->draw(0.25f);
	}

	// Iterate through and draw all enemies in the level, if they are active.
	for (int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i]->isActive())
			enemies[i]->draw();
	}

	// Draw the player character, if they are active.
	if (m_state->getPlayer()->isActive())
		m_state->getPlayer()->draw();

	// Additional functions to draw the health system, key indicator, and star counter or collectibles indicator.
	drawHealthSystem();
	drawKey();
	drawStar();
}


// Renders the key indicator on the screen based on whether the player has collected a key.
void Level::drawKey() {
	// Check if the player has collected the key.
	if (m_state->getPlayer()->m_player_has_key) {
		// Set the texture to represent the key being collected.
		m_brush_key_system.texture = m_spriteskeycollected[1];
		// Draw the key indicator at a specified position with dimensions, indicating possession.
		graphics::drawRect(12.5, 1.2, 0.8, 0.6, m_brush_key_system);
		// Ensure the outline of the key indicator is not visible by setting its opacity to 0.
		m_brush_key_system.outline_opacity = 0.0f;
	}
	else {
		// Set the texture to represent the key not being collected yet.
		m_brush_key_system.texture = m_spriteskeycollected[0];
		// Draw the key indicator at a slightly adjusted position to perhaps differentiate from the collected state.
		graphics::drawRect(12.5, 1.15, 0.8, 0.6, m_brush_key_system);
		// Ensure the outline of the key indicator is not visible by setting its opacity to 0.
		m_brush_key_system.outline_opacity = 0.0f;
	}
}


// Renders the star indicator on the screen to reflect the player's current star collection status.
void Level::drawStar() {
	// Check if the number of stars collected by the player is within the valid range of the sprites available.
	if (m_state->getPlayer()->m_player_has_star >= 0 && m_state->getPlayer()->m_player_has_star < m_spritesstarsystem.size()) {
		// Set the texture for the star indicator based on the number of stars the player has collected.
		// This texture is selected from an array of textures, where each texture represents a different number of collected stars.
		m_brush_star_system.texture = m_spritesstarsystem[m_state->getPlayer()->m_player_has_star];

		// Draw the star indicator at the specified position on the screen with the specified dimensions.
		// This visually represents the player's progress in collecting stars.
		graphics::drawRect(14.3, 1.1, 2.2, 1.4, m_brush_star_system);

		// Ensure that the outline of the star indicator is not visible by setting its outline opacity to 0.
		// This makes the indicator blend more seamlessly with the game's graphical interface.
		m_brush_star_system.outline_opacity = 0.0f;
	}
}

// Renders the health system indicator, visually representing the player's current health.
void Level::drawHealthSystem() {
	// Increment the animation timer for the health system to cycle through the animation frames.
	animationtimerforhealthsystem += 0.05f;

	// Check the player's current health and select the appropriate sprite set for the health indicator.
	if (m_state->getPlayer()->m_player_health == 5) {
		// Player's health is full.
		int spriteshealthsystemfull = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemfull.size());
		m_brush_health_system.texture = m_spriteshealthsystemfull[spriteshealthsystemfull];
	}
	else if (m_state->getPlayer()->m_player_health == 4) {
		// Player's health is almost full.
		int spriteshealthsystemalmostfull = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemalmosthalf.size());
		m_brush_health_system.texture = m_spriteshealthsystemalmosthalf[spriteshealthsystemalmostfull];
	}
	else if (m_state->getPlayer()->m_player_health == 3) {
		// Player's health is at half.
		int spriteshealthsystemhalf = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemhalf.size());
		m_brush_health_system.texture = m_spriteshealthsystemhalf[spriteshealthsystemhalf];
	}
	else if (m_state->getPlayer()->m_player_health == 2 || m_state->getPlayer()->m_player_health == 1) {
		// Player is on their last lives.
		int spriteshealthsystemlastlife = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemlastlife.size());
		m_brush_health_system.texture = m_spriteshealthsystemlastlife[spriteshealthsystemlastlife];
	}
	else {
		// Player's health is at the death state.
		int spriteshealthsystemdeath = (int)fmod(animationtimerforhealthsystem, m_spriteshealthsystemdeath.size());
		m_brush_health_system.texture = m_spriteshealthsystemdeath[spriteshealthsystemdeath];
	}

	// Draw the health indicator on the screen at a specified position with a specified size.
	graphics::drawRect(1.3, 1.1, 1.8, 0.5, m_brush_health_system);
	// Ensure the outline of the health indicator is not visible by setting its outline opacity to 0.
	m_brush_health_system.outline_opacity = 0.0f;
}


// Constructor for the Level class, inheriting from GameObject.
Level::Level(const std::string& name)
	: GameObject(name) // Initialize the base GameObject class with the level name.
{
	// Check the name of the level to determine specific settings.
	if (name == "1.lvl") {
		// For level 1, set the outline opacity of the background brush to 0 to ensure no outline is visible.
		m_brush_background.outline_opacity = 0.0f;
		// Set the texture for the background brush to a specific image file, indicating this level's background.
		m_brush_background.texture = m_state->getFullAssetPath("background7.png");
	}
	else if (name == "2.lvl") {
		// Similar setup for level 2, but with a different background image.
		m_brush_background.outline_opacity = 0.0f;
		m_brush_background.texture = m_state->getFullAssetPath("background2.png");
	}
	// Additional level configurations could be added here with else-if statements.
}


//Destructor
Level::~Level()
{
	
}

// Checks for collisions between the player and spike objects in the level.
void Level::checkCollisionPlayerSpike(std::vector<Spikes*> spikes) {
	// Iterate through all spike objects to check for collisions.
	for (int i = 0; i < spikes.size(); i++)
	{
		// Create a reference to the current spike object as a CollisionObject for collision checking.
		CollisionObject& spike = *spikes[i];

		// Use the player's intersect method to check if there is a collision with the current spike.
		if (m_state->getPlayer()->intersect(spike)) {
			// If there is a collision, set the isCollidingSpike flag to true.
			isCollidingSpike = true;
			// Break out of the loop since we only need to detect one collision to set the flag.
			break;
		}
		else {
			// If there is no collision, ensure the isCollidingSpike flag is set to false.
			// Note: This approach may incorrectly reset the flag if a collision was detected with a previous spike in the list.
			isCollidingSpike = false;
		}
	}
}


// Checks for collisions between the player and saw objects in the level.
void Level::checkCollisionPlayerSaw(std::vector<saw*> saws) {
	// Iterate through all saw objects in the level.
	for (int i = 0; i < saws.size(); i++)
	{
		// Dereference the current saw pointer to get a CollisionObject reference.
		CollisionObject& saw = *saws[i];

		// Use the player's intersectDown method to check if there is a downward collision with the current saw.
		// This might imply checking if the player is landing on the saw from above.
		if (m_state->getPlayer()->intersectDown(saw)) {
			// If there is a collision, set the isCollidingSaw flag to true.
			isCollidingSaw = true;
			// Break out of the loop since we only need one collision to confirm the player is colliding with a saw.
			break;
		}
		else {
			// If there is no collision detected with the current saw, set the isCollidingSaw flag to false.
			// This approach may not be entirely accurate if the player is colliding with multiple saws, as the flag
			// will be set to false if the last saw checked does not collide with the player.
			isCollidingSaw = false;
		}
	}
}


// Checks for collisions between the player and star objects within the level.
void Level::checkCollisionPlayerStar(std::vector<Star*> stars) {
	// Iterate through all star objects in the level.
	for (int i = 0; i < stars.size(); i++) {
		// Check if the player is colliding with the current star object.
		if (m_state->getPlayer()->intersect(*stars[i])) {
			// Only proceed if the star is active (not yet collected).
			if (stars[i]->isActive()) {
				// Play a sound effect for collecting a star.
				graphics::playSound(m_state->getFullAssetPath("sound-effect-twinklesparkle-115095.wav"), 0.4f);
				// Increase the player's star count by 1.
				m_state->getPlayer()->m_player_has_star += 1;
				// Deactivate the star, indicating it has been collected and should no longer be visible or collide with the player.
				stars[i]->setActive(false);
			}
		}
	}

	// Play a special sound effect when the player has collected exactly 3 stars.
	// This part of the code appears to reference a non-declared variable 'soundPlayed', which should ideally be checked and updated within a broader scope to avoid repeated sound playback.
	if (m_state->getPlayer()->m_player_has_star == 3 && !soundPlayed) {
		graphics::playSound(m_state->getFullAssetPath("ntolmadakia.wav"), 0.5f);
		soundPlayed = true; // This should set the flag to prevent multiple playbacks, but 'soundPlayed' is not declared in this snippet.
	}
}

void Level::checkCollisionPlayerKey(AnimatedObjects* m_keylevel, KeyLevel* mn_keylevel) {
	// Check if player intersects with the key
	if (m_state->getPlayer()->intersect(*m_keylevel)) {
		// Check if the key is active
		if (m_keylevel->isActive()) {
			// Play key pickup sound effect
			graphics::playSound(m_state->getFullAssetPath("keys_pickup-27204 .wav"), 0.4f);

			// Set player's has key to true, to be able to unlock the door
			m_state->getPlayer()->m_player_has_key = true;

			// Deactivate the key and mark it for deactivation
			mn_keylevel->m_KeyisDeactivating = true;
			m_keylevel->setActive(false);

			// Delete the key object
			delete m_keylevel;
		}
	}
}


void Level::checkCollisionPlayerDoor() {
	// Check if player intersects with the level door
	if (m_state->getPlayer()->intersect(*m_leveldoor1)) {
		isCollidingLevelDoor1 = true;

		// Check if player does not have the key
		if (!m_state->getPlayer()->m_player_has_key) {
			// Check if the player presses the 'E' key
			if (graphics::getKeyState(graphics::SCANCODE_E)) {
				// Play door lock sound effect
				graphics::playSound(m_state->getFullAssetPath("door-lock-43124.wav"), 0.3f);
			}
		}

		// Show text prompt if not already shown
		if (!text_showed) {
			graphics::setFont(m_state->getFullAssetPath("ThaleahFat.ttf"));
			graphics::drawText(5.7f, 8.9f, 0.7f, "Press E to open door", brush_text);
		}
	}
	else {
		isCollidingLevelDoor1 = false;
	}
}

//Check collision between player and enemies
void Level::checkCollisions(std::vector<CollisionObject> m_blocks)
{
	// Iterate over each block to check for sideways collisions with the player.
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		// Check for a sideways collision with the current block and get the offset needed to resolve the collision.
		if (offset = m_state->getPlayer()->intersectSideways(block))
		{
			// Mark that the player is colliding sideways and adjust the player's position by the offset.
			m_state->getPlayer()->isCollidingSideways = true;
			m_state->getPlayer()->m_pos_x += offset;
			// Stop the player's horizontal movement by setting the horizontal velocity to 0.
			m_state->getPlayer()->m_vx = 0.0f;

			break; // Stop checking after the first collision is found and handled.
		}
	}

	// Iterate over each block to check for downward collisions (e.g., landing on top of a block).
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		// Check for a downward collision with the current block and get the offset needed to resolve the collision.
		if (offset = m_state->getPlayer()->intersectDown(block))
		{
			// If the player is moving down fast, play a landing sound effect.
			if (m_state->getPlayer()->m_vy > 1.0f)
				graphics::playSound(m_state->getFullAssetPath("land.wav"), 0.2f);

			// Mark that the player is colliding downwards and stop the player's vertical movement.
			m_state->getPlayer()->isCollidingDown = true;
			m_state->getPlayer()->m_vy = 0.0f;

			// Additional check related to sideways collision handling that seems redundant due to misplacement or logical error.
			if ((m_state->getPlayer()->m_vy == 0 || !m_state->getPlayer()->isCollidingSideways) && m_state->getPlayer()->isCollidingDown)
			{
				if (m_state->getPlayer()->isCollidingSideways)
				{
					// Attempts to handle horizontal velocity adjustments that are already addressed.
					m_state->getPlayer()->m_vx = 0.0f;
					m_state->getPlayer()->m_vy = 0.0f;
					m_state->getPlayer()->isCollidingSideways = false;
				}

				// Adjust the player's vertical position by the offset.
				m_state->getPlayer()->m_pos_y += offset;

				break; // Stop checking after the first collision is found and handled.
			}
		}
		else
		{
			// If no downward collision is detected, ensure the flag is set to false.
			m_state->getPlayer()->isCollidingDown = false;
		}
	}

	// Iterate over each block to check for upward collisions (e.g., hitting the head on a block).
	for (auto& block : m_blocks)
	{
		// Check for an upward collision with the current block.
		if (m_state->getPlayer()->intersectUp(block))
		{
			// Adjust the player's vertical movement based on the collision.
			// There seems to be a logical inconsistency as 'offsetmvy' is assigned but not defined in this context.
			m_state->getPlayer()->offsetmvy = m_state->getPlayer()->intersectUp(block);
		}
	}
}

// Check collision between enemies and blocks
void Level::checkCollisionsForEnemy(std::vector<CollisionObject> m_blocks, std::vector<Enemy*> enemies)
{
	// First, check for sideways collisions between each block and enemy.
	for (auto& block : m_blocks)
	{
		float offset = 0.0f;
		for (int i = 0; i < enemies.size(); i++)
		{
			// If a sideways collision is detected, adjust the enemy's position and stop its horizontal movement.
			if ((offset = enemies[i]->intersectSideways(block)))
			{
				enemies[i]->isCollidingSidewaysEnemy = true; // Mark the enemy as colliding sideways.
				enemies[i]->m_pos_x += offset; // Adjust the enemy's position based on the collision offset.
				enemies[i]->m_vx = 0.0f; // Stop the enemy's horizontal movement by setting its velocity to 0.

				break; // Once a collision is handled, break out of the loop to avoid double handling.
			}
		}
	}

	// Next, check for downward collisions between each block and enemy.
	for (auto& block : m_blocks)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			float offset = 0.0f;
			// If a downward collision is detected, adjust the enemy's vertical position and stop its vertical movement.
			if ((offset = enemies[i]->intersectDown(block)))
			{
				enemies[i]->isCollidingDownEnemy = true; // Mark the enemy as colliding downward.
				enemies[i]->m_vy = 0.0f; // Stop the enemy's vertical movement by setting its velocity to 0.

				// The following conditional logic seems to be redundant or incorrect due to the misuse of conditions.
				if ((enemies[i]->m_vy == 0 || !enemies[i]->isCollidingSidewaysEnemy) && enemies[i]->isCollidingDownEnemy)
				{
					if (enemies[i]->isCollidingSidewaysEnemy)
					{
						// Redundantly stops the enemy's movement again, despite already being handled above.
						enemies[i]->m_vx = 0.0f;
						enemies[i]->m_vy = 0.0f;
						enemies[i]->isCollidingSidewaysEnemy = false; // Resets the sideways collision flag.
					}

					enemies[i]->m_pos_y += offset; // Adjust the enemy's vertical position based on the collision offset.
				}
			}
			else
			{
				enemies[i]->isCollidingDownEnemy = false; // If no collision is detected, ensure the flag is cleared.
			}
		}
	}
}


//Check collision between player and moving objects
void Level::checkCollisionsMovingObjects(std::vector<Enemy*> enemies) {

	for (int i = 0; i < enemies.size(); i++) {
			CollisionObject& enemy = *enemies[i];
			if (enemies[i]->isActive()) {
				if (m_state->getPlayer()->intersectSideways(enemy)) {

					float delta_time = graphics::getDeltaTime() / 1000.0f;
					enemies[i]->m_vx = 0.0f;
					enemies[i]->isCollidingPlayerEnemy = true; // Set isCollidingPlayerEnemy to true
					if (graphics::getKeyState(graphics::SCANCODE_A)) {
						m_state->getPlayer()->m_vx = 3.0f;   // Apply deceleration force to player's velocity
					}
					else if (graphics::getKeyState(graphics::SCANCODE_D)) {
						m_state->getPlayer()->m_vx = 2.0f;   // Apply deceleration force to player's velocity
					}
					break;
				}
				else {
					m_state->getPlayer()->m_vx = 5.0f;	//reset player's velocity
					enemies[i]->m_vx = 2.0f; //reset enemy's velocity
					enemies[i]->isCollidingSidewaysEnemy = false;
					enemies[i]->isCollidingPlayerEnemy = false; // Set isCollidingPlayerEnemy to false
				}
			}
			else {
				m_state->getPlayer()->m_vx = 5.0f;
				enemies[i]->m_vx = 2.0f;
				enemies[i]->isCollidingSidewaysEnemy = false;
				enemies[i]->isCollidingPlayerEnemy = false;  // Set isCollidingPlayerEnemy to false
			}
	}
}

// Processes two 2D arrays to create game objects. The first array, lvl1, contains collidable objects,
// and the second array, non_coll1, contains non-collidable objects.
void Level::ArrayCheck(const char* lvl[20][74], const char* non_coll[20][74]) {

	// Initialize counters for different object types to manage their creation and initialization.
	int sawctr = 0;
	int enemyctr = 0;
	int spikesctr = 0;

	// Iterate over the lvl1 array to identify and create collidable objects based on their coded values.
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 74; y++) {
			// Check if the current cell is not empty (i.e., not "0").
			if (lvl[x][y] != "0") {
				// Convert the string value to an integer to determine the object type.
				switch (std::stoi(lvl[x][y])) {
				case 110:
					// Create an Enemy object at the specified position, initialize it, and increment the counter.
					enemies.push_back(new Enemy("Enemy" + std::to_string(enemyctr), y - 19, x - 2));
					enemies[enemyctr]->init();
					enemyctr++;
					break;
				case 111:
					// Create a Spikes object at the specified position, initialize it, and increment the counter.
					spikes.push_back(new Spikes(y - 19, x - 2));
					spikes[spikesctr]->init();
					spikesctr++;
					break;
				case 101:
					// Create a Saw object at the specified position, initialize it, and increment the counter.
					saws.push_back(new saw(y - 19, x - 2));
					saws[sawctr]->init();
					sawctr++;
					break;
				default:
					// For any other value, create a generic CollisionObject and store its name.
					m_blocks.push_back(CollisionObject(y - 19, x - 2, 1, 1));
					m_block_names.push_back("IndustrialTile_" + std::string(lvl[x][y]) + ".png");
					break;
				}
			}
		}
	}

	// Iterate over the non_coll1 array to identify and create non-collidable objects.
	int starctr = 0;
	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 74; y++) {
			// Check if the current cell is not empty (i.e., not "0").
			if (non_coll[x][y] != "0") {
				// Convert the string value to an integer to determine the object type.
				switch (std::stoi(non_coll[x][y])) {
				case 102:
					// Create a Star object at the specified position, initialize it, and increment the counter.
					stars.push_back(new Star(y - 19, x - 2));
					stars[starctr]->init();
					starctr++;
					break;
				default:
					// For any other value, create a generic NonCollisionObject and store its name.
					m_non_collidable_blocks.push_back(NonCollisionObject(y - 19, x - 2, 1, 1));
					m_non_collidable_block_names.push_back("NonCollidableTile_" + std::string(non_coll[x][y]) + ".png");
					break;
				}
			}
		}
	}
}


//Initialize the level
void Level::init()
{
	if (!lvl1_finished) {
		//Level 1 Bit-Map Collidable Blocks
		const char* lvl1[20][74]{
		{ "10", "04", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05",  "05", "05",  "05",  "05",  "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "06"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "03", "03", "03", "03", "03", "03", "03", "03", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "64", "03", "03", "03", "55", "56", "57", "10", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "65", "03", "03", "03", "55", "56", "57", "01", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "65", "73", "64", "64", "34", "50", "35", "01", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "65", "73", "65", "65", "42", "69", "40", "19", "15"},
		{ "01", "13", "14", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "66", "73", "66", "66", "42", "69", "40", "19", "15"},
		{ "01", "13", "14", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "41", "41", "73", "66", "43", "32", "44", "19", "15"},
		{ "01", "13", "14", "18", "31", "32", "32", "33", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "27", "27", "27", "27", "27", "27", "27", "27", "41", "41", "73", "66", "03", "03", "03", "01", "15"},
		{ "19", "13", "14", "45", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "27", "27", "0",  "0",  "0",  "0",  "0",  "31", "32", "32", "33", "64", "04", "05", "05", "05", "05", "05", "06", "01", "15"},
		{ "01", "13", "03", "54", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "05", "05", "05", "0",  "0",  "0",  "0",  "74", "75", "75", "76", "05", "06", "0",  "0",  "0",  "0",  "0",  "0",  "61", "0",  "0",   "54", "0",   "0",   "61",  "27", "18", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "42", "65", "13", "02", "03", "11", "12", "03", "15", "01", "15"},
		{ "01", "13", "03", "54", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "17", "0",  "0",  "0",  "101","101","101","101","0",  "0",  "0",  "0",  "0",  "16", "06", "0",  "0",  "0",  "0",  "0",  "61", "0",  "0",   "54", "0",   "0",   "61",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "42", "65", "22", "23", "23", "23", "23", "23", "24", "01", "15"},
		{ "19", "13", "26", "54", "49", "50", "50", "51", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "17", "58", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "59", "16", "06", "0",  "0",  "0",  "0",  "61", "0",  "0",   "63", "0",   "0",   "61",  "0",  "0",  "72", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "49", "50", "50", "51", "66", "55", "56", "56", "56", "56", "56", "57", "01", "15"},
		{ "01", "13", "03", "63", "36", "62", "47", "11", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04", "17", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "03", "16", "05", "0",  "0",  "0",  "61", "0",  "0",   "63", "0",   "0",   "61",  "0",  "0",  "0",  "0",  "72", "72", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "56",  "56",  "56",  "56",  "56",  "01", "15"},
		{ "19", "13", "03", "63", "36", "62", "47", "11", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "110","0",  "110","0",  "110","0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "70", "101","101", "54", "101", "101", "70",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "72", "72", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "56",  "56",  "56",  "56",  "56",  "01", "15"},
		{ "19", "13", "14", "55", "56", "57", "11", "11", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "12", "11", "26", "11", "03", "14", "03", "14", "03", "14", "03", "03", "14", "03", "14", "03", "14", "14", "03", "03", "03", "14", "03", "14", "14", "14",  "54", "25",  "25",  "25",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "56",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "01", "13", "14", "55", "02", "57", "61", "71", "71", "71", "71", "71", "71", "71", "71", "71", "71", "71", "61", "34", "50", "35", "25", "25", "25", "21", "21", "41", "41", "73", "25", "73", "41", "41", "25", "73", "10", "73", "25", "41", "21", "25", "73", "73",  "54", "03",  "03",  "03",  "111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","56",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "01", "13", "03", "55", "02", "57", "61", "21", "21", "21", "21", "21", "21", "21", "21", "21", "21", "21", "61", "42", "73", "40", "21", "41", "41", "25", "21", "41", "41", "25", "21", "25", "41", "41", "25", "25", "19", "73", "73", "41", "25", "73", "25", "21",  "63", "03",  "03",  "03",  "58", "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",   "59",  "59", "59","59", "59", "59",  "59",  "60",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "01", "13", "26", "55", "02", "57", "70", "48", "48", "48", "48", "48", "48", "48", "48", "48", "48", "48", "70", "43", "32", "44", "73", "41", "41", "21", "25", "25", "25", "48", "48", "48", "73", "73", "21", "73", "28", "25", "38", "29", "20", "29", "30", "25",  "63", "03",  "03",  "03",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "28", "22", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23",  "63", "03",  "03",  "03",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "24"}

		};

		//Level 1 Bit-Map Non-Collidable Blocks
		const char* non_coll1[20][74]{
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "102", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "102", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "21", "21", "21", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "10", "11", "11", "11", "12", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "21", "0", "32", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "32", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "16", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "33", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "13", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "25", "0", "0", "0", "0", "0", "0", "102", "0", "0", "0", "0", "0", "0", "0", "26", "0", "0", "0", "0", "0", "14", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "18", "0", "0", "10", "11", "12", "0", "0", "0", "0", "0", "0", "0", "19", "20", "0", "0", "0", "6", "7", "0", "0", "0", "15", "0", "0", "31", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "22", "26", "24", "0", "23", "0", "25", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "27", "29", "28", "0", "0", "5", "6", "31", "30", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"}

		};
		
		//Call the function to make the according block and object placements
		ArrayCheck(lvl1, non_coll1);

		mn_leveldoor1->init();
		mn_keylevel1->init();

	}else {

		//Level 2 Bit-Map Collidable Blocks
		const char* lvl2[20][74]{
		{ "10", "04", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05",  "05", "05",  "05",  "05",  "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "05", "06"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "03", "03", "03", "03", "03", "03", "03", "03", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "110",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "110",  "0",  "0",  "0",  "0",  "0",   "0",  "77",   "78",   "79",   "78",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "64", "03", "03", "03", "55", "56", "57", "10", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04",  "05",  "05",  "0",  "0",  "0",  "58",  "59",  "59",  "59",  "59",  "59",  "60",  "60",  "0",  "0",  "0",  "55",  "56",  "57",  "0",  "0",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "101",  "71",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "65", "03", "03", "03", "55", "56", "57", "01", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04",  "05",  "05",  "05",  "05",  "17",  "0",  "0",  "0",  "0",  "0",  "69",  "69",  "69",  "69",  "69",  "69",  "68",  "68",  "0",  "0",  "0",  "55",  "56",  "57",  "64",  "64",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "101",  "71",  "0",  "0",  "101",  "37",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "65", "73", "64", "64", "34", "50", "35", "01", "15"},
		{ "01", "13", "03", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "04",  "17",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "68",  "68",  "0",  "0",  "0",  "0",  "0",  "0",  "65",  "65",  "0",  "0",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "101",  "37",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "65", "73", "65", "65", "42", "69", "40", "19", "15"},
		{ "01", "13", "14", "0",  "0",  "0",  "0",  "0",  "16",  "05",  "05",  "05",  "05",  "17",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "68",  "68",  "0",  "0",  "0",  "0",  "0",  "0",  "65",  "65",  "54",  "54",  "0",   "0",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "66", "73", "66", "66", "42", "69", "40", "19", "15"},
		{ "01", "13", "14", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "68",  "68",  "0",  "0",  "0",  "0",  "0",  "0",  "66",  "66",  "63",  "63",  "0",   "54",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "41", "41", "73", "66", "43", "32", "44", "19", "15"},
		{ "01", "13", "14", "18", "31", "32", "32", "33", "0",  "41",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "25",  "0",  "0",  "0",  "68",  "68",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "54",  "63",  "0",   "54",  "18",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0", "0", "0", "27", "27", "27", "27", "27", "41", "41", "73", "66", "03", "03", "03", "01", "15"},
		{ "19", "13", "14", "45", "40", "68", "68", "42", "0",  "41",  "0",  "0",  "0",  "0",  "0",  "0",  "71",  "71",  "0",  "0",  "16",  "05",  "17",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "68",  "68",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "63",  "54",  "0",   "63",  "0",   "0",   "0",   "0",  "0",  "0",  "0",  "0",  "27", "27", "0",  "0",  "0",  "0",  "0",  "31", "32", "32", "33", "64", "04", "05", "05", "05", "05", "05", "06", "01", "15"},
		{ "01", "13", "03", "54", "40", "68", "68", "42", "0",  "0",  "0",  "55",  "56",  "57",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0", "0", "0", "0", "0",  "0",  "0",  "0",  "0", "77", "68", "68", "0", "0", "0",  "0",  "0",  "0",  "0",  "0",  "54", "63",  "0",   "54", "0",   "0",   "61",  "27", "18", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "42", "65", "13", "02", "03", "11", "12", "03", "15", "01", "15"},
		{ "01", "13", "03", "54", "40", "68", "68", "42", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0", "0", "0",  "0",  "0",  "0","111","111","111","0",  "0",  "69",  "69",  "0",  "0", "0", "0",  "0",  "0",  "0",  "0",  "63", "54",  "0",   "54", "0",   "0",   "61",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "42", "65", "22", "23", "23", "23", "23", "23", "24", "01", "15"},
		{ "19", "13", "26", "54", "49", "50", "50", "51", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "110", "0", "0", "0", "0", "58", "59", "59", "59", "59", "59", "59", "59", "60", "0", "0", "0", "0", "0",  "0",  "0",  "0",  "63", "54",  "0",   "54", "0",   "0",   "61",  "0",  "0",  "72", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "49", "50", "50", "51", "66", "55", "56", "56", "56", "56", "56", "57", "01", "15"},
		{ "01", "13", "03", "63", "36", "62", "47", "11", "0",  "0",  "0",  "0",  "0",  "0",  "04",  "05",  "05",  "05",  "05", "06", "0", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0", "0", "0", "0",  "0",  "0",  "63", "54",  "0",   "63", "0",   "0",   "61",  "0",  "0",  "0",  "0",  "72", "72", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "56",  "56",  "56",  "56",  "56",  "01", "15"},
		{ "19", "13", "03", "63", "36", "62", "47", "11", "0",  "0",  "0",  "0",  "0",  "04", "17",  "25",  "25",  "25",  "25",  "15",  "0",  "0",  "0",  "0",  "101",  "101","101",  "101","0",  "0",	"101",  "0",  "0",  "101",  "101",  "101",  "101",  "101",  "101",  "101",  "101",  "54", "63","101", "54", "101", "101", "70",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "72", "72", "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "56",  "56",  "56",  "56",  "56",  "01", "15"},
		{ "19", "13", "14", "55", "56", "57", "11", "11", "05", "05", "05", "05", "05", "17", "22", "23", "23", "23", "23", "24", "26", "11", "03", "14", "03", "14", "03", "14", "03", "03", "14", "03", "14", "03", "14", "14", "03", "03", "03", "14", "03", "14", "14", "14",  "54", "25",  "25",  "25",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "0",  "56",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "01", "13", "14", "55", "02", "57", "61", "71", "71", "71", "71", "71", "71", "71", "71", "71", "71", "71", "61", "34", "50", "35", "25", "25", "25", "21", "21", "41", "41", "73", "25", "73", "41", "41", "25", "73", "10", "73", "25", "41", "21", "25", "73", "73",  "54", "03",  "03",  "03",  "111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","111","56",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "01", "13", "03", "55", "02", "57", "61", "21", "21", "21", "21", "21", "21", "21", "21", "21", "21", "21", "61", "42", "73", "40", "21", "41", "41", "25", "21", "41", "41", "25", "21", "25", "41", "41", "25", "25", "19", "73", "73", "41", "25", "73", "25", "21",  "63", "03",  "03",  "03",  "58", "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",  "59",   "59",  "59", "59","59", "59", "59",  "59",  "60",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "01", "13", "26", "55", "02", "57", "70", "48", "48", "48", "48", "48", "48", "48", "48", "48", "48", "48", "70", "43", "32", "44", "73", "41", "41", "21", "25", "25", "25", "48", "48", "48", "73", "73", "21", "73", "28", "25", "38", "29", "20", "29", "30", "25",  "63", "03",  "03",  "03",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "15"},
		{ "28", "22", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23", "23",  "63", "03",  "03",  "03",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "56",  "24"}

		};

		//Level 2 Bit-Map Non-Collidable Blocks
		const char* non_coll2[20][74]{
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "21", "21", "0", "0", "0", "0", "0", "0", "0", "0", "0", "32", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "102", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "27", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "19", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "102", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "18", "0", "0", "0", "0", "0", "0", "0", "30", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "18", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "18", "10", "11", "12", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "102", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "21", "21", "21", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "21", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "10", "11", "11", "11", "12", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "32", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "16", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "33", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "13", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "14", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "18", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "19", "20", "0", "0", "0", "6", "7", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "22", "26", "24", "0", "23", "0", "25", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "27", "29", "28", "0", "0", "5", "6", "31", "30", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"},
		{ "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"}
		};

		//Call the function to make the according block and object placements
		ArrayCheck(lvl2, non_coll2);

		mn_leveldoor1->init();
		mn_keylevel1->init();
	}
	
	

	m_block_brush.outline_opacity = 0.0f;
	m_block_brush_debug.fill_opacity = 0.1f;
	SETCOLOR(m_block_brush_debug.fill_color, 0.1f, 1.0f, 0.1f);
	SETCOLOR(m_block_brush_debug.outline_color, 0.3f, 1.0f, 0.2f);
	
	spitesinit();

}

//Initalize the sprites 
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

	m_spritesstarsystem.push_back(m_state->getFullAssetPath("emptystars.png"));
	m_spritesstarsystem.push_back(m_state->getFullAssetPath("onestar.png"));
	m_spritesstarsystem.push_back(m_state->getFullAssetPath("twostars.png"));
	m_spritesstarsystem.push_back(m_state->getFullAssetPath("fullstars.png"));

	m_spriteskeycollected.push_back(m_state->getFullAssetPath("NotCollectedKey.png"));
	m_spriteskeycollected.push_back(m_state->getFullAssetPath("CollectedKey.png"));
}
