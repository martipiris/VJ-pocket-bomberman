#ifndef _STARTING_SCENE_INCLUDE
#define _STARTING_SCENE_INCLUDE

#include "Scene.h"
#include "libs/Text.h"
#include "libs/TexturedQuad.h"
#include "Game.h"
#include "Level1.h"

class StartingScene : public Scene
{

public:
	StartingScene() {};

	void initObjects();
	void renderObjects();
	void updateObjects(int deltaTime, int currentTime);
	void deleteObjects();

private:
	string menuOptionsText[3] = { "Jugar", "Instr.", "Credits" };
	int selectedOption = 0;

	Text bottomText;
	Text* menuOptions[3];

	TexturedQuad* title;
	TexturedQuad* menu_bg;
	TexturedQuad* selectorBomb;
	TexturedQuad* character;

	Texture texs[4];
};

#endif // _STARTING_SCENE_INCLUDE