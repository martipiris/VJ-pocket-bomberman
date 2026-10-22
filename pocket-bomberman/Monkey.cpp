#include "Monkey.h"

Monkey::Monkey() : Enemy() {}

enum monkeyAnims {
	IDLE, EXPLODE
};

void Monkey::init(const glm::ivec2& initPos, ShaderProgram& shaderProgram) {
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(SIZE_ENEMY, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	/* sprite->setAnimationSpeed(STAND_LEFT, 8);
	sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(MOVE_LEFT, 8);
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.2f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.4f, 0.f));
	sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));

	sprite->setAnimationSpeed(JUMP_LEFT, 8);
	sprite->addKeyframe(JUMP_LEFT, glm::vec2(0.6f, 0.f));

	sprite->setAnimationSpeed(FALL_LEFT, 8);
	sprite->addKeyframe(FALL_LEFT, glm::vec2(0.8f, 0.f));


	sprite->setAnimationSpeed(STAND_RIGHT, 8);
	sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.f, 0.2f));

	sprite->setAnimationSpeed(MOVE_RIGHT, 8);
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.2f, 0.2f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.0f, 0.2f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.4f, 0.2f));
	sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.2f));

	sprite->setAnimationSpeed(JUMP_RIGHT, 8);
	sprite->addKeyframe(JUMP_RIGHT, glm::vec2(0.6f, 0.2f));

	sprite->setAnimationSpeed(FALL_RIGHT, 8);
	sprite->addKeyframe(FALL_RIGHT, glm::vec2(0.8f, 0.2f));

	sprite->changeAnimation(1);*/

	sprite->setPosition(initPos); 
}

void Monkey::updateMovement() {
}