#include "Tree.h"

Tree::Tree() : Enemy(), attackTimer(0), attackCooldown(0), returnTimer(0) {}

Tree::~Tree() = default;

enum TreeAnims {
	IDLE, ATTACK, EXPLODE
};

enum TreeState {
	WANDER, RETURNING, SPAWNING_LEAVES, SHOOTING_POLLEN, DROPPING_BOMBS
};

void Tree::init() {
	spritesheet.loadFromFile("images/tree.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(enemySize, glm::vec2(0.25f, 1.f), &spritesheet, &texProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(IDLE, 2);
	sprite->addKeyframe(IDLE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(IDLE, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(ATTACK, 1);
	sprite->addKeyframe(ATTACK, glm::vec2(0.5f, 0.f));

	sprite->setAnimationSpeed(EXPLODE, 1);
	sprite->addKeyframe(EXPLODE, glm::vec2(0.75f, 0.f));


	sprite->changeAnimation(IDLE);

	state = WANDER;
	timer = 0;
	attackTimer = 0;
	attackCooldown = 0;
	returnTimer = 0;
	initialPos = posEnemy;
}

void Tree::updateMovement() {
	int tileSize = map->getTileSize();
	glm::ivec2 mapSize = map->getMapSize();
	int mapMargin = tileSize; // 1 tile margin from edges

	// Clamp tree to bounds with margin
	int minX = mapMargin;
	int maxX = (mapSize.x * tileSize) - enemySize.x - mapMargin;
	int minY = mapMargin;
	int maxY = (mapSize.y * tileSize) - enemySize.y - mapMargin;

	// Update attack cooldown
	if (attackCooldown > 0) {
		--attackCooldown;
	}

	if (state == WANDER) {
		// Wander towards player in both X and Y
		glm::vec2 playerPos = player->getPosition();
		int treeCenterX = posEnemy.x + enemySize.x / 2;
		int treeCenterY = posEnemy.y + enemySize.y / 2;
		int playerCenterX = int(playerPos.x) + 32; // Player size is 64x64
		int playerCenterY = int(playerPos.y) + 32;

		// Move towards player X
		if (treeCenterX < playerCenterX) {
			posEnemy.x += 1;
		} else if (treeCenterX > playerCenterX) {
			posEnemy.x -= 1;
		}

		// Move towards player Y
		if (treeCenterY < playerCenterY) {
			posEnemy.y += 1;
		} else if (treeCenterY > playerCenterY) {
			posEnemy.y -= 1;
		}

		// Keep within bounds
		if (posEnemy.x < minX) posEnemy.x = minX;
		if (posEnemy.x > maxX) posEnemy.x = maxX;
		if (posEnemy.y < minY) posEnemy.y = minY;
		if (posEnemy.y > maxY) posEnemy.y = maxY;

		++timer;

		// Decide to attack after some time
		if (timer > 360 && attackCooldown == 0) {
			// Before attacking, need to return to initial position
			targetPos = initialPos;
			state = RETURNING;
			returnTimer = 0;
			timer = 0;
		}
	}
	else if (state == RETURNING) {
		// Move towards initial position X
		if (posEnemy.x < initialPos.x) {
			posEnemy.x += 2; // Faster speed
		} else if (posEnemy.x > initialPos.x) {
			posEnemy.x -= 2; // Faster speed
		}

		// Move towards initial position Y
		if (posEnemy.y < initialPos.y) {
			posEnemy.y += 2; // Faster speed
		} else if (posEnemy.y > initialPos.y) {
			posEnemy.y -= 2; // Faster speed
		}

		// Keep within bounds
		if (posEnemy.x < minX) posEnemy.x = minX;
		if (posEnemy.x > maxX) posEnemy.x = maxX;
		if (posEnemy.y < minY) posEnemy.y = minY;
		if (posEnemy.y > maxY) posEnemy.y = maxY;

		// Check if reached initial position (within 4 pixels tolerance)
		if (abs(posEnemy.x - initialPos.x) <= 4 && abs(posEnemy.y - initialPos.y) <= 4) {
			// Reached initial position, execute attack
			int attackType = (timer / 60) % 4;

			switch (attackType) {
			case 0: // Leaf attack
				sprite->changeAnimation(ATTACK);
				state = SPAWNING_LEAVES;
				break;
			case 1: // Pollen attack
				sprite->changeAnimation(ATTACK);
				state = SHOOTING_POLLEN;
				break;
			case 2: // Bomb rain attack
				sprite->changeAnimation(ATTACK);
				state = DROPPING_BOMBS;
				break;
			default: // Return to wander
				sprite->changeAnimation(IDLE);
				state = WANDER;
				break;
			}

			attackTimer = 0;
			attackCooldown = 240; // Cooldown before next attack
			timer = 0;
		}

		++returnTimer;
	}
	// Attack states - return to wander after animation completes
	else if (state == SPAWNING_LEAVES || state == SHOOTING_POLLEN || state == DROPPING_BOMBS) {
		++attackTimer;

		if (attackTimer > 180) {
			sprite->changeAnimation(IDLE);
			state = WANDER;
			timer = 0;
		}
	}

	printf("Tree State: %d, Attack Timer: %d, Attack Cooldown: %d, Position: (%d,%d), Target position:(%d,%d)\n", state, attackTimer, attackCooldown, posEnemy.x, posEnemy.y, targetPos.x, targetPos.y);
}

glm::ivec2 Tree::getCollisionSize() const {
	return enemyCollision;
}

glm::ivec2 Tree::getCollisionOffset() const {
	return enemyCollisionOffset;
}