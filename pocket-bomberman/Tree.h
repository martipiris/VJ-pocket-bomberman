#ifndef _TREE_INCLUDE
#define _TREE_INCLUDE

#include "Enemy.h"

class Tree : public Enemy {
public:
	Tree();
	~Tree();

	void init() override;

	void updateMovement() override;

	glm::ivec2 getCollisionSize() const override;
	glm::ivec2 getCollisionOffset() const override;
private:
	glm::ivec2 initialPos;
	glm::ivec2 targetPos;
	int attackTimer;
	int attackCooldown;
	int returnTimer;

	glm::ivec2 enemySize = glm::ivec2(128, 128);
	glm::ivec2 enemyCollision = glm::ivec2(96, 128);
	glm::ivec2 enemyCollisionOffset = glm::ivec2(16, 0);
};

#endif // _TREE_INCLUDE