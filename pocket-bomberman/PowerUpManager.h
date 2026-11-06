#ifndef _POWERUP_MANAGER_INCLUDE
#define _POWERUP_MANAGER_INCLUDE

#include <memory>
#include <random>
#include <vector>
#include "PowerUp.h"

class PowerUpManager {
public:
	PowerUpManager(TileMap* map, ShaderProgram& program, int dropPercent = 60);

	void maybeSpawn(const glm::ivec2& tile);   // Rolls the dice for a drop at this tile
	void render(const glm::mat4& view);

	// Removes the power-ups the box touches and returns their types
	std::vector<PowerUpType> collect(const glm::ivec2& boxPos, const glm::ivec2& boxSize);

private:
	TileMap* map;
	ShaderProgram& program;
	Texture texture;
	int dropPercent; // Chance for the powerup to spawn
	std::mt19937 rng;
	std::vector<std::unique_ptr<PowerUp>> powerUps;
};

#endif