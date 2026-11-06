#ifndef _LEVEL2_INCLUDE
#define _LEVEL2_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
#include "Pacman.h"
#include <glm/glm.hpp>

class Level2 : public LevelScene {
public:
	Level2()
		: LevelScene("levels/level02.txt")
	{
	}

	~Level2() override = default;

	int id() const override {
		return LV2;
	}

protected:
	glm::vec2 getPlayerInitialTile() const override {
		return glm::vec2(4, 29);
	}

private:
	glm::ivec2 monkeyCoords[4] = { glm::ivec2(1, 26),
								  glm::ivec2(7, 13),
								  glm::ivec2(5, 4),
								  glm::ivec2(4, 7)
	};

	glm::ivec2 pacmanCoords = glm::ivec2(13, 25);

	void onLevelInit() override;
	void onLevelUpdate(int deltaTime) override;
	void onLevelRender() override;
	std::string introTitle() const override { return "STAGE 2"; }
	int nextLevelId() const override { return LV3; };
};

#endif // _LEVEL2_INCLUDE