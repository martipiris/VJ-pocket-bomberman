#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	if (sprite != nullptr)
		delete sprite;
}

void Enemy::init(const glm::ivec2& tileMapPos, TileMap* tileMap, ShaderProgram& shaderProgram) {
	mapDispl = tileMapPos;
	map = tileMap;
	texProgram = shaderProgram;
	init();
}

void Enemy::update(int deltaTime) {
	sprite->update(deltaTime);

	updateMovement();
	sprite->setPosition(glm::ivec2(mapDispl.x + posEnemy.x, mapDispl.y + posEnemy.y));
}

void Enemy::render(const glm::mat4& view) {
	if (sprite) sprite->render(view);
}

void Enemy::setPosition(const glm::vec2& pos) {
	posEnemy = pos;
	sprite->setPosition(posEnemy);
}

glm::vec2 Enemy::getPosition() const {
	return glm::ivec2(mapDispl.x + posEnemy.x, mapDispl.y + posEnemy.y);
}







