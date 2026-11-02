#include "Level4.h"

void Level4::onLevelInit() {
	for (int i = 0; i < 2; ++i) {
		addEnemy<Monkey>();
		enemies.back()->init(map->tile2Pixel(monkeyCoords[i]), map, player, texProgram);
	}

	for (int i = 0; i < 3; ++i) {
		addEnemy<Pacman>();
		enemies.back()->init(map->tile2Pixel(pacmanCoords[i]), map, player, texProgram);
	}
}

void Level4::onLevelUpdate(int deltaTime) {

}

void Level4::onLevelRender() {

}