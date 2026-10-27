#include "GUI.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

void GUI::init() {
	background = Quad::createQuad(0, SCREEN_HEIGHT - GUI_HEIGHT, SCREEN_WIDTH, GUI_HEIGHT, program);

	texs.loadFromFile("images/gui_icons.png", TEXTURE_PIXEL_FORMAT_RGBA);
	texs.setMinFilter(GL_NEAREST);
	texs.setMagFilter(GL_NEAREST);

	glm::vec2 geom[2];

	for (int i=0; i<5; ++i) {
		stats[i] = new Stat();
		geom[0] = glm::vec2(x_coords[i], SCREEN_HEIGHT - GUI_HEIGHT); geom[1] = glm::vec2(x_coords[i] + GUI_HEIGHT, SCREEN_HEIGHT);
		stats[i]->icon = TexturedQuad::createTexturedQuad(geom, texCoords[i], texProgram);

		stats[i]->text = new Text();
		stats[i]->text->init(MAIN_FONT);

		stats[i]->x_coord = x_coords[i];
	}
}

void GUI::render() {
	glDisable(GL_DEPTH_TEST);

	program.use();
	program.setUniformMatrix4f("projection", glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.0f));

	program.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	program.setUniform4f("color", 248/256.0f, 112/256.0f, 208/256.0f, 1.0f);

	background->render();

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", glm::ortho(0.0f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT),	0.0f));
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);

	texProgram.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	for (int i=0; i<5; ++i) {
		stats[i]->icon->render(texs);
		stats[i]->text->render(i == 0 ? timer : "1", glm::vec2(stats[i]->x_coord + GUI_HEIGHT + 7, SCREEN_HEIGHT + 6), 45, glm::vec4(0, 0, 0, 1));
	}

	glEnable(GL_DEPTH_TEST);
}

int GUI::updateTimer(int newTime) {
	int minutes = newTime / 60;
	int seconds = newTime % 60;

	sprintf(timer, "%d:%02d", minutes, seconds);
	return newTime;
}

GUI::~GUI() {
	delete background;
	for (int i=0; i<5; ++i) {
		delete stats[i]->icon;
		delete stats[i]->text;
		delete stats[i];
	}
}