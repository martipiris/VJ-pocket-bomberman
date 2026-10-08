#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"


Game::Game() {}

void Game::init()
{
	bPlay = true;
	glClearColor(1, 1, 1, 1);
	scene = dynamic_cast<Scene*>(new StartingScene);
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



