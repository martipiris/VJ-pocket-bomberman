#include <iostream>
#include <cmath>
#include <GL/glew.h>
#include "Player.h"
#include "Game.h"

enum PlayerAnims {
	STAND_LEFT, MOVE_LEFT, JUMP_LEFT, FALL_LEFT, 
	STAND_RIGHT, MOVE_RIGHT, JUMP_RIGHT, FALL_RIGHT,
	WIN, DIE, EXPLODE, 
};

Player::Player()
: posPlayerF(0.f, 0.f)
, tileMapDispl(0,0)
, posPlayer(0,0)
, velY(0.f)
, onGround(false)
, sprite(nullptr)
, map(nullptr)
{
}

Player::~Player() {
	if (sprite != nullptr)
		delete sprite;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
	velY = 0.f;
	onGround = false;
	spritesheet.loadFromFile("images/player.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(SIZE_PLAYER, glm::vec2(0.2f, 0.2f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(12);

	sprite->setAnimationSpeed(STAND_LEFT, 8);
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

	sprite->changeAnimation(1);
	tileMapDispl = tileMapPos;
	// posPlayerF se ajusta desde setPosition() que llama LevelScene después de init
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::update(int deltaTime) {
	const float dt = float(deltaTime) / 16.0f;

	sprite->update(deltaTime);

	if (Game::instance().getKey(GLFW_KEY_LEFT)) {
		if (onGround) {
			if (sprite->animation() != MOVE_LEFT) sprite->changeAnimation(MOVE_LEFT);
		} else {
			if (sprite->animation() != JUMP_LEFT && sprite->animation() != FALL_LEFT) sprite->changeAnimation(velY < 0 ? JUMP_LEFT : FALL_LEFT);
		}
		posPlayerF.x -= horizSpeed * dt;
	} else if (Game::instance().getKey(GLFW_KEY_RIGHT)) {
		if (sprite->animation() != MOVE_RIGHT) sprite->changeAnimation(MOVE_RIGHT);
		posPlayerF.x += horizSpeed * dt;
	} else {
		if (sprite->animation() == MOVE_LEFT) sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT) sprite->changeAnimation(STAND_RIGHT);
	}

	glm::ivec2 candidateH = glm::ivec2(int(posPlayerF.x), int(posPlayerF.y));
	if (posPlayerF.x < posPlayer.x && map->collisionMoveLeft(candidateH, SIZE_PLAYER)) {
		posPlayerF.x = float(posPlayer.x);
	}
	if (posPlayerF.x > posPlayer.x && map->collisionMoveRight(candidateH, SIZE_PLAYER)) {
		posPlayerF.x = float(posPlayer.x);
	}

	if (onGround && Game::instance().getKey(GLFW_KEY_UP)) {
		velY = -jumpSpeed;
		onGround = false;

		if (sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
			sprite->changeAnimation(JUMP_LEFT);
		} else {
			sprite->changeAnimation(JUMP_RIGHT);
		}
	}

	velY += gravity * dt;

	if (velY > maxFallSpeed)
		velY = maxFallSpeed;

	if (velY > 0.f && !onGround) {
		if (sprite->animation() == JUMP_LEFT || sprite->animation() == MOVE_LEFT || sprite->animation() == STAND_LEFT) {
			sprite->changeAnimation(FALL_LEFT);
		} else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == MOVE_RIGHT || sprite->animation() == STAND_RIGHT) {
			sprite->changeAnimation(FALL_RIGHT);
		}
	}

	const float verticalMovement = velY * dt;
	const int steps = std::max(1, static_cast<int>(std::ceil(std::abs(verticalMovement))));
	const float verticalStep = verticalMovement / float(steps);

	bool collidedVertically = false;

	for (int i = 0; i < steps; ++i)	{
		const float nextY = posPlayerF.y + verticalStep;
		glm::ivec2 candidate(
			static_cast<int>(posPlayerF.x),
			static_cast<int>(nextY));

		if (verticalStep > 0.f) {
			int correctedY = candidate.y;

			if (map->collisionMoveDown(candidate, SIZE_PLAYER, &correctedY)) {
				posPlayerF.y = static_cast<float>(correctedY);
				velY = 0.f;
				onGround = true;
				collidedVertically = true;

				if (sprite->animation() == JUMP_LEFT || sprite->animation() == FALL_LEFT) sprite->changeAnimation(STAND_LEFT);
				else if (sprite->animation() == JUMP_RIGHT || sprite->animation() == FALL_RIGHT) sprite->changeAnimation(STAND_RIGHT);
				
				break;
			}
		} else if (verticalStep < 0.f) {
			int correctedY = candidate.y;

			if (map->collisionMoveUp(candidate, SIZE_PLAYER, &correctedY)) {
				posPlayerF.y = static_cast<float>(correctedY);
				velY = 0.f;
				onGround = false;
				collidedVertically = true;
				break;
			}
		}

		posPlayerF.y = nextY;
	}

	if (!collidedVertically) {
		onGround = false;
	}

	posPlayer = glm::ivec2(int(posPlayerF.x), int(posPlayerF.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render(const glm::mat4 &view) {
	if (sprite) sprite->render(view);
}

void Player::setTileMap(TileMap *tileMap) {
	map = tileMap;
}

void Player::setPosition(const glm::vec2 &pos) {
	posPlayerF = pos;
	posPlayer = glm::ivec2(int(posPlayerF.x), int(posPlayerF.y));
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::vec2 Player::getPosition() const {
	return glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y));
}







