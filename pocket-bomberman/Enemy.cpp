#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	if (sprite != nullptr)
		delete sprite;
}

void Enemy::update(int deltaTime) {
	sprite->update(deltaTime);
	updateMovement();
}

void Enemy::render(const glm::mat4& view) {
	if (sprite) sprite->render(view);
}

void Enemy::setPosition(const glm::vec2& pos) {
	posEnemy = pos;
	sprite->setPosition(posEnemy);
}

glm::vec2 Enemy::getPosition() const {
	return posEnemy;
}







