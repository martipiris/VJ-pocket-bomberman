#ifndef _TEXT_PAGE_SCENE_INCLUDE
#define _TEXT_PAGE_SCENE_INCLUDE

#include <string>
#include <vector>
#include "Scene.h"
#include "libs/Text.h"
#include "libs/Quad.h"

// Shared layout for simple text pages (instructions, credits).
// A line containing " - " is split into a left part and a right-aligned part.
class TextPageScene : public Scene {
public:
	TextPageScene(const std::string& title,
		const std::vector<std::string>& lines,
		const std::string& footnote = "",
       bool bullets = true,
		const std::string& musicTrack = "");

	void initObjects() override;
	void renderObjects() override;
	void updateObjects(int deltaTime, int currentTime) override;
	void deleteObjects() override;

private:
	void drawQuad(Quad* q, float x, float y, const glm::vec4& color);
	void drawRow(const std::string& line, float x, float baseline, float maxRight);

	std::string title, footnote;
 std::string musicTrack;
	std::vector<std::string> lines;
	bool bullets;

	Text text;   // one Text is enough for the whole page
	Quad* header = nullptr, * line = nullptr, * stripe = nullptr, * bullet = nullptr;
};

#endif