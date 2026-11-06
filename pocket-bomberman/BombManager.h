#ifndef _BOMB_MANAGER_INCLUDE
#define _BOMB_MANAGER_INCLUDE

#include <memory>
#include <vector>
#include "Bomb.h"
#include "Game.h"
#include "Stats.h"

class BombManager {
public:
	BombManager(TileMap* map, ShaderProgram& program);


	bool hits(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const;

	bool tryPlace(const glm::ivec2& tile);
	void update(bool isGameOver, int deltaTime, const glm::ivec2& boxPos, const glm::ivec2& feet, const glm::ivec2& boxSize);
	void render(const glm::mat4& view);

	int available() const { return maximum() - int(bombs.size()); }
	int maximum() const { return Stats::getInstance().getBombAmount(); }

	std::vector<glm::ivec2> takeDestroyedTiles(); 

private:
	TileMap* map;
	ShaderProgram& program;
	Texture texture;               // Shared by every bomb
	std::vector<std::unique_ptr<Bomb>> bombs; //All the bombs currently placed
	std::vector<BlastTile> computeBlast(const Bomb& src) const;
	bool blocked(const glm::ivec2& tile, const Bomb* self) const;
	std::vector<glm::ivec2> destroyedTiles;
};

#endif