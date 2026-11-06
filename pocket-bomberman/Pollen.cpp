#include "Pollen.h"

Pollen::Pollen(const glm::vec2& position, const glm::vec2& direction, ShaderProgram& shaderProgram, int lifetime)
    : position(position), velocity(direction * 4.f), lifetime(lifetime)
{
    texture.loadFromFile("images/bullet.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(size, glm::vec2(0.5f, 1.f), &texture, &shaderProgram);

    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 6);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(0, glm::vec2(0.5f, 0.f));

    sprite->changeAnimation(0);
    sprite->setPosition(position);
}

Pollen::~Pollen() {
    delete sprite;
}

bool Pollen::update(int deltaTime, const glm::ivec2& mapSize) {
    age += deltaTime;
    sprite->update(deltaTime);
    position += velocity;
    sprite->setPosition(position);
    return (lifetime <= 0 || age < lifetime) &&
        position.x + size.x > 0 && position.y + size.y > 0 &&
        position.x < mapSize.x && position.y < mapSize.y;
}

void Pollen::render(const glm::mat4& view) {
    sprite->render(view);
}

bool Pollen::hitsPlayer(const glm::ivec2& playerPosition) const {
    const glm::ivec2 playerBox = playerPosition + PLAYER_COLLISION_OFFSET;
    return position.x < playerBox.x + SIZE_PLAYER_COLLISION.x &&
        position.x + size.x > playerBox.x &&
        position.y < playerBox.y + SIZE_PLAYER_COLLISION.y &&
        position.y + size.y > playerBox.y;
}
