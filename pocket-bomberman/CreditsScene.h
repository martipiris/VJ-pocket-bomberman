#ifndef _CREDITS_SCENE_INCLUDE
#define _CREDITS_SCENE_INCLUDE

#include "TextPageScene.h"

 //Text for the credits scene
class CreditsScene : public TextPageScene {
public:
	CreditsScene()
		: TextPageScene("Credits", {
			"Programacio d'enemics i jugador - Marti",
			"Disseny de so - Marti",
			"GUI i menus - Marti",
			"Disseny de mapes - Oscar",
			"Interaccio de bombes - Oscar",
			"Art personalitzat - Oscar"
			},
			"Utilitzant recursos de Pocket Bomberman",
			false) {
	}   // no bullets: the name on the right is enough
};

#endif
