#include "Tree.h"
#include <algorithm>
#include <cstdlib>
#include "Tree.h"
#include "BombManager.h"

Tree::Tree()
	: Enemy(), attackTimer(0), attackCooldown(0), returnTimer(0), attackRound(0),
      hitsRemaining(5), hitFlashTimer(0), deathTimer(0), petalSpawnTimer(0), dying(false) {}

Tree::~Tree() = default;

enum TreeAnims {
	IDLE, ATTACK, EXPLODE
};

enum TreeState {
   WANDER, RETURNING, ATTACKING
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
	attackRound = 0;
  hitsRemaining = 5;
	hitFlashTimer = 0;
	deathTimer = 0;
  petalSpawnTimer = 0;
	dying = false;
	pollen.clear();
	petals.clear();
	leaves.clear();
	initialPos = posEnemy;
}

bool Tree::onBombHit(const BombManager& bombs) {
	for (auto it = leaves.begin(); it != leaves.end();) {
		if ((*it)->onBombHit(bombs))
			it = leaves.erase(it);
		else
			++it;
	}

	if (dying)
       return deathTimer >= 5000;
	if (hitFlashTimer == 0 && bombs.hits(posEnemy + getCollisionOffset(), getCollisionSize())) {
		--hitsRemaining;
        hitFlashTimer = 2000;
	}
	return false;
}

void Tree::updateMovement() {
  const int tileSize = map->getTileSize();
	const glm::ivec2 mapPixels = map->getMapPixelSize();
	const int minX = tileSize;
	const int maxX = mapPixels.x - enemySize.x - tileSize;
	const int minY = tileSize;
	const int maxY = mapPixels.y - enemySize.y - tileSize;

	for (auto it = pollen.begin(); it != pollen.end();) {
		if (!(*it)->update(16, mapPixels))
			it = pollen.erase(it);
		else
			++it;
	}
	for (auto& leaf : leaves)
		leaf->update(16);
  for (auto it = petals.begin(); it != petals.end();) {
		if (!(*it)->update(lastDeltaTime, mapPixels))
			it = petals.erase(it);
		else
			++it;
	}

	if (dying) {
       deathTimer += lastDeltaTime;
		petalSpawnTimer += lastDeltaTime;
		if (petalSpawnTimer >= 200) {
			petalSpawnTimer -= 200;
			spawnPetal();
     }
		return;
	}

	if (attackCooldown > 0)
		--attackCooldown;

	if (hitFlashTimer > 0) {
        hitFlashTimer = std::max(0, hitFlashTimer - lastDeltaTime);
		if (hitsRemaining == 0) {
			beginDeath();
		}
		return;
	}

	if (state == WANDER) {
		const glm::vec2 playerPos = player->getPosition();
		const int treeCenterX = posEnemy.x + enemySize.x / 2;
		const int treeCenterY = posEnemy.y + enemySize.y / 2;
		const int playerCenterX = int(playerPos.x) + SIZE_PLAYER.x / 2;
		const int playerCenterY = int(playerPos.y) + SIZE_PLAYER.y / 2;
		posEnemy.x += (treeCenterX < playerCenterX) ? 1 : (treeCenterX > playerCenterX ? -1 : 0);
		posEnemy.y += (treeCenterY < playerCenterY) ? 1 : (treeCenterY > playerCenterY ? -1 : 0);
		posEnemy.x = std::max(minX, std::min(maxX, posEnemy.x));
		posEnemy.y = std::max(minY, std::min(maxY, posEnemy.y));

		if (++timer > 360 && attackCooldown == 0) {
			targetPos = initialPos;
			state = RETURNING;
			returnTimer = 0;
			timer = 0;
		}
	}
	else if (state == RETURNING) {
		if (posEnemy.x < initialPos.x) posEnemy.x = std::min(initialPos.x, posEnemy.x + 2);
		else if (posEnemy.x > initialPos.x) posEnemy.x = std::max(initialPos.x, posEnemy.x - 2);
		if (posEnemy.y < initialPos.y) posEnemy.y = std::min(initialPos.y, posEnemy.y + 2);
		else if (posEnemy.y > initialPos.y) posEnemy.y = std::max(initialPos.y, posEnemy.y - 2);
		posEnemy.x = std::max(minX, std::min(maxX, posEnemy.x));
		posEnemy.y = std::max(minY, std::min(maxY, posEnemy.y));

		if (abs(posEnemy.x - initialPos.x) <= 4 && abs(posEnemy.y - initialPos.y) <= 4) {
			posEnemy = initialPos;
			state = ATTACKING;
			attackRound = 0;
			attackTimer = 0;
			sprite->changeAnimation(ATTACK);
			startPollenRound();
		}
		++returnTimer;
	}
	else if (state == ATTACKING && ++attackTimer >= 30) {
		attackTimer = 0;
		++attackRound;
		if (attackRound == 1) {
			startPollenRound();
		}
		else if (attackRound == 2) {
			startLeafRound();
		}
		else {
			state = WANDER;
			attackCooldown = 240;
			timer = 0;
			sprite->changeAnimation(IDLE);
		}
	}
}

