#ifndef _BOMB_INCLUDE
#define _BOMB_INCLUDE

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "libs/Sprite.h"
#include "libs/TileMap.h"

#define SIZE_BOMB glm::ivec2(64, 64)

 //Different parts of a bomb explosion to be rendered individually and pieced together
enum BlastPart { CENTER, ARM_H, ARM_V, CAP_RIGHT, CAP_LEFT, CAP_DOWN, CAP_UP };

struct BlastTile {
	glm::ivec2 pos;
	BlastPart part;
};

class Bomb {
public:
	enum State { FUSE, EXPLODING, DONE };

	Bomb(const glm::ivec2& tile, int range, TileMap* map, Texture* tex, ShaderProgram& program);
	~Bomb();

	bool update(int deltaTime); // Returns true on the frame the fuse runs out 
	void detonate(const std::vector<BlastTile>& tiles);
	void render(const glm::mat4& view);
	void updateSolidity(const glm::ivec2& boxPos, const glm::ivec2& boxSize);
	bool hits(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const;

	glm::ivec2 getTile() const { return tile; }
	int getRange() const { return range; }
	bool isDone() const { return state == DONE; }

private:
	glm::ivec2 tile;
	int range;
	TileMap* map;
	Texture* texture;
	ShaderProgram* program;

	std::unique_ptr<Sprite> fuseSprite;
	std::vector<BlastTile> blastTiles;
	std::vector<std::unique_ptr<Sprite>> blastSprites;

	State state;
	int timer;
	bool solid;
};

#endif