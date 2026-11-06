#include <algorithm>
#include <cmath>
#include <vector>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include "ScreenWipe.h"
#include "Game.h"

namespace {
	const int SEGMENTS = 96;                 // triangles for a full turn
	const float RADIUS = 400.f;              // larger than the half-diagonal of 480x480 (~340)
	const float TWO_PI = 6.2831853f;
}

ScreenWipe::ScreenWipe(ShaderProgram& p) : program(&p), vao(0), vbo(0), posLocation(0)
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, SEGMENTS * 3 * 2 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 2 * sizeof(float), 0);
}

ScreenWipe::~ScreenWipe()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void ScreenWipe::drawSector(float from, float to, const glm::vec4& color)
{
	from = std::max(0.f, std::min(1.f, from));
	to = std::max(0.f, std::min(1.f, to));
	if (to - from <= 0.f) return;

	const int segs = std::max(1, std::min(SEGMENTS, int(std::ceil((to - from) * SEGMENTS))));
	const glm::vec2 c(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

	std::vector<float> v;
	v.reserve(segs * 6);
	for (int i = 0; i < segs; ++i) {
		const float a0 = (from + (to - from) * float(i) / segs) * TWO_PI;
		const float a1 = (from + (to - from) * float(i + 1) / segs) * TWO_PI;
		v.push_back(c.x); v.push_back(c.y);
		v.push_back(c.x + RADIUS * std::sin(a0)); v.push_back(c.y - RADIUS * std::cos(a0));
		v.push_back(c.x + RADIUS * std::sin(a1)); v.push_back(c.y - RADIUS * std::cos(a1));
	}

	program->use();
	program->setUniformMatrix4f("projection", glm::ortho(0.f, float(SCREEN_WIDTH), float(SCREEN_HEIGHT), 0.f));
	program->setUniformMatrix4f("modelview", glm::mat4(1.f));
	program->setUniform4f("color", color.r, color.g, color.b, color.a);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, v.size() * sizeof(float), v.data());
	glEnableVertexAttribArray(posLocation);
	glDrawArrays(GL_TRIANGLES, 0, segs * 3);
}