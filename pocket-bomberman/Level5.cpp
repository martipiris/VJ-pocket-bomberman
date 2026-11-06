#include "Level5.h"

void Level5::onLevelInit() {
	addEnemy<Tree>();
	enemies.back()->init(map->tile2Pixel(bossCoords), map, player, texProgram);
}

void Level5::onLevelUpdate(int deltaTime) {
	if (enemies.empty())
		startOutro();
}

void Level5::onLevelRender() {

}