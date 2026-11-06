#ifndef _POLLEN_INCLUDE
#define _POLLEN_INCLUDE

#include "libs/Sprite.h"
#include "Player.h"

class Pollen {
public:
    Pollen(const glm::vec2& position, const glm::vec2& direction, ShaderProgram& shaderProgram, int lifetime = 0);
    ~Pollen();

    bool update(int deltaTime, const glm::ivec2& mapSize);
    void render(const glm::mat4& view);
    bool hitsPlayer(const glm::ivec2& playerPosition) const;

private:
    Texture texture;
    Sprite* sprite = nullptr;
    glm::vec2 position;
    glm::vec2 velocity;
    glm::ivec2 size = glm::ivec2(32, 32);
    int lifetime;
    int age = 0;
};

#endif // _POLLEN_INCLUDE
