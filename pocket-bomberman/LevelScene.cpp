#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelScene.h"

#define SCREEN_X 32
#define SCREEN_Y 32

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
	const glm::vec2 minCoords = getMapMinCoords();
	map = TileMap::createTileMap(file, minCoords, texProgram);
	if (map) {
		player.setTileMap(map);
		player.init(getInitialPlayerTilePos(), texProgram);
	}
	onLevelInit();
}

void LevelScene::updateObjects(int deltaTime)
{
	player.update(deltaTime);
	onLevelUpdate(deltaTime);
}

void LevelScene::renderObjects()
{
	if (map) map->render();
	player.render();
	onLevelRender();
}

void LevelScene::deleteObjects() {
	if (map) {
		delete map;
		map = nullptr;
	}
}

std::string LevelScene::getLevelFile() const {
	return m_levelFile;
}

glm::ivec2 LevelScene::getInitialPlayerTilePos() const {
	return glm::ivec2(4, 25);
}

glm::vec2 LevelScene::getMapMinCoords() const {
	return glm::vec2(0.f, 0.f);
}