#include "Level3.h"

void Level3::onLevelInit() {
	for (int i = 0; i < 4; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(enemyCoords[i]), map, texProgram);
	}
}

void Level3::onLevelUpdate(int deltaTime) {

}

void Level3::onLevelRender() {

}