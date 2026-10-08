#ifndef _STARTING_SCENE_INCLUDE
#define _STARTING_SCENE_INCLUDE

#include "Scene.h"
#include "Text.h"
#include "TexturedQuad.h"
#include "Game.h"

class StartingScene : public Scene
{

public:
	StartingScene() {};

	void renderObjects();
	void initObjects();
	void updateObjects();

private:
	string menuOptionsText[3] = { "Jugar", "Instr.", "Credits" };
	int selectedOption = 0;

	Text bottomText;
	Text* menuOptions[3];

	TexturedQuad* title;
	TexturedQuad* menu_bg;
	TexturedQuad* selectorBomb;

	Texture texs[3];
};

#endif // _STARTING_SCENE_INCLUDE