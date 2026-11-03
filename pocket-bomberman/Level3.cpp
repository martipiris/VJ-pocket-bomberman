#include "Level3.h"

void Level3::onLevelInit() {
	for (int i = 0; i < 4; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(monkeyCoords[i]), map, player, texProgram);
	}

	for (int i=0; i < 2; ++i) {
		addEnemy<Pacman>();
		enemies.back()->init(map->tile2Pixel(pacmanCoords[i]), map, player, texProgram);
	}
}

void Level3::onLevelUpdate(int deltaTime) {

}

void Level3::onLevelRender() {

}