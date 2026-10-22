#ifndef _LEVEL_SCENE_INCLUDE
#define _LEVEL_SCENE_INCLUDE

#include <string>
#include <glm/glm.hpp>
#include "Scene.h"
#include "libs/TileMap.h"
#include "Player.h"
#include "Camera.h"
#include "GUI.h"
#include "Enemy.h"

#define SCREEN_OFFSET_X 0
#define SCREEN_OFFSET_Y 0

class LevelScene : public Scene {
public:
	LevelScene(const std::string &levelFile);
	virtual ~LevelScene();

private:
	void renderObjects() override;
	void initObjects() override;
	void updateObjects(int deltaTime, int currentTime) override;
	void deleteObjects() override;

protected:
	virtual std::string getLevelFile() const;

	glm::vec2 getInitialPlayerTilePos() const;

	virtual void onLevelInit() {}
	virtual void onLevelUpdate(int deltaTime) {}
	virtual void onLevelRender() {}

	TileMap *map = nullptr;
	Player player;
	GUI *gui = nullptr;

	int playerInitialTileX = 4;
	int playerInitialTileY = 13;

	int timer = 240;

	Camera *cam;

private:
	std::string m_levelFile;

	// REMOVE
	Enemy *e;
};


#endif // _LEVEL_SCENE_INCLUDE

