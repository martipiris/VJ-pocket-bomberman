#ifndef _LEVEL5_INCLUDE
#define _LEVEL5_INCLUDE

#include "LevelScene.h"
#include "Monkey.h"
#include <glm/glm.hpp>

class Level5 : public LevelScene {
public:
	Level5()
		: LevelScene("levels/level05.txt")
	{
	}

	~Level5() override = default;

	int id() const override {
		return LV5;
	}

protected:
	glm::vec2 getPlayerInitialTile() const override {
		return glm::vec2(4, 8);
	}

private:
	void onLevelInit() override;
	void onLevelUpdate(int deltaTime) override;
	void onLevelRender() override;
};

#endif // _LEVEL4_INCLUDE