#ifndef _INSTRUCTIONS_SCENE_INCLUDE
#define _INSTRUCTIONS_SCENE_INCLUDE

#include "Scene.h"
#include "libs/Text.h"

class InstructionsScene : public Scene {
public:
	InstructionsScene() {};

	void initObjects();
	void renderObjects();
	void updateObjects(int deltaTime, int currentTime);
	void deleteObjects();

private:
	Text title;

	static const int instructionLines = 7;

	Text* instructionTexts[instructionLines];
	string instructions[instructionLines] = {
		"Mou-te amb: <- ->",
		"Salta amb: X",
		"Posa una bomba amb: Z",
		"Explota els enemics per passar de nivell",
		"No deixis que els enemics et toquin!",
		"Vigila el temps!",
		"Les fulles contenen millores utils",
	};

	Text backText;
};

#endif //_INSTRUCTIONS_SCENE_INCLUDE

