#include "StartingScene.h"

void StartingScene::renderObjects() {
	//titleText.render("Pocket Momberban", glm::vec2(10, 30), 40, glm::vec4(1, 1, 1, 1));
	title->render(texs[0]);
}

void StartingScene::initObjects() {
	titleText.init("fonts/arcade.ttf");

	glm::vec2 geom[2] = { glm::vec2(100.f, 0.f), glm::vec2(SCREEN_WIDTH-100, SCREEN_HEIGHT*2/5) };
	glm::vec2 texCoords[2];
	texCoords[0] = glm::vec2(0.f, 0.f); texCoords[1] = glm::vec2(1.f, 1.f);
	title = TexturedQuad::createTexturedQuad(geom, texCoords, texProgram);

	texs[0].loadFromFile("images/title.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs[0].setMagFilter(GL_NEAREST);

	printf("test");
}