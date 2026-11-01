#include "Level4.h"

void Level4::onLevelInit() {
	for (int i = 0; i < 4; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(enemyCoords[i]), map, texProgram);
	}
}

void Level4::onLevelUpdate(int deltaTime) {

}

void Level4::onLevelRender() {

}