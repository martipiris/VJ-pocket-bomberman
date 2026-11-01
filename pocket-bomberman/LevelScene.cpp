#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelScene.h"
#include "StartingScene.h"

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
	simpleProgram.setUniformMatrix4f("modelview", view);

	map->render();
	player->render(view);

	for (auto& enemy : enemies) {
		enemy->render(view);
	}

	gui->render();

	onLevelRender();
}

void LevelScene::deleteObjects() {
	delete map;
	delete gui;
	delete player;
	delete cam;
}

std::string LevelScene::getLevelFile() const {
	return m_levelFile;
}

glm::vec2 LevelScene::getInitialPlayerTilePos() const {
	if (map == nullptr) {
		std::cerr << "Error: map is null in getInitialPlayerTilePos()" << std::endl;
		return glm::vec2(0.f, 0.f);
	}
	return glm::vec2(playerInitialTileX*map->getTileSize(), playerInitialTileY * map->getTileSize());
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