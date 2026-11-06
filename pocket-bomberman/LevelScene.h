#ifndef _LEVEL_SCENE_INCLUDE
#define _LEVEL_SCENE_INCLUDE

#include <string>
#include <glm/glm.hpp>
#include <memory>
#include <vector>
#include <utility>

#include "Scene.h"
#include "libs/TileMap.h"
#include "Player.h"
#include "Camera.h"
#include "GUI.h"
#include "Enemy.h"
#include "BombManager.h"
#include "PowerUpManager.h"
#include "Stats.h"
#include "ScreenWipe.h"

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

	virtual int id() const = 0;

	virtual glm::vec2 getPlayerInitialTile() const {
		return glm::vec2(4, 13);
	}
	
	glm::vec2 getInitialPlayerTilePos() const;

	virtual void onLevelInit() {}
	virtual void onLevelUpdate(int deltaTime) {}
	virtual void onLevelRender() {}
	virtual int nextLevelId() const = 0;

	virtual bool showExitHint() const { return true; }
	virtual std::string introTitle() const { return "STAGE"; }
	void startOutro();  
	void startBossClear();

	TileMap *map = nullptr;
	Player *player = nullptr;
	GUI *gui = nullptr;

	template <typename T>
	T& addEnemy() {
		auto ptr = std::make_unique<T>();
		T& ref = *ptr;
		enemies.push_back(std::move(ptr));
		return ref;
	}
	std::vector<std::unique_ptr<Enemy>> enemies;

	int timer = 240;

	BombManager* bombs = nullptr;
	PowerUpManager* powerUps = nullptr;
	Camera* cam = nullptr;        

private:
	std::string m_levelFile;
	bool isGameOver = false;
	bool doorOpened = false;
	bool exitJinglePlaying = false;

	int gameOverTimer = 30;
	AudioManager::SoundId jingleID = 0;

	bool enemyPlayerCollision() const;
	void hurtPlayer(bool exploded);

	enum Phase { INTRO_HOLD, INTRO_REVEAL, PLAYING, OUTRO_WIPE, OUTRO_HOLD };
	Phase phase = INTRO_HOLD;
	int phaseTimer = 0;       // ms spent in the current phase
	int playStartTime = 0;    // Scene::currentTime when gameplay began

	ScreenWipe* wipe = nullptr;
	Text* introText = nullptr;
	Text* exitText = nullptr;
	int exitTimer = 0;   // ms of flashing left

	void updateTransition(int deltaTime, int currentTime);
	void renderTransition();
	glm::vec2 introTextPos() const;

	virtual void playSceneMusic();
};


#endif // _LEVEL_SCENE_INCLUDE

