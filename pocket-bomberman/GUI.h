#ifndef _GUI_INCLUDE
#define _GUI_INCLUDE

#include "libs/TexturedQuad.h"
#include "libs/Text.h"


struct Stat {
	TexturedQuad icon;
	Text text;
	int x_pos;
};

class GUI {
public:
	GUI();
	~GUI();

	void render();
private:
	TexturedQuad background;

	Stat* stats[5];
};

#endif // _GUI_INCLUDE
