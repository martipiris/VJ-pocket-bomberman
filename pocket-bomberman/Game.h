#ifndef _GAME_INCLUDE
#define _GAME_INCLUDE


#include <GLFW/glfw3.h>
#include "Scene.h"
#include "StartingScene.h"


#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

#define MAIN_FONT "fonts/ByteBounce.ttf"

// Game is a singleton (a class with a single instance) that represents our whole application


class Game
{

private:
	Game();
	
public:
	static Game &instance()
	{
		static Game G;
		return G;
	}
	
	void init();
	bool update(int deltaTime);
	void render();
	
	// Input callback methods
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMove(int x, int y);
	void mousePress(int button);
	void mouseRelease(int button);

	bool getKey(int key) const;
	bool isKeyPressedOnce(int key);

private:
	bool bPlay; // Continue to play game?
	bool keys[GLFW_KEY_LAST+1]; 
	bool keysProcessed[GLFW_KEY_LAST + 1];

	Scene* scene;
};


#endif // _GAME_INCLUDE


