#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "libs/Quad.h"
#include "libs/TexturedQuad.h"
#include "libs/Text.h"
#include "libs/ShaderProgram.h"

#include "Game.h"

struct Stat {
	TexturedQuad icon;
	Text text;
	int x_pos;
};

class GUI {
public:
	GUI(ShaderProgram& simpleProgram) : program(simpleProgram) {};
	~GUI();

	void init();

	void render();
private:
	Quad *background;
	Stat* stats[5];

	ShaderProgram program;
};

#endif // _GUI_INCLUDE