void Tree::startPollenRound() {
	const glm::ivec2 mapPixels = map->getMapPixelSize();
	const glm::vec2 center(mapPixels.x / 2.f - 8.f, mapPixels.y / 2.f - 4.f);
	const glm::vec2 directions[] = {
		glm::vec2(-1.f, -1.f), glm::vec2(1.f, -1.f),
		glm::vec2(-1.f, 1.f), glm::vec2(1.f, 1.f)
	};
	for (const glm::vec2& direction : directions)
		pollen.push_back(std::make_unique<Pollen>(center, direction, texProgram));
}

void Tree::startLeafRound() {
  if (!leaves.empty())
		return;
	for (int direction = -1; direction <= 1; direction += 2) {
		auto leaf = std::make_unique<LeafEnemy>(direction);
     leaf->Enemy::init(posEnemy, map, player, texProgram);
		leaves.push_back(std::move(leaf));
	}
}

void Tree::beginDeath() {
	dying = true;
	deathTimer = 0;
	petalSpawnTimer = 0;
	pollen.clear();
	sprite->changeAnimation(EXPLODE);
	for (auto& leaf : leaves)
		leaf->startExploding();
	spawnPetal();
}

void Tree::spawnPetal() {
	const glm::ivec2 mapPixels = map->getMapPixelSize();
	const int x = std::max(0, std::min(mapPixels.x - 32,
		posEnemy.x - 32 + std::rand() % (enemySize.x + 64)));
	const int y = std::max(0, std::min(mapPixels.y - 32,
		posEnemy.y - 32 + std::rand() % (enemySize.y + 64)));
   petals.push_back(std::make_unique<Pollen>(glm::vec2(x, y), glm::vec2(0.f, 0.f), texProgram, 1500));
}

void Tree::render(const glm::mat4& view) {
    if (dying || hitFlashTimer == 0 || (hitFlashTimer / 80) % 2 != 0)
		Enemy::render(view);
	for (const auto& bullet : pollen)
		bullet->render(view);
	for (const auto& leaf : leaves)
		leaf->render(view);
   for (const auto& petal : petals)
		petal->render(view);
}

bool Tree::hitsPlayer() const {
	const glm::ivec2 playerPosition(player->getPosition());
	for (const auto& bullet : pollen) {
		if (bullet->hitsPlayer(playerPosition))
			return true;
	}
	for (const auto& leaf : leaves) {
		if (leaf->hitsPlayer())
			return true;
	}
	return false;
}

glm::ivec2 Tree::getCollisionSize() const {
	return enemyCollision;
}

glm::ivec2 Tree::getCollisionOffset() const {
	return enemyCollisionOffset;
}