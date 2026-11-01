#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "StartingScene.h"
#include "CreditsScene.h"
#include "InstructionsScene.h"

Game::Game() {}

void Game::init()
{
	bPlay = true;
	glClearColor(1, 1, 1, 1);
	scene = new StartingScene();
	scene->init();
}

bool Game::update(int deltaTime)
{
	scene->update(deltaTime);

	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	scene->render();
}

void Game::changeScene(int id) {
	if (scene != nullptr) {
		scene->deleteScene();
		delete scene;
		scene = nullptr;
	}

	Scene* newScene;
	switch (id) {
	case STARTING:
		newScene = new StartingScene();
		break;
	case INSTRUCTIONS:
		newScene = new InstructionsScene();
		break;
	case CREDITS:
		newScene = new CreditsScene();
		break;
	case LV1:
		newScene = new Level1();
		break;
	default:
		printf("ERROR: couldn't load scene %d\n", id);
		newScene = new StartingScene();
	}

	scene = newScene;
	scene->init();
}

void Game::keyPressed(int key)
{
	if(key == GLFW_KEY_ESCAPE) // Escape code
		bPlay = false;
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
	keysProcessed[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::isKeyPressedOnce(int key)
{
	if (keys[key] && !keysProcessed[key]) {
		keysProcessed[key] = true;
		return true;
	}
	return false;
}



