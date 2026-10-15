#ifndef _LEVEL_SCENE_INCLUDE
#define _LEVEL_SCENE_INCLUDE


#include <string>
#include <glm/glm.hpp>
#include "Scene.h"
#include "libs/TileMap.h"
#include "Player.h"

class LevelScene : public Scene {
public:
	LevelScene(const std::string &levelFile);
	virtual ~LevelScene();

private:
	void renderObjects() override;
	void initObjects() override;
	void updateObjects(int deltaTime) override;
	void deleteObjects() override;

protected:
	virtual std::string getLevelFile() const;
	virtual glm::ivec2 getInitialPlayerTilePos() const;
	virtual glm::vec2 getMapMinCoords() const;

	virtual void onLevelInit() {}
	virtual void onLevelUpdate(int deltaTime) {}
	virtual void onLevelRender() {}

	TileMap *map = nullptr;
	Player player;

private:
	std::string m_levelFile;
};


#endif // _LEVEL_SCENE_INCLUDE

