#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "libs/Quad.h"
#include "libs/TexturedQuad.h"
#include "libs/Text.h"
#include "libs/ShaderProgram.h"

#include "Game.h"

#define GUI_HEIGHT 30

struct Stat {
	TexturedQuad *icon;
	Text *text;
	int x_coord;
};

class GUI {
public:
	GUI(ShaderProgram& simpleProgram, ShaderProgram& texProgram) : program(simpleProgram), texProgram(texProgram) {};
	~GUI();

	void init();
	void render();

	int updateTimer(int newTime);
private:
	const int x_coords[5] = {20, 190, 260, 330, 400};
	glm::vec2 texCoords[5][2] = {
		{glm::vec2(0.5f, 0.5f), glm::vec2(0.75f, 1.f)},
		{glm::vec2(0.5f, 0.0f), glm::vec2(0.75f, 0.5f)},
		{glm::vec2(0.0f, 0.5f), glm::vec2(0.25f, 1.0f)},
		{glm::vec2(0.25f, 0.0f), glm::vec2(0.5f, 0.5f)},
		{glm::vec2(0.0f, 0.0f), glm::vec2(0.25f, 0.5f)}
	};

	char timer[6];

	Quad *background;
	Stat *stats[5];

	Texture texs;

	ShaderProgram program, texProgram;
};

#endif // _GUI_INCLUDE
