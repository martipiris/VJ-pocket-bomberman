#ifndef _SCREEN_WIPE_INCLUDE
#define _SCREEN_WIPE_INCLUDE

#include <glm/glm.hpp>
#include "libs/ShaderProgram.h"

// Draws a pie slice over the whole screen. Angles are fractions of a turn,
// measured clockwise from 12 o'clock (0 = top, 0.25 = right, 0.5 = bottom).
class ScreenWipe {
public:
	explicit ScreenWipe(ShaderProgram& program);
	~ScreenWipe();

	void drawSector(float from, float to, const glm::vec4& color);

private:
	ShaderProgram* program;
	GLuint vao, vbo;
	GLint posLocation;
};

#endif
