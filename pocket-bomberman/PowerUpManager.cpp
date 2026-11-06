#include <iostream>
#include "PowerUpManager.h"

PowerUpManager::PowerUpManager(TileMap* map, ShaderProgram& program, int dropPercent)
	: map(map), program(program), dropPercent(dropPercent), rng(std::random_device{}())
{
	if (!texture.loadFromFile("images/powerups.png", TEXTURE_PIXEL_FORMAT_RGBA))
		std::cerr << "Could not load images/powerups.png" << std::endl;
}

void PowerUpManager::maybeSpawn(const glm::ivec2& tile)
{
	// Random check to see if powerup spawns
	if (int(rng() % 100) >= dropPercent) return;

	// Weight chances for every pwerup, in order of the enum
	static const int weights[POWERUP_COUNT] = {
		30,   // POWERUP_BOMB
		30,   // POWERUP_FIRE
		20,   // POWERUP_SPEED
		5,    // POWERUP_STAR
		15    // POWERUP_HEART
	};

	int total = 0;
	for (int i = 0; i < POWERUP_COUNT; ++i)
		total += weights[i];

	int roll = int(rng() % total);
	PowerUpType type = POWERUP_BOMB;
	for (int i = 0; i < POWERUP_COUNT; ++i) {
		if (roll < weights[i]) {
			type = PowerUpType(i);
			break;
		}
		roll -= weights[i];
	}

	powerUps.push_back(std::make_unique<PowerUp>(type, tile, map, &texture, program));
}

void PowerUpManager::render(const glm::mat4& view)
{
	for (auto& p : powerUps) p->render(view);
}

std::vector<PowerUpType> PowerUpManager::collect(const glm::ivec2& boxPos, const glm::ivec2& boxSize)
{
	std::vector<PowerUpType> got;
	for (auto it = powerUps.begin(); it != powerUps.end(); ) {
		if ((*it)->overlaps(boxPos, boxSize)) {
			got.push_back((*it)->getType());
			it = powerUps.erase(it);
		}
		else {
			++it;
		}
	}
	return got;
}