#ifndef _LEVEL4_INCLUDE
#define _LEVEL4_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
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
	glm::ivec2 enemyCoords[4] = { glm::ivec2(1, 9),
								  glm::ivec2(7, 7),
								  glm::ivec2(7, 4),
								  glm::ivec2(12, 13)
	};

	void onLevelInit() override;
	void onLevelUpdate(int deltaTime) override;
	void onLevelRender() override;
};

#endif // _LEVEL4_INCLUDE