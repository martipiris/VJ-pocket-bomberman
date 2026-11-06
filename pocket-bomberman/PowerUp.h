#ifndef _POWERUP_INCLUDE
#define _POWERUP_INCLUDE

#include <memory>
#include <glm/glm.hpp>
#include "libs/Sprite.h"
#include "libs/TileMap.h"

//All different powerups directly tied to their positions in the spritesheet
enum PowerUpType {
	POWERUP_BOMB,    
	POWERUP_FIRE,    
	POWERUP_SPEED,   
	POWERUP_STAR,    
	POWERUP_HEART,   
	POWERUP_COUNT
};

class PowerUp {
public:
	PowerUp(PowerUpType type, const glm::ivec2& tile, TileMap* map, Texture* tex, ShaderProgram& program);

	void render(const glm::mat4& view);
	bool overlaps(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const;

	PowerUpType getType() const { return type; }

private:
	PowerUpType type;
	glm::ivec2 pos;   
	int size;
	std::unique_ptr<Sprite> sprite;
};

#endif