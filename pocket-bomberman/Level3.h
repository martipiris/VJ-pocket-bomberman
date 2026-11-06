#ifndef _LEVEL3_INCLUDE
#define _LEVEL3_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
#include "Pacman.h"
#include <glm/glm.hpp>

class Level3 : public LevelScene {
public:
	Level3()
		: LevelScene("levels/level03.txt")
	{
	}

	~Level3() override = default;

	int id() const override {
		return LV3;
	}

protected:
	glm::vec2 getPlayerInitialTile() const override {
		return glm::vec2(4, 13);
	}

private:
	glm::ivec2 monkeyCoords[4] = { glm::ivec2(4, 7),
								  glm::ivec2(15, 4),
								  glm::ivec2(14, 10),
								  glm::ivec2(22, 7)
	};

	glm::ivec2 pacmanCoords[2] = { glm::ivec2(7, 4),
								  glm::ivec2(22, 12)
	};

	void onLevelInit() override;
	void onLevelUpdate(int deltaTime) override;
	void onLevelRender() override;
	std::string introTitle() const override { return "STAGE 3"; }
	int nextLevelId() const override { return LV4; };
};

#endif // _LEVEL3_INCLUDE