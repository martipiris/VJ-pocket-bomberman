#ifndef _INSTRUCTIONS_SCENE_INCLUDE
#define _INSTRUCTIONS_SCENE_INCLUDE

#include "TextPageScene.h"

 //Text for the instructions scene
class InstructionsScene : public TextPageScene {
public:
	InstructionsScene()
		: TextPageScene("Instruccions", {
			"Mou-te amb: <- ->",
			"Salta amb: X",
			"Posa una bomba amb: Z",
			"Explota els enemics per passar de nivell",
			"No deixis que els enemics et toquin!",
			"Vigila el temps!",
			"Les fulles contenen millores utils"
            }, "", true, "instructions_theme") {
	}
};

#endif

