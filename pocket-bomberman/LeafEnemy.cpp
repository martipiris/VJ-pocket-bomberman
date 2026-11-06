#include "LeafEnemy.h"
#include "LeafEnemy.h"
#include <algorithm>
#include <cmath>

enum leafAnims {
    EXPLODE, MOVE_LEFT, MOVE_RIGHT
};

LeafEnemy::LeafEnemy(int fallDirection)
    : fallDirection(fallDirection)
{
    enemySize = leafSize;
    enemyCollision = leafSize;
    enemyCollisionOffset = glm::ivec2(0, 0);
}

void LeafEnemy::init() {
    spritesheet.loadFromFile("images/bug.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(leafSize, glm::vec2(0.25f, 0.5f), &spritesheet, &texProgram);
    sprite->setNumberAnimations(3);

    sprite->setAnimationSpeed(MOVE_LEFT, 4);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.25f, 0.f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 4);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.f, 0.5f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25f, 0.5f));

    sprite->setAnimationSpeed(EXPLODE, 3);
    sprite->addKeyframe(EXPLODE, glm::vec2(0.5f, 0.f));
    sprite->addKeyframe(EXPLODE, glm::vec2(0.75f, 0.f));

    sprite->changeAnimation(fallDirection > 0 ? MOVE_RIGHT : MOVE_LEFT);

    startPosition = posEnemy;
    const int tileSize = map->getTileSize();
    const glm::ivec2 mapPixels = map->getMapPixelSize();
    const int landingX = std::max(0, std::min(mapPixels.x - leafSize.x,
        startPosition.x + fallDirection * tileSize * 3));
    const int centerX = landingX + leafSize.x / 2;
    int floorY = mapPixels.y - tileSize;

    landingPosition = glm::ivec2(landingX, floorY - leafSize.y);
    if (landingPosition.y < startPosition.y)
        landingPosition.y = mapPixels.y - leafSize.y;
}

void LeafEnemy::updateMovement() {
    if (!landed) {
        const float t = std::min(1.f, float(++fallTimer) / 45.f);
        posEnemy.x = startPosition.x + int((landingPosition.x - startPosition.x) * t);
        posEnemy.y = startPosition.y + int((landingPosition.y - startPosition.y) * t * t);
        if (t >= 1.f) {
            posEnemy = landingPosition;
            landed = true;
        }
        return;
    }

    const int playerCenter = int(player->getPosition().x) + SIZE_PLAYER.x / 2;
    const int leafCenter = posEnemy.x + leafSize.x / 2;
    if (std::abs(playerCenter - leafCenter) <= 1)
        return;
    const int direction = playerCenter < leafCenter ? -1 : 1;
   const int animation = direction < 0 ? MOVE_LEFT : MOVE_RIGHT;
    if (sprite->animation() != animation)
        sprite->changeAnimation(animation);
    const int nextX = posEnemy.x + direction;
    const int supportX = nextX + leafSize.x / 2;
    if (map->isTileSolid(glm::ivec2(supportX, posEnemy.y + leafSize.y)))
        posEnemy.x = nextX;
}

glm::ivec2 LeafEnemy::getCollisionSize() const {
    return leafSize;
}

glm::ivec2 LeafEnemy::getCollisionOffset() const {
    return glm::ivec2(0, 0);
}

bool LeafEnemy::hitsPlayer() const {
   if (isExploding())
        return false;
    const glm::ivec2 playerBox = glm::ivec2(player->getPosition()) + PLAYER_COLLISION_OFFSET;
    return posEnemy.x < playerBox.x + SIZE_PLAYER_COLLISION.x &&
        posEnemy.x + leafSize.x > playerBox.x &&
        posEnemy.y < playerBox.y + SIZE_PLAYER_COLLISION.y &&
        posEnemy.y + leafSize.y > playerBox.y;
}
