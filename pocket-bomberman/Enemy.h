#ifndef _Enemy_INCLUDE
#define _Enemy_INCLUDE

#include "libs/Sprite.h"
#include "libs/TileMap.h"

#define SIZE_ENEMY glm::ivec2(64, 64)
#define SIZE_ENEMY_COLLISION glm::ivec2(48, 64)
#define ENEMY_COLLISION_OFFSET glm::ivec2(8, 0)

class Enemy {
public:
	Enemy();
	virtual ~Enemy();

	virtual void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) = 0;
	void update(int deltaTime);
	void render(const glm::mat4& view);

	void setPosition(const glm::vec2& pos);
	glm::vec2 getPosition() const;
protected:
	glm::ivec2 posEnemy;

	Texture spritesheet;
	Sprite* sprite;
private:
	virtual void updateMovement() = 0;
};

#endif // _Enemy_INCLUDE


