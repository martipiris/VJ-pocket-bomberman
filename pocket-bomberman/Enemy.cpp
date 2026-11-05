#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() {
	if (sprite != nullptr)
		delete sprite;
}

void Enemy::init(const glm::ivec2& tileMapPos, TileMap* tileMap, Player* player, ShaderProgram& shaderProgram) {
	map = tileMap;
	this->player = player;
	texProgram = shaderProgram;
	
	posEnemy = tileMapPos;
	init();
	sprite->setPosition(posEnemy);
}

void Enemy::update(int deltaTime) {
	sprite->update(deltaTime);

	updateMovement();
	sprite->setPosition(posEnemy);
}

void Enemy::render(const glm::mat4& view) {
	if (sprite) sprite->render(view);
}

glm::vec2 Enemy::getPosition() const {
	return glm::ivec2(posEnemy.x, posEnemy.y);
}

glm::ivec2 Enemy::getCollisionSize() const {
	return enemyCollision;
}

glm::ivec2 Enemy::getCollisionOffset() const {
	return enemyCollisionOffset;
}






