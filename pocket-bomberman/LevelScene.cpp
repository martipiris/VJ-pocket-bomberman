#include <iostream>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include "LevelScene.h"
#include "StartingScene.h"
#include <algorithm>

namespace {
	//Duration of transitions
	const int INTRO_HOLD_MS = 2000;
	const int INTRO_REVEAL_MS = 700;
	const int OUTRO_WIPE_MS = 700;
	const int OUTRO_HOLD_MS = 700;
	//Exit text parameters
	const int INTRO_TEXT_SIZE = 60;
	const glm::vec4 WIPE_COLOR(0.12f, 0.12f, 0.13f, 1.f);
	const int EXIT_FLASH_MS = 3000;   // Total time the text is active
	const int EXIT_BLINK_MS = 300;    // Visible/hidden interval
	const int EXIT_TEXT_SIZE = 60;
	const int EXIT_OUTLINE_PX = 3;    // Outline thickness
	const glm::vec4 EXIT_OUTLINE_COLOR(0.f, 0.f, 0.f, 1.f);
	const glm::vec4 EXIT_COLOR(0.7f, 0.25f, 0.9f, 1.f);
}

LevelScene::LevelScene(const std::string &levelFile)
	: m_levelFile(levelFile), map(nullptr)
{
}

LevelScene::~LevelScene()
{
	deleteObjects();
}

void LevelScene::initObjects()
{
	const std::string file = getLevelFile();
	const glm::vec2 minCoords = glm::vec2(SCREEN_OFFSET_X, SCREEN_OFFSET_Y);
	map = TileMap::createTileMap(file, minCoords, texProgram);

	bombs = new BombManager(map, texProgram);
	powerUps = new PowerUpManager(map, texProgram);

	player = new Player();
	player->init(glm::ivec2(SCREEN_OFFSET_X, SCREEN_OFFSET_Y), texProgram);
	player->setPosition(getInitialPlayerTilePos());
	player->setTileMap(map);

	cam = new Camera(SCREEN_WIDTH, SCREEN_HEIGHT, map->getMapPixelSize(), player->getPosition());
	gui = new GUI(simpleProgram, texProgram);
	gui->init();

	onLevelInit();

	wipe = new ScreenWipe(simpleProgram);
	introText = new Text();
	introText->init(MAIN_FONT);
	exitText = new Text();
	exitText->init(MAIN_FONT);
	cam->update();
	gui->updateTimer(timer);
	gui->setEnemyCount(enemies.size());
	gui->render();
}

