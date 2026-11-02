#include "Level2.h"

void Level2::onLevelInit() {
	for (int i = 0; i < 4; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(monkeyCoords[i]), map, player, texProgram);
	}

	addEnemy<Pacman>();
	enemies.back()->init(map->tile2Pixel(pacmanCoords), map, player, texProgram);
}

void Level2::onLevelUpdate(int deltaTime) {

}

void Level2::onLevelRender() {

}