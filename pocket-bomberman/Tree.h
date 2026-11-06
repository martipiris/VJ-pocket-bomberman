#ifndef _TREE_INCLUDE
#define _TREE_INCLUDE

#include "Enemy.h"
#include "LeafEnemy.h"
#include "Pollen.h"
#include <memory>
#include <vector>

class Tree : public Enemy {
public:
	Tree();
	~Tree();

	void init() override;

	void updateMovement() override;
	void render(const glm::mat4& view) override;
	bool hitsPlayer() const override;
	bool onBombHit(const BombManager& bombs) override;

	glm::ivec2 getCollisionSize() const override;
	glm::ivec2 getCollisionOffset() const override;
private:
	glm::ivec2 initialPos;
	glm::ivec2 targetPos;
	int attackTimer;
	int attackCooldown;
	int returnTimer;
	int attackRound;
    int hitsRemaining;
	int hitFlashTimer;
	int deathTimer;
	int petalSpawnTimer;
	bool dying;
	std::vector<std::unique_ptr<Pollen>> pollen;
	std::vector<std::unique_ptr<Pollen>> petals;
	std::vector<std::unique_ptr<LeafEnemy>> leaves;

	void startPollenRound();
	void startLeafRound();
	void beginDeath();
	void spawnPetal();

	glm::ivec2 enemySize = glm::ivec2(128, 128);
	glm::ivec2 enemyCollision = glm::ivec2(96, 128);
	glm::ivec2 enemyCollisionOffset = glm::ivec2(16, 0);
};

#endif // _TREE_INCLUDE