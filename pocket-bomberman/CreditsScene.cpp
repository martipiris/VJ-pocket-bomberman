#include "CreditsScene.h"

#include "Game.h"

void CreditsScene::initObjects() {
	title.init(MAIN_FONT);

	for (int i = 0; i < creditLines; ++i) {
		creditTexts[i] = new Text();
		creditTexts[i]->init(MAIN_FONT);
	}

	backText.init(MAIN_FONT);
}

void CreditsScene::renderObjects() {
	int y_coord = 130;

	title.render("Credits", glm::vec2(170, 50), 40, glm::vec4(0, 0, 0, 1));

	for (int i = 0; i < creditLines; ++i) {
		creditTexts[i]->render(credits[i], glm::vec2(10, y_coord), 20, glm::vec4(0, 0, 0, 1));
		y_coord += 40;
	}

	title.render("Prem X per tornar", glm::vec2(45, SCREEN_HEIGHT - 20), 40, glm::vec4(0, 0, 0, 1));
}

void CreditsScene::updateObjects(int deltaTime, int currentTime) {
	if (Game::instance().getKey(RETURN_KEY)) {
		Game::instance().changeScene(STARTING);
	}
}

void CreditsScene::deleteObjects() {
	for (int i = 0; i < creditLines; ++i) {
		delete creditTexts[i];
	}
}