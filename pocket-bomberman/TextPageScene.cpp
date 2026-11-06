#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "TextPageScene.h"
#include "Game.h"

namespace {
	const float HEADER_H = 90.f;
	const float LIST_Y = 108.f;
	const float ROW_STEP = 44.f;
	const float ROW_H = 38.f;
	const float MARGIN = 20.f;
	const float FOOTER_Y = 428.f;
	const int   ROW_TEXT = 22;

	const glm::vec4 ORANGE(0.98f, 0.66f, 0.30f, 1.f);   // soft orange (header, bullets, divider)
	const glm::vec4 DARK_ORANGE(0.80f, 0.42f, 0.12f, 1.f);   // deeper orange (names, footer text, header line)
	const glm::vec4 STRIPE_A(1.00f, 0.93f, 0.84f, 1.f);   // pale orange row
	const glm::vec4 STRIPE_B(1.00f, 0.97f, 0.92f, 1.f);   // paler orange row
	const glm::vec4 SHADOW(0.45f, 0.22f, 0.05f, 1.f);   // title drop shadow, dark brown-orange
	const glm::vec4 INK(0.15f, 0.10f, 0.15f, 1.f);
	const glm::vec4 WHITE(1.f, 1.f, 1.f, 1.f);
	const glm::vec4 GREY(0.45f, 0.40f, 0.45f, 1.f);
}

TextPageScene::TextPageScene(const std::string& title, const std::vector<std::string>& lines,
  const std::string& footnote, bool bullets, const std::string& musicTrack)
	: title(title), footnote(footnote), musicTrack(musicTrack), lines(lines), bullets(bullets)
{
}

void TextPageScene::initObjects()
{
   musicId = Game::instance().audio().playSound(musicTrack, true);
	text.init(MAIN_FONT);

	header = Quad::createQuad(0.f, 0.f, float(SCREEN_WIDTH), HEADER_H, simpleProgram);
	line = Quad::createQuad(0.f, 0.f, float(SCREEN_WIDTH), 4.f, simpleProgram);
	stripe = Quad::createQuad(0.f, 0.f, SCREEN_WIDTH - 2 * MARGIN, ROW_H, simpleProgram);
	bullet = Quad::createQuad(0.f, 0.f, 12.f, 12.f, simpleProgram);
}

void TextPageScene::drawQuad(Quad* q, float x, float y, const glm::vec4& c)
{
	simpleProgram.use();
	simpleProgram.setUniformMatrix4f("projection", projection);
	simpleProgram.setUniformMatrix4f("modelview", glm::translate(glm::mat4(1.f), glm::vec3(x, y, 0.f)));
	simpleProgram.setUniform4f("color", c.r, c.g, c.b, c.a);
	q->render();
}

void TextPageScene::drawRow(const std::string& lineStr, float x, float baseline, float maxRight)
{
	std::string left = lineStr, right;
	const size_t sep = lineStr.rfind(" - ");
	if (sep != std::string::npos) {
		left = lineStr.substr(0, sep);
		right = lineStr.substr(sep + 3);
	}

	// Shrink the text if the row would not fit
	int size = ROW_TEXT;
	const float avail = maxRight - x;
	while (size > 12) {
		float w = text.getWidth(left, size);
		if (!right.empty()) w += text.getWidth(right, size) + 16.f;
		if (w <= avail) break;
		--size;
	}

	text.render(left, glm::vec2(x, baseline), size, INK);
	if (!right.empty()) {
		const float rw = text.getWidth(right, size);
		text.render(right, glm::vec2(maxRight - rw, baseline), size, DARK_ORANGE);
	}
}

void TextPageScene::renderObjects()
{
	// Header band + title with a drop shadow
	drawQuad(header, 0.f, 0.f, ORANGE);
	drawQuad(line, 0.f, HEADER_H, DARK_ORANGE);

	const int titleSize = 44;
	const float tw = text.getWidth(title, titleSize);
	const float tx = (SCREEN_WIDTH - tw) * 0.5f;
	text.render(title, glm::vec2(tx + 3.f, 65.f), titleSize, SHADOW);
	text.render(title, glm::vec2(tx, 62.f), titleSize, WHITE);

	// Rows
	for (size_t i = 0; i < lines.size(); ++i) {
		const float y = LIST_Y + i * ROW_STEP;
		drawQuad(stripe, MARGIN, y, (i % 2 == 0) ? STRIPE_A : STRIPE_B);

		float textX = MARGIN + 12.f;
		if (bullets) {
			drawQuad(bullet, textX, y + 13.f, ORANGE);
			textX += 24.f;
		}
		drawRow(lines[i], textX, y + 26.f, SCREEN_WIDTH - MARGIN - 12.f);
	}

	// Optional note under the list
	if (!footnote.empty()) {
		const int size = 18;
		const float w = text.getWidth(footnote, size);
		text.render(footnote, glm::vec2((SCREEN_WIDTH - w) * 0.5f, FOOTER_Y - 22.f), size, GREY);
	}

	// Footer: divider + pulsing "go back" hint
	drawQuad(line, 0.f, FOOTER_Y, ORANGE);

	const std::string back = "Prem X per tornar";
	const int backSize = 26;
	const float bw = text.getWidth(back, backSize);
	const float alpha = 0.65f + 0.35f * std::sin(currentTime / 300.f);
	text.render(back, glm::vec2((SCREEN_WIDTH - bw) * 0.5f, 462.f), backSize,
		glm::vec4(DARK_ORANGE.r, DARK_ORANGE.g, DARK_ORANGE.b, alpha));
}

void TextPageScene::updateObjects(int deltaTime, int currentTime)
{
	// Ignore the first 400 ms so a key still held from the previous scene
	// (X is also the jump key) doesn't send you straight back
 if (currentTime > 400 && Game::instance().isKeyPressedOnce(RETURN_KEY)) {
		Game::instance().audio().playSound("menu_select");
		Game::instance().changeScene(STARTING);
   }
}

void TextPageScene::deleteObjects()
{
	delete header; header = nullptr;
	delete line;   line = nullptr;
	delete stripe; stripe = nullptr;
	delete bullet; bullet = nullptr;
}