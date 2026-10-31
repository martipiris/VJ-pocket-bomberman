#include "InstructionsScene.h"

#include "Game.h"

void InstructionsScene::initObjects() {
	title.init(MAIN_FONT);

	for (int i = 0; i < instructionLines; ++i) {
		instructionTexts[i] = new Text();
		instructionTexts[i]->init(MAIN_FONT);
	}

	backText.init(MAIN_FONT);
}

void InstructionsScene::renderObjects() {
	int y_coord = 130;

	title.render("Instruccions", glm::vec2(110, 50), 40, glm::vec4(0, 0, 0, 1));

	for (int i = 0; i < instructionLines; ++i) {
		instructionTexts[i]->render(instructions[i], glm::vec2(10, y_coord), 20, glm::vec4(0, 0, 0, 1));
		y_coord += 40;
	}

	title.render("Prem X per tornar", glm::vec2(45, SCREEN_HEIGHT - 20), 40, glm::vec4(0, 0, 0, 1));
}

void InstructionsScene::updateObjects(int deltaTime, int currentTime) {
	if (Game::instance().getKey(RETURN_KEY)) {
		Game::instance().changeScene(STARTING);
	}
}

void InstructionsScene::deleteObjects() {
	for (int i = 0; i < instructionLines; ++i) {
		delete instructionTexts[i];
	}
}