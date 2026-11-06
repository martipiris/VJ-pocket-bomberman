#ifndef _LEAF_ENEMY_INCLUDE
#define _LEAF_ENEMY_INCLUDE

#include "Enemy.h"

class LeafEnemy : public Enemy {
public:
    explicit LeafEnemy(int fallDirection);
    ~LeafEnemy() override = default;

    glm::ivec2 getCollisionSize() const override;
    glm::ivec2 getCollisionOffset() const override;
    bool hitsPlayer() const override;

private:
    void init() override;
    void updateMovement() override;

    int fallDirection;
    int fallTimer = 0;
    bool landed = false;
    glm::ivec2 startPosition;
    glm::ivec2 landingPosition;
    glm::ivec2 leafSize = glm::ivec2(64, 64);
};

#endif // _LEAF_ENEMY_INCLUDE
