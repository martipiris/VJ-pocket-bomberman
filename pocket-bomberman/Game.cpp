#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Game.h"
#include "StartingScene.h"
#include "CreditsScene.h"
#include "InstructionsScene.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"
#include "Level5.h"
#include <iostream>

Game::Game() {}

void Game::init()
{
	glDepthFunc(GL_LEQUAL);
	bPlay = true;
   if (!audioManager.initialize())
		std::cerr << "Failed to initialize audio output.\n";
	glClearColor(1, 1, 1, 1);
	scene = new StartingScene();
	scene->init();
}

bool Game::update(int deltaTime)
{
	scene->update(deltaTime);
	audioManager.update();

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
	case LV2:
		newScene = new Level2();
		break;
	case LV3:
		newScene = new Level3();
		break;
	case LV4:
		newScene = new Level4();
		break;
	case LV5:
		newScene = new Level5();
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



