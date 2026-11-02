#ifndef _LEVEL1_INCLUDE
#define _LEVEL1_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
#include <glm/glm.hpp>

class Level1 : public LevelScene {
public:
	Level1()
		: LevelScene("levels/level01.txt")
	{
	}

	~Level1() override = default;

	int id() const override {
		return LV1;
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

#endif // _LEVEL1_INCLUDE