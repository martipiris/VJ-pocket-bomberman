#ifndef _Enemy_INCLUDE
#define _Enemy_INCLUDE

#include "libs/Sprite.h"
#include "libs/TileMap.h"
#include "Player.h"

class BombManager;

class Enemy {
public:
	Enemy();
	virtual ~Enemy();

	void init(const glm::ivec2& tileMapPos, TileMap* tileMap, Player* player, ShaderProgram& shaderProgram);

    void update(int deltaTime);
	virtual void render(const glm::mat4& view);
	virtual bool hitsPlayer() const;
	virtual bool onBombHit(const BombManager& bombs);
	void startExploding();

	glm::vec2 getPosition() const;

	virtual glm::ivec2 getCollisionSize() const;
	virtual glm::ivec2 getCollisionOffset() const;
protected:
	int state;
	int timer;
	int lastDeltaTime = 16;

	glm::ivec2 posEnemy;

	glm::ivec2 enemySize = glm::ivec2(64, 64);
	glm::ivec2 enemyCollision = glm::ivec2(48, 64);
	glm::ivec2 enemyCollisionOffset = glm::ivec2(8, 0);

	TileMap* map;
	Player* player;

	Texture spritesheet;
	Sprite* sprite;

    ShaderProgram texProgram;
	bool isExploding() const { return exploding; }
	virtual void updateWhileExploding(int deltaTime);
private:
	bool exploding = false;
	int explosionTimer = 0;
	virtual void init() = 0;
	virtual void updateMovement() = 0;
};

#endif // _Enemy_INCLUDE


