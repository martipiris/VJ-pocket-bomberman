#include "Level5.h"

void Level5::onLevelInit() {
	for (int i = 0; i < 4; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(enemyCoords[i]), map, texProgram);
	}
}

void Level5::onLevelUpdate(int deltaTime) {

}

void Level5::onLevelRender() {

}