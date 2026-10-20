#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelScene.h"

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
	if (map) {
		player.init(glm::ivec2(SCREEN_OFFSET_X, SCREEN_OFFSET_Y), texProgram);
		player.setPosition(getInitialPlayerTilePos());
		player.setTileMap(map);

		cam = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, map->getMapPixelSize(), player.getPosition());
	}
	onLevelInit();
}

void LevelScene::updateObjects(int deltaTime)
{
	// Actualizar jugador primero (posicionará su sprite)
	player.update(deltaTime);

	// Actualizar cámara para centrarla en el jugador
	cam->setTarget(player.getPosition());
	cam->update();

	onLevelUpdate(deltaTime);
}

void LevelScene::renderObjects()
{
	glm::mat4 view = cam->getViewMatrix();
	texProgram.setUniformMatrix4f("modelview", view);
	simpleProgram.setUniformMatrix4f("modelview", view);

	if (map) map->render();

	player.render(view);
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

glm::vec2 LevelScene::getInitialPlayerTilePos() const {
	if (map == nullptr) {
		std::cerr << "Error: map is null in getInitialPlayerTilePos()" << std::endl;
		return glm::vec2(0.f, 0.f);
	}
	return glm::vec2(playerInitialTileX*map->getTileSize(), playerInitialTileY * map->getTileSize());
}
