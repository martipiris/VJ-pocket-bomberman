#include "CreditsScene.h"

#include "Game.h"
#include "StartingScene.h"

void CreditsScene::initObjects() {
	title->init(MAIN_FONT);

	for (int i = 0; i < credits->size(); ++i) {
		creditTexts[i] = new Text();
		creditTexts[i]->init(MAIN_FONT);
	}
}

void CreditsScene::renderObjects() {
	int x_coord;

	title->render("Credits", glm::vec2(SCREEN_WIDTH / 2 - 40, 10), 40, glm::vec4(0, 0, 0, 1));
}

void CreditsScene::updateObjects(int deltaTime, int currentTime) {
	if (Game::instance().getKey(RETURN_KEY)) {
		Game::instance().changeScene(new StartingScene());
	}
}