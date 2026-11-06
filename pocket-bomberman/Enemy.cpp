#include "Enemy.h"
#include "Enemy.h"
#include "BombManager.h"

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

void Enemy::update(int deltaTime, bool isGameOver) {
	sprite->update(deltaTime);
	//If the enemy has been hit by a bomb it does not move
	if (exploding) {
		explosionTimer++;
		updateWhileExploding(deltaTime);
	} 
	
	if (!isGameOver) {
		updateMovement();
		sprite->setPosition(posEnemy);
	}
}

void Enemy::render(const glm::mat4& view) {
	sprite->render(view);
}

bool Enemy::hitsPlayer() const {
	return false;
}

 //See if explosion has hit an enemy and begin death process if so
bool Enemy::onBombHit(const BombManager& bombs) {
	const glm::ivec2 hitPosition = posEnemy + getCollisionOffset();	
	if (!exploding && bombs.hits(hitPosition, getCollisionSize())) {
		
		startExploding();
	}

	return exploding && explosionTimer >= 30;
}

 //Enemy begin of death process after being hit by explosion
void Enemy::startExploding() {
	if (exploding)
		return;
	exploding = true;
	explosionTimer = 0;
	sprite->changeAnimation(0);
	Game::instance().audio().playSound("hit_enemy");
}

void Enemy::updateWhileExploding(int deltaTime) {
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






