#include "Monkey.h"

Monkey::Monkey() : Enemy() {}

Monkey::~Monkey() = default;

enum monkeyAnims {
	MOVE, EXPLODE
};

enum monkeyState {
	MOVE_LEFT, MOVE_RIGHT
};

void Monkey::init() {
	spritesheet.loadFromFile("images/monkey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(SIZE_ENEMY, glm::vec2(0.2f, 1.f), &spritesheet, &texProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 3);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(0.2f, 0.f));

	sprite->setAnimationSpeed(EXPLODE, 3);
	sprite->addKeyframe(EXPLODE, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(EXPLODE, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(EXPLODE, glm::vec2(0.8f, 0.f));

	sprite->changeAnimation(MOVE);
	sprite->setPosition(mapDispl);

	state = MOVE_RIGHT;
	timer = 0;
}

void Monkey::updateMovement() {
	int tileSize = map->getTileSize();

	if (timer > 0) {
		++timer;
		if (timer > 180) {
			timer = 0;
			state = (state == MOVE_LEFT) ? MOVE_RIGHT : MOVE_LEFT;
		}
	} else {
		switch (state) {
		case MOVE_RIGHT:
			if (!map->isTileSolid(glm::ivec2(mapDispl.x + posEnemy.x + tileSize, mapDispl.y + posEnemy.y)) &&
				map->isTileSolid(glm::ivec2(mapDispl.x + posEnemy.x + tileSize, mapDispl.y + posEnemy.y + tileSize))) {
				posEnemy.x += 1;
			}
			else {
				timer = 1;
			}
			break;
		case MOVE_LEFT:
			if (!map->isTileSolid(glm::ivec2(mapDispl.x + posEnemy.x - 1, mapDispl.y + posEnemy.y)) &&
				map->isTileSolid(glm::ivec2(mapDispl.x + posEnemy.x - 1, mapDispl.y + posEnemy.y + tileSize))) {
				posEnemy.x -= 1;
			}
			else {
				timer = 1;
			}
			break;
		}
	}
}