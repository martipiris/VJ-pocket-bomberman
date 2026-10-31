#ifndef _CREDITS_SCENE_INCLUDE
#define _CREDITS_SCENE_INCLUDE

#include "Scene.h"
#include "libs/Text.h"

class CreditsScene : public Scene {
public:
	CreditsScene() {};

	void initObjects();
	void renderObjects();
	void updateObjects(int deltaTime, int currentTime);
	void deleteObjects();

private:
	Text title;

	static const int creditLines = 7;

	Text* creditTexts[creditLines];
	string credits[creditLines] = {
		"Programacio d'enemics i jugador - Marti",
		"Disseny de so - Marti",
		"GUI i menus - Marti",
		"Disseny de mapes - Oscar",
		"Interaccio de bombes - Oscar",
		"Art personalitzat - Oscar",
		"Utilitzant recursos de Pocket Bomberman"
	};

	Text backText;
};

#endif //_CREDITS_SCENE_INCLUDE

