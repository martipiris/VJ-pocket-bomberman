#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelScene.h"
#include "StartingScene.h"
#include <algorithm>

LevelScene::LevelScene(const std::string &levelFile)
	: m_levelFile(levelFile), map(nullptr)
{
}

LevelScene::~LevelScene()
{
	deleteObjects();
}

void LevelScene::initObjects()
{
	const std::string file = getLevelFile();
	const glm::vec2 minCoords = glm::vec2(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
	map = TileMap::createTileMap(file, minCoords, texProgram);

	bombs = new BombManager(map, texProgram);
	powerUps = new PowerUpManager(map, texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_OFFSET_X, SCREEN_OFFSET_Y), texProgram);
	player->setPosition(getInitialPlayerTilePos());
	player->setTileMap(map);

	cam = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, map->getMapPixelSize(), player->getPosition());
	gui = new GUI(simpleProgram, texProgram);
	gui->init();

	onLevelInit();
}

void LevelScene::updateObjects(int deltaTime, int currentTime)
{
	if (enemyPlayerCollision()) {
		hurtPlayer();
	}

	if (isGameOver) {
		if (gameOverTimer > 0) {
			--gameOverTimer;
		} else {
			if (Stats::getInstance().getLives() == 0) {
				Game::instance().changeScene(STARTING);
			} else {
				Stats::getInstance().decreaseLives();
				Game::instance().changeScene(id());
			}
			return;
		}
	}


	if (gui->updateTimer(std::max(0, int(timer - currentTime / 1000))) == 0 && !isGameOver) {
		isGameOver = true;
		player->setDead();
	}

	gui->setEnemyCount(enemies.size());

	player->update(deltaTime);

	const glm::vec2 playerPos = player->getPosition();
	const glm::ivec2 boxPos = glm::ivec2(playerPos) + PLAYER_COLLISION_OFFSET;
	const glm::ivec2 center(boxPos.x + SIZE_PLAYER_COLLISION.x / 2, boxPos.y + SIZE_PLAYER_COLLISION.y / 2);

	if (!isGameOver) bombs->update(deltaTime, boxPos, center, SIZE_PLAYER_COLLISION);

	// Destroyed tiles may drop a power-up
	for (const glm::ivec2& t : bombs->takeDestroyedTiles())
		powerUps->maybeSpawn(t);

	// Pickups
	if (!isGameOver) {
		Stats& stats = Stats::getInstance();
		for (PowerUpType type : powerUps->collect(boxPos, SIZE_PLAYER_COLLISION)) {
			switch (type) {
			case POWERUP_BOMB:  stats.increaseBombAmount(); break;
			case POWERUP_FIRE:  stats.increaseBombRange();  break;
			case POWERUP_SPEED: stats.increaseSpeed();      break;
			case POWERUP_STAR:  player->setInvincible(10000); break;   // 10 s
			case POWERUP_HEART: stats.setHeart(true); break;
			default: break;
			}
		}
	}

	// Explosion kills the player
	if (bombs->hits(boxPos, SIZE_PLAYER_COLLISION)) {
		hurtPlayer();
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[&](const std::unique_ptr<Enemy>& e) {
          return e->onBombHit(*bombs);
		}), enemies.end());

	// All enemies dead -> open the door
	if (!isGameOver && !doorOpened && enemies.empty()) {
		map->openDoor();
		doorOpened = true;
	}

	// Touching the open door -> next level
	if (!isGameOver && map->touchesOpenDoor(boxPos, SIZE_PLAYER_COLLISION)) {
		Game::instance().changeScene(nextLevelId());
		return;   // the scene was just deleted, don't touch members after this
	}

	cam->setTarget(player->getPosition());
	cam->update();

	for (auto& enemy : enemies) {
		enemy->update(deltaTime);
	}

	onLevelUpdate(deltaTime);
}

void LevelScene::renderObjects()
{
	glm::mat4 view = cam->getViewMatrix();
	texProgram.setUniformMatrix4f("modelview", view);
	// simpleProgram.setUniformMatrix4f("modelview", view);

	map->render();
	powerUps->render(view);
	bombs->render(view);
	player->render(view);

	for (auto& enemy : enemies) {
		enemy->render(view);
	}

	gui->render();

	onLevelRender();
}

void LevelScene::deleteObjects() {
	delete powerUps; powerUps = nullptr;
	delete bombs;  bombs = nullptr;
	delete map;    map = nullptr;
	delete gui;    gui = nullptr;
	delete player; player = nullptr;
	delete cam;    cam = nullptr;
}

std::string LevelScene::getLevelFile() const {
	return m_levelFile;
}

glm::vec2 LevelScene::getInitialPlayerTilePos() const {
	if (map == nullptr) {
		std::cerr << "Error: map is null in getInitialPlayerTilePos()" << std::endl;
		return glm::vec2(0.f, 0.f);
	}

	glm::vec2 pos = getPlayerInitialTile();
	return glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize());
}

bool LevelScene::enemyPlayerCollision() const {
	const glm::ivec2 playerBox = glm::ivec2(player->getPosition()) + PLAYER_COLLISION_OFFSET;
	for (const auto& enemy : enemies) {
       if (enemy->hitsPlayer())
			return true;
		const glm::ivec2 enemyBox = glm::ivec2(enemy->getPosition()) + enemy->getCollisionOffset();
		if (playerBox.x < enemyBox.x + enemy->getCollisionSize().x &&
			playerBox.x + SIZE_PLAYER_COLLISION.x > enemyBox.x &&
			playerBox.y < enemyBox.y + enemy->getCollisionSize().y &&
			playerBox.y + SIZE_PLAYER_COLLISION.y > enemyBox.y) {
			return true;
		}
	}
	return false;
}
void LevelScene::hurtPlayer()
{
	if (isGameOver || player->isInvincible()) return;

	Stats& stats = Stats::getInstance();
	if (stats.hasHeart()) {
		player->setInvincible(3000);  // 2 s of grace, with blinking
		stats.setHeart(false);        // the extra hit absorbs the damage
		return;
	}

	isGameOver = true;
	player->setDead();
}