#ifndef _Enemy_INCLUDE
#define _Enemy_INCLUDE

#include "libs/Sprite.h"
#include "libs/TileMap.h"
#include "Player.h"

class Enemy {
public:
	Enemy();
	virtual ~Enemy();

	void init(const glm::ivec2& tileMapPos, TileMap* tileMap, Player* player, ShaderProgram& shaderProgram);

	void update(int deltaTime);
	void render(const glm::mat4& view);

	glm::vec2 getPosition() const;

	virtual glm::ivec2 getCollisionSize() const;
	virtual glm::ivec2 getCollisionOffset() const;
protected:
	int state;
	int timer;

	glm::ivec2 posEnemy;

	glm::ivec2 enemySize = glm::ivec2(64, 64);
	glm::ivec2 enemyCollision = glm::ivec2(48, 64);
	glm::ivec2 enemyCollisionOffset = glm::ivec2(8, 0);

	TileMap* map;
	Player* player;

	Texture spritesheet;
	Sprite* sprite;

	ShaderProgram texProgram;
private:
	virtual void init() = 0;
	virtual void updateMovement() = 0;
};

#endif // _Enemy_INCLUDE


