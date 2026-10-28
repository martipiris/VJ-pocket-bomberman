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
	Text* title;

	Text* creditTexts[7];

	string credits[7] = {
		"Infraestructura de motor - Marti",
		"Programació d'enemics i jugador - Marti",
		"Disseny de so - Marti",
		"Disseny de mapes - Oscar",
		"Interacció de bombes - Oscar",
		"Art personalitzat - Oscar",
		"Utilitzant recursos de Pocket Bomberman"
	};
};

#endif //_CREDITS_SCENE_INCLUDE

