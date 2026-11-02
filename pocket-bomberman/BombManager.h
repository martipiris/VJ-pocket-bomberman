#ifndef _BOMB_MANAGER_INCLUDE
#define _BOMB_MANAGER_INCLUDE

#include <memory>
#include <vector>
#include "Bomb.h"

class BombManager {
public:
	BombManager(TileMap* map, ShaderProgram& program, int maxBombs, int range = 1);

	void setRange(int r) { range = r; }
	void increaseRange() { ++range; }
	int getRange() const { return range; }

	bool hits(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const;

	bool tryPlace(const glm::ivec2& tile);
	void update(int deltaTime, const glm::ivec2& boxPos, const glm::ivec2& boxSize);
	void render(const glm::mat4& view);

	int available() const { return maxBombs - int(bombs.size()); }
	int maximum() const { return maxBombs; }

private:
	TileMap* map;
	ShaderProgram& program;
	Texture texture;               // shared by every bomb
	int maxBombs;
	std::vector<std::unique_ptr<Bomb>> bombs;
	int range;
	std::vector<BlastTile> computeBlast(const Bomb& src) const;
	bool blocked(const glm::ivec2& tile, const Bomb* self) const;
};

#endif