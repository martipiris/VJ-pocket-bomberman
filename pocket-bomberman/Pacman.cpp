#include "Pacman.h"

Pacman::Pacman() : Enemy() {}

Pacman::~Pacman() = default;

enum PacmanAnims {
	MOVE_LEFT, DASH_LEFT,
	MOVE_RIGHT, DASH_RIGHT,
	EXPLODE
};

void Pacman::init() {
	spritesheet.loadFromFile("images/pacman.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(enemySize, glm::vec2(0.25f, 0.25f), &spritesheet, &texProgram);
	sprite->setNumberAnimations(5);

	sprite->setAnimationSpeed(MOVE_LEFT, 3);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(DASH_LEFT, 6);
	sprite->addKeyframe(DASH_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(DASH_LEFT, glm::vec2(0.25f, 0.f));
	sprite->addKeyframe(DASH_LEFT, glm::vec2(0.5f, 0.f));
	sprite->addKeyframe(DASH_LEFT, glm::vec2(0.25f, 0.f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 3);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(DASH_RIGHT, 6);
	sprite->addKeyframe(DASH_RIGHT, glm::vec2(0.f, 0.25f));
	sprite->addKeyframe(DASH_RIGHT, glm::vec2(0.25f, 0.25f));
	sprite->addKeyframe(DASH_RIGHT, glm::vec2(0.5f, 0.25f));
	sprite->addKeyframe(DASH_RIGHT, glm::vec2(0.25f, 0.25f));

	sprite->setAnimationSpeed(EXPLODE, 3);
	sprite->addKeyframe(EXPLODE, glm::vec2(0.0f, 0.5f));
	sprite->addKeyframe(EXPLODE, glm::vec2(0.25f, 0.5f));
	sprite->addKeyframe(EXPLODE, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(MOVE_RIGHT);

	state = MOVE_RIGHT;
	timer = 0;
}

void Pacman::updateMovement() {
	int tileSize = map->getTileSize();
	
	int pacmanTileY = posEnemy.y / tileSize;
	int playerTileY = int(player->getPosition().y) / tileSize;

	int previousState = state;
	if (pacmanTileY == playerTileY) {
		state = (player->getPosition().x < posEnemy.x) ? DASH_LEFT : DASH_RIGHT;
	} else {
		state = (state == MOVE_LEFT || state == DASH_LEFT) ? MOVE_LEFT : MOVE_RIGHT;
	}
	
	if (previousState != state) {
		sprite->changeAnimation(state);
	}

	if (timer > 0) {
		++timer;
		if (timer > 30) {
			timer = 0;
			state = (state == MOVE_LEFT || state == DASH_LEFT) ? MOVE_RIGHT : MOVE_LEFT;
			sprite->changeAnimation(state);
		}
	}
	else {
		int speedMultiplier = (state == DASH_LEFT || state == DASH_RIGHT) ? 2 : 1;
		
		switch (state) {
		case MOVE_RIGHT:
		case DASH_RIGHT:
			if (!map->isTileSolid(glm::ivec2(posEnemy.x + tileSize, posEnemy.y)) &&
				map->isTileSolid(glm::ivec2(posEnemy.x + tileSize, posEnemy.y + tileSize))) {
				posEnemy.x += speedMultiplier;
			}
			else {
				timer = 1;
			}
			break;
		case MOVE_LEFT:
		case DASH_LEFT:
			if (!map->isTileSolid(glm::ivec2(posEnemy.x - 1, posEnemy.y)) &&
				map->isTileSolid(glm::ivec2(posEnemy.x - 1, posEnemy.y + tileSize))) {
				posEnemy.x -= speedMultiplier;
			}
			else {
				timer = 1;
			}
			break;
		}
	}
}