#include "Monkey.h"

Monkey::Monkey() : Enemy() {}

Monkey::~Monkey() = default;

enum monkeyAnims {
	MOVE, EXPLODE
};

void Monkey::init(const glm::ivec2& initPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/monkey.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(SIZE_ENEMY, glm::vec2(0.2f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(MOVE, 3);
	sprite->addKeyframe(MOVE, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE, glm::vec2(0.2f, 0.f));

	sprite->setAnimationSpeed(EXPLODE, 3);
	sprite->addKeyframe(EXPLODE, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(EXPLODE, glm::vec2(0.6f, 0.f));
	sprite->addKeyframe(EXPLODE, glm::vec2(0.8f, 0.f));

	sprite->changeAnimation(MOVE);
	sprite->setPosition(initPos); 
}

void Monkey::updateMovement() {
	posEnemy.x += 1;

	sprite->setPosition(posEnemy);
}