#include "StartingScene.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

void StartingScene::renderObjects() {
	glDisable(GL_DEPTH_TEST);

	title->render(texs[0]);
	menu_bg->render(texs[1]);
	character->render(texs[3]);

	glm::mat4 modelview;
	modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 50.f*selectedOption, 0.f));

	texProgram.setUniformMatrix4f("modelview", modelview);
	selectorBomb->render(texs[2]);

	// Menu options
	int startingY = 255;
	for (int i = 0; i < 3; i++) {
		menuOptions[i]->render(menuOptionsText[i], glm::vec2(280, startingY), 30, glm::vec4(0, 0, 0, 1));
		startingY += 50;
	}

	bottomText.render("VJ FIB QT 2026/27", glm::vec2(30, SCREEN_HEIGHT-20), 40, glm::vec4(0, 0, 0, 1));
}

void StartingScene::initObjects() {
	// Copyright text
	if (!bottomText.init(MAIN_FONT)) {
		printf("Error importing font!");
	}

	for (int i = 0; i < 3; i++) {
		menuOptions[i] = new Text();

		if (!menuOptions[i]->init(MAIN_FONT)) {
			printf("Error importing font!");
		}
	}

	glm::vec2 texCoords[2];

	// Title
	glm::vec2 geom[2] = { glm::vec2(75.f, 0.f), glm::vec2(SCREEN_WIDTH - 75, SCREEN_HEIGHT * 0.4375) };
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(0.66f, 0.73f);
	title = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	// Menu background
	geom[0] = glm::vec2(220.f, 189.f); geom[1] = glm::vec2(SCREEN_WIDTH - 25, 400);

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	menu_bg = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texs[0].loadFromFile("images/title_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);

	// Menu background
	geom[0] = glm::vec2(220.f, 189.f); geom[1] = glm::vec2(SCREEN_WIDTH - 25, 400);

	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	menu_bg = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);
	
	texs[1].loadFromFile("images/menu_bg.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[1].setMagFilter(GL_NEAREST);

	// Selector bomb
	geom[0] = glm::vec2(245.f, 227.f); geom[1] = glm::vec2(270.f, 252.f);

	texCoords[0] = glm::vec2(0.03f, 0.85f); texCoords[1] = glm::vec2(0.08f, 0.945f);
	selectorBomb = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texs[2].loadFromFile("images/title_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[2].setMagFilter(GL_NEAREST);

	//Character
	geom[0] = glm::vec2(20.f, 190.f); geom[1] = glm::vec2(SCREEN_WIDTH - 270, SCREEN_HEIGHT - 20);
	texCoords[0] = glm::vec2(0.68f, 0.05f); texCoords[1] = glm::vec2(1.f, 0.92f);
	character = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texs[3].loadFromFile("images/title_spritesheet.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[3].setMagFilter(GL_NEAREST);
}

void StartingScene::updateObjects(int deltaTime, int currentTime) {

	if (Game::instance().isKeyPressedOnce(GLFW_KEY_DOWN)) {
		selectedOption = (selectedOption + 1) % 3;
	} else if (Game::instance().isKeyPressedOnce(GLFW_KEY_UP)) {
		selectedOption = (selectedOption + 2) % 3; 
	}

	if (Game::instance().isKeyPressedOnce(CONFIRM_KEY)) {
		switch (selectedOption) {
			case 0:
				Game::instance().changeScene(LV1);
				Stats::getInstance().resetAll();
				break;
			case 1:
				Game::instance().changeScene(INSTRUCTIONS);
				break;
			case 2:
				Game::instance().changeScene(CREDITS);
				break;
		}
	}
}

void StartingScene::deleteObjects() {
	
	if (title != NULL)
		delete title;

	if (menu_bg != NULL)
		delete menu_bg;

	if (selectorBomb != NULL)
		delete selectorBomb;

	for (int i = 0; i < 3; i++) {
		if (menuOptions[i] != NULL)
			delete menuOptions[i];
	}
}