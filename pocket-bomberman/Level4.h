#ifndef _LEVEL4_INCLUDE
#define _LEVEL4_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
#include "Pacman.h"
#include <glm/glm.hpp>

class Level4 : public LevelScene {
public:
	Level4()
		: LevelScene("levels/level04.txt")
	{
	}

	~Level4() override = default;

	int id() const override {
		return LV4;
	}

protected:
	glm::vec2 getPlayerInitialTile() const override {
		return glm::vec2(4, 13);
	}

private:
	glm::ivec2 monkeyCoords[2] = { glm::ivec2(6, 11),
								  glm::ivec2(24, 10),
	};

	glm::ivec2 pacmanCoords[3] = { glm::ivec2(23, 2),
								  glm::ivec2(16, 1),
								  glm::ivec2(20, 9)
	};

	void onLevelInit() override;
	void onLevelUpdate(int deltaTime) override;
	void onLevelRender() override;
};

#endif // _LEVEL4_INCLUDE