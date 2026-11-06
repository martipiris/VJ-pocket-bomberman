#include <algorithm>
#include <iostream>
#include "BombManager.h"

BombManager::BombManager(TileMap* map, ShaderProgram& program)
	: map(map), program(program)
{
	if (!texture.loadFromFile("images/bomb.png", TEXTURE_PIXEL_FORMAT_RGBA))
		std::cerr << "Could not load images/bomb.png" << std::endl;
}

bool BombManager::tryPlace(const glm::ivec2& tile)
{
	if (int(bombs.size()) >= maximum()) return false;
	for (const auto& b : bombs)
		if (b->getTile() == tile) return false;
	if (map->isTileSolid(map->tile2Pixel(tile))) return false;

	const int range = Stats::getInstance().getBombRange();   // each bomb keeps the range it had when placed
	bombs.push_back(std::make_unique<Bomb>(tile, range, map, &texture, program));
	Game::instance().audio().playSound("bomb_place");
	return true;
}

void BombManager::update(bool isGameOver, int deltaTime, const glm::ivec2& boxPos, const glm::ivec2& feet, const glm::ivec2& boxSize)
{
	if (!isGameOver && Game::instance().isKeyPressedOnce(CONFIRM_KEY))
		tryPlace(map->pixel2Tile(feet));

	for (auto& b : bombs) {
		if (b->update(deltaTime) && !isGameOver) {
			std::vector<BlastTile> blast = computeBlast(*b);
			b->detonate(blast);
			Game::instance().audio().playSound("bomb_explode");

			for (const BlastTile& bt : blast)
				if (map->destroyTile(bt.pos))        
					destroyedTiles.push_back(bt.pos);
		}
		b->updateSolidity(boxPos, boxSize);
	}
	bombs.erase(std::remove_if(bombs.begin(), bombs.end(),
		[](const std::unique_ptr<Bomb>& b) { return b->isDone(); }), bombs.end());
}

void BombManager::render(const glm::mat4& view)
{
	for (auto& b : bombs) b->render(view);
}

bool BombManager::blocked(const glm::ivec2& t, const Bomb* self) const
{
	const glm::ivec2 size = map->getMapSize();
	if (t.x < 0 || t.x >= size.x || t.y < 0 || t.y >= size.y) return true;
	if (map->isTileSolid(map->tile2Pixel(t))) return true;       // wall (or a solid bomb)

	for (const auto& b : bombs)                                  // any other bomb, solid or not
		if (b.get() != self && b->getTile() == t) return true;
	return false;
}

std::vector<BlastTile> BombManager::computeBlast(const Bomb& src) const
{
	struct Dir { glm::ivec2 d; BlastPart arm, cap; };
	const Dir dirs[4] = {
		{ glm::ivec2(1,  0), ARM_H, CAP_RIGHT },
		{ glm::ivec2(-1,  0), ARM_H, CAP_LEFT  },
		{ glm::ivec2(0,  1), ARM_V, CAP_DOWN  },
		{ glm::ivec2(0, -1), ARM_V, CAP_UP    },
	};

	std::vector<BlastTile> tiles;
	tiles.push_back({ src.getTile(), CENTER });

	for (const Dir& dir : dirs) {
		std::vector<glm::ivec2> ray;
		for (int i = 1; i <= src.getRange(); ++i) {
			glm::ivec2 t = src.getTile() + dir.d * i;

			if (map->isDestructible(t)) {      // reaches the breakable tile, destroys it, stops
				ray.push_back(t);
				break;
			}
			if (blocked(t, &src)) break;
			ray.push_back(t);
		}
		// The last tile of each ray is the tip, even if a wall cut the ray short
		for (size_t k = 0; k < ray.size(); ++k)
			tiles.push_back({ ray[k], (k + 1 == ray.size()) ? dir.cap : dir.arm });
	}
	return tiles;
}

bool BombManager::hits(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const
{
	for (const auto& b : bombs)
		if (b->hits(boxPos, boxSize)) return true;
	return false;
}

std::vector<glm::ivec2> BombManager::takeDestroyedTiles()
{
	std::vector<glm::ivec2> out;
	out.swap(destroyedTiles);
	return out;
}