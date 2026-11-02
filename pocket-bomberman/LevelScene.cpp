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

	bombs = new BombManager(map, texProgram, maxBombs, bombRange);

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

	if (enemyPlayerCollision() && !isGameOver) {
		isGameOver = true;
		player->setDead();
	}

	if (gui->updateTimer(std::max(0, int(timer - currentTime / 1000))) == 0 && !isGameOver) {
		isGameOver = true;
		player->setDead();
	}

	gui->setEnemyCount(enemies.size());

	player->update(deltaTime);

	const glm::vec2 p = player->getPosition();
	const glm::ivec2 boxPos = glm::ivec2(p) + PLAYER_COLLISION_OFFSET;

	if (!isGameOver && Game::instance().isKeyPressedOnce(GLFW_KEY_Z)) {
		// Tile under the player's feet, horizontally centered
		glm::ivec2 feet(int(p.x) + SIZE_PLAYER.x / 2, int(p.y) + SIZE_PLAYER.y - 1);
		bombs->tryPlace(map->pixel2Tile(feet));
	}

	bombs->update(deltaTime, boxPos, SIZE_PLAYER_COLLISION);

	// Explosion kills the player
	if (!isGameOver && bombs->hits(boxPos, SIZE_PLAYER_COLLISION)) {
		isGameOver = true;
		player->setDead();
	}

	// Explosion kills enemies
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[&](const std::unique_ptr<Enemy>& e) {
			return bombs->hits(glm::ivec2(e->getPosition()) + ENEMY_COLLISION_OFFSET, SIZE_ENEMY_COLLISION);
		}), enemies.end());

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
	bombs->render(view);
	player->render(view);

	for (auto& enemy : enemies) {
		enemy->render(view);
	}

	gui->render();

	onLevelRender();
}

void LevelScene::deleteObjects() {
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
	for (const auto& enemy : enemies) {
		const glm::vec2 enemyPos = enemy->getPosition();
		const glm::vec2 playerPos = player->getPosition();
		if (std::abs(enemyPos.x - playerPos.x) < SIZE_ENEMY_COLLISION.x &&
			std::abs(enemyPos.y - playerPos.y) < SIZE_ENEMY_COLLISION.y) {
			return true;
		}
	}
	return false;
}