void LevelScene::updateObjects(int deltaTime, int currentTime)
{
	if (phase != PLAYING) {
		updateTransition(deltaTime, currentTime);
		return;
	}
	if (exitJinglePlaying) {
		player->update(deltaTime);
		if (Game::instance().audio().isSoundFinished(jingleID))
			startOutro();
		return;
	}

	//Check if player touches enemy
	if (enemyPlayerCollision()) {
		hurtPlayer(false);
	}

	//Game over handler
	if (isGameOver) {
		if (gameOverTimer > 0) {
			if (gameOverTimer == 1)
				jingleID = Game::instance().audio().playSound("death");

			--gameOverTimer;
		} else {
			if (Game::instance().audio().isSoundFinished(jingleID)) {
				if (Stats::getInstance().getLives() == 0) {
					Game::instance().changeScene(STARTING);
				} else {
					Stats::getInstance().decreaseLives();
					Game::instance().changeScene(id());
				}
				return;
			}
		}
	}

	if (!isGameOver && gui->updateTimer(std::max(0, int(timer - (currentTime - playStartTime) / 1000))) == 0) {
		hurtPlayer(false);
	}

	//Change the enemy count in the GUI
	gui->setEnemyCount(enemies.size());

	//Update player characer
	player->update(deltaTime);

	const glm::vec2 playerPos = player->getPosition();
	const glm::ivec2 boxPos = glm::ivec2(playerPos) + PLAYER_COLLISION_OFFSET;
	const glm::ivec2 center(boxPos.x + SIZE_PLAYER_COLLISION.x / 2, boxPos.y + SIZE_PLAYER_COLLISION.y / 2);

	bombs->update(isGameOver, deltaTime, boxPos, center, SIZE_PLAYER_COLLISION);

	// Destroyed tiles may drop a power-up
	for (const glm::ivec2& t : bombs->takeDestroyedTiles())
		powerUps->maybeSpawn(t);

	// Pickups
	if (!isGameOver) {
		Stats& stats = Stats::getInstance();
		for (PowerUpType type : powerUps->collect(boxPos, SIZE_PLAYER_COLLISION)) {
			switch (type) {
			case POWERUP_BOMB:  stats.increaseBombAmount(); break;
			case POWERUP_FIRE:  stats.increaseBombRange();  break;
			case POWERUP_SPEED: stats.increaseSpeed();      break;
			case POWERUP_STAR:  player->setInvincible(10000); break;   // 10 s
			case POWERUP_HEART: stats.setHeart(true); break;
			default: break;
			}
		}
	}

	// Explosion kills the player
	if (bombs->hits(boxPos, SIZE_PLAYER_COLLISION)) {
		hurtPlayer(true);
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
		[&](const std::unique_ptr<Enemy>& e) {
          return e->onBombHit(*bombs);
		}), enemies.end());

	// Debug: K kills every enemy
	if (!isGameOver && Game::instance().isKeyPressedOnce(GLFW_KEY_K)) {
		for (auto& e : enemies)
			e->startExploding();
	}

	// All enemies dead -> open the door
	if (!isGameOver && !doorOpened && enemies.empty()) {
		map->openDoor();
		doorOpened = true;
		if (map->hasDoor() && showExitHint())
			exitTimer = EXIT_FLASH_MS;
	}

 // Reach the center of the open door to play the exit jingle and win animation
	if (!isGameOver && map->touchesOpenDoor(glm::ivec2(boxPos.x - PLAYER_COLLISION_OFFSET.x, boxPos.y - PLAYER_COLLISION_OFFSET.y), SIZE_PLAYER_COLLISION)) {
		exitTimer = 0;
		player->startWinningAnimation();
		jingleID = Game::instance().audio().playSound("stage_clear");
		Game::instance().audio().stopSound(musicId);
		exitJinglePlaying = true;
		return;
	}

	if (exitTimer > 0)
		exitTimer = std::max(0, exitTimer - deltaTime);

	cam->setTarget(player->getPosition());
	cam->update();

	for (auto& enemy : enemies) {
		enemy->update(deltaTime, isGameOver);
	}

	onLevelUpdate(deltaTime);
}

void LevelScene::renderObjects()
{
	glm::mat4 view = cam->getViewMatrix();
	texProgram.setUniformMatrix4f("modelview", view);
	// simpleProgram.setUniformMatrix4f("modelview", view);

	map->render();
	powerUps->render(view);
	bombs->render(view);
	player->render(view);

	for (auto& enemy : enemies) {
		enemy->render(view);
	}

	gui->render();
	//Render for the EXIT text after enemies died
	if (phase == PLAYING && exitTimer > 0 && (exitTimer / EXIT_BLINK_MS) % 2 == 0) {
		const std::string msg = "EXIT";
		const float w = exitText->getWidth(msg, EXIT_TEXT_SIZE);
		const float x = (SCREEN_WIDTH - w) * 0.5f;
		const float y = SCREEN_HEIGHT * 0.5f + EXIT_TEXT_SIZE * 0.3f;

		// Outline exit text
		for (int dx = -1; dx <= 1; ++dx) {
			for (int dy = -1; dy <= 1; ++dy) {
				if (dx == 0 && dy == 0) continue;
				exitText->render(msg,
					glm::vec2(x + dx * EXIT_OUTLINE_PX, y + dy * EXIT_OUTLINE_PX),
					EXIT_TEXT_SIZE, EXIT_OUTLINE_COLOR);
			}
		}

		// Exit text
		exitText->render(msg, glm::vec2(x, y), EXIT_TEXT_SIZE, EXIT_COLOR);
	}
	renderTransition();
	onLevelRender();
}

void LevelScene::deleteObjects() {
	delete powerUps; powerUps = nullptr;
	delete bombs;  bombs = nullptr;
	delete map;    map = nullptr;
	delete gui;    gui = nullptr;
	delete player; player = nullptr;
	delete cam;    cam = nullptr;
	delete wipe;      wipe = nullptr;
	delete introText; introText = nullptr;
	delete exitText; exitText = nullptr;
}

std::string LevelScene::getLevelFile() const {
	return m_levelFile;
}

glm::vec2 LevelScene::getInitialPlayerTilePos() const {
	if (map == nullptr) {
		std::cerr << "Error: map is null in getInitialPlayerTilePos()" << std::endl;
		return glm::vec2(0.f, 0.f);
	}

	glm::vec2 pos = getPlayerInitialTile();
	return glm::vec2(pos.x * map->getTileSize(), pos.y * map->getTileSize());
}

 //Controller for colisions between player and enemies 
