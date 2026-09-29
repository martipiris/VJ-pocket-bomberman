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

private:
	Text titleText;
	TexturedQuad* title;
	Texture texs[2];
};

#endif // _STARTING_SCENE_INCLUDE