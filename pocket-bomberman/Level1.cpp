#include "Level1.h"

void Level1::onLevelInit() {
	addEnemy<Monkey>();
	enemies.back()->init(map->tile2Pixel(glm::ivec2(1, 9)), texProgram);
}

void Level1::onLevelUpdate(int deltaTime) {

}

void Level1::onLevelRender() {

}