bool LevelScene::enemyPlayerCollision() const {
	const glm::ivec2 playerBox = glm::ivec2(player->getPosition()) + PLAYER_COLLISION_OFFSET;
	for (const auto& enemy : enemies) {
       if (enemy->hitsPlayer())
			return true;
		const glm::ivec2 enemyBox = glm::ivec2(enemy->getPosition()) + enemy->getCollisionOffset();
		if (playerBox.x < enemyBox.x + enemy->getCollisionSize().x &&
			playerBox.x + SIZE_PLAYER_COLLISION.x > enemyBox.x &&
			playerBox.y < enemyBox.y + enemy->getCollisionSize().y &&
			playerBox.y + SIZE_PLAYER_COLLISION.y > enemyBox.y) {
			return true;
		}
	}
	return false;
}
void LevelScene::hurtPlayer(bool exploded) {
	if (isGameOver || player->isInvincible()) return;

	Stats& stats = Stats::getInstance();
	if (stats.hasHeart()) {
		player->setInvincible(3000);  // 2 s of grace, with blinking
		stats.setHeart(false);        // the extra hit absorbs the damage
		return;
	}

	Game::instance().audio().playSound(exploded ? "bomberman_explode" : "bomberman_hit");
	Game::instance().audio().stopSound(musicId);
	player->setDead(exploded);
	isGameOver = true;
}

 //Handler for the level transitions
void LevelScene::updateTransition(int deltaTime, int currentTime)
{
	phaseTimer += deltaTime;

	switch (phase) {
	case INTRO_HOLD:
		if (phaseTimer >= INTRO_HOLD_MS) { phase = INTRO_REVEAL; phaseTimer = 0; }
		break;
	case INTRO_REVEAL:
		if (phaseTimer >= INTRO_REVEAL_MS) {
			phase = PLAYING;
			phaseTimer = 0;
			playStartTime = currentTime;   // the level clock starts now
			playSceneMusic();
		}
		break;
	case OUTRO_WIPE:
		if (phaseTimer >= OUTRO_WIPE_MS) { phase = OUTRO_HOLD; phaseTimer = 0; }
		break;
	case OUTRO_HOLD:
		if (phaseTimer >= OUTRO_HOLD_MS)
			Game::instance().changeScene(nextLevelId());   // deletes this scene: touch nothing after this
		break;
	default:
		break;
	}
}

void LevelScene::renderTransition()
{
	switch (phase) {
	case INTRO_HOLD:
		wipe->drawSector(0.f, 1.f, WIPE_COLOR);
		introText->render(introTitle(), introTextPos(), INTRO_TEXT_SIZE, glm::vec4(1.f));
		break;
	case INTRO_REVEAL: {
		const float t = std::min(1.f, phaseTimer / float(INTRO_REVEAL_MS));
		wipe->drawSector(t, 1.f, WIPE_COLOR);
		introText->render(introTitle(), introTextPos(), INTRO_TEXT_SIZE, glm::vec4(1.f, 1.f, 1.f, 1.f - t));
		break;
	}
	case OUTRO_WIPE: {
		const float t = std::min(1.f, phaseTimer / float(OUTRO_WIPE_MS));
		wipe->drawSector(0.f, t, WIPE_COLOR);
		break;
	}
	case OUTRO_HOLD:
		wipe->drawSector(0.f, 1.f, WIPE_COLOR);
		break;
	default:
		break;
	}
}

glm::vec2 LevelScene::introTextPos() const
{
	const float width = introText->getWidth(introTitle(), INTRO_TEXT_SIZE);
	return glm::vec2((SCREEN_WIDTH - width) * 0.5f, 250.f);
}

void LevelScene::startOutro()
{
	if (phase != PLAYING || isGameOver) return;
	phase = OUTRO_WIPE;
	phaseTimer = 0;
}

void LevelScene::startBossClear()
{
	if (phase != PLAYING || isGameOver || exitJinglePlaying) return;
	player->startWinningAnimation();
	Game::instance().audio().stopSound(musicId);
	jingleID = Game::instance().audio().playSound("game_clear");
	exitJinglePlaying = true;
}

void LevelScene::playSceneMusic() {
	musicId = Game::instance().audio().playSound("forest_area", true);
}