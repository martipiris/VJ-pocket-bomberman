#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene.h"
#include "Game.h"

Scene::Scene() {}

void Scene::init()
{
	projection = glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f);
	currentTime = 0.0f;
	initShaders();
	initObjects();
}

void Scene::update(int deltaTime)
{
	currentTime += deltaTime;
	updateObjects(deltaTime);
}

void Scene::render()
{
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);

	renderObjects();
}

void Scene::deleteScene() {
	texProgram.free();
	deleteObjects();
}

void Scene::initShaders()
{
	Shader textureVertexShader;
	Shader textureFragmentShader;
	Shader simpleVertexShader;
	Shader simpleFragmentShader;

	textureVertexShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	textureFragmentShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");

	simpleVertexShader.initFromFile(VERTEX_SHADER, "shaders/simple.vert");
	simpleFragmentShader.initFromFile(FRAGMENT_SHADER, "shaders/simple.frag");

	texProgram.init();
	texProgram.addShader(textureVertexShader);
	texProgram.addShader(textureFragmentShader);
	texProgram.bindFragmentOutput("outColor");
	texProgram.link();

	simpleProgram.init();
	simpleProgram.addShader(simpleVertexShader);
	simpleProgram.addShader(simpleFragmentShader);
	simpleProgram.bindFragmentOutput("outColor");
	simpleProgram.link();

	textureVertexShader.free();
	textureFragmentShader.free();
	simpleVertexShader.free();
	simpleFragmentShader.free();
}



