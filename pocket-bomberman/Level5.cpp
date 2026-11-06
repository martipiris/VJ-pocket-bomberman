#include "Level5.h"

void Level5::onLevelInit() {
	addEnemy<Tree>();
	enemies.back()->init(map->tile2Pixel(bossCoords), map, player, texProgram);
}

void Level5::onLevelUpdate(int deltaTime) {

}

void Level5::onLevelRender() {

}

void Level5::playSceneMusic() {
	musicId = Game::instance().audio().playSound("boss_theme", true);
}