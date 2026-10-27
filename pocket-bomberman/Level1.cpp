#include "Level1.h"

void Level1::onLevelInit() {
	for (int i = 0; i < 4; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(enemyCoords[i]), map, texProgram);
	}
}

void Level1::onLevelUpdate(int deltaTime) {

}

void Level1::onLevelRender() {

}