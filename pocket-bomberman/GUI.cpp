#include "GUI.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

void GUI::init()
{
	background = Quad::createQuad(
		0,
		SCREEN_HEIGHT - 50,
		SCREEN_WIDTH,
		50,
		program);
}

void GUI::render()
{
	glDisable(GL_DEPTH_TEST);

	program.use();
	program.setUniformMatrix4f(
		"projection",
		glm::ortho(
			0.0f,
			float(SCREEN_WIDTH),
			float(SCREEN_HEIGHT),
			0.0f));

	program.setUniformMatrix4f("modelview", glm::mat4(1.0f));
	program.setUniform4f("color", 0.2f, 0.2f, 0.8f, 1.0f);

	background->render();

	glEnable(GL_DEPTH_TEST);
}