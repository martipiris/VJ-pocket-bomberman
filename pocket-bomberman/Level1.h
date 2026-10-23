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

protected:
	void onLevelInit() override;

	void onLevelUpdate(int deltaTime) override;

	void onLevelRender() override;
};

#endif // _LEVEL1_INCLUDE