#ifndef _LEVEL2_INCLUDE
#define _LEVEL2_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
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
	glm::ivec2 enemyCoords[4] = { glm::ivec2(1, 9),
								  glm::ivec2(7, 7),
								  glm::ivec2(7, 4),
								  glm::ivec2(12, 13)
	};

	void onLevelInit() override;
	void onLevelUpdate(int deltaTime) override;
	void onLevelRender() override;
};

#endif // _LEVEL2_INCLUDE