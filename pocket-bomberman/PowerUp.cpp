#include "PowerUp.h"

namespace {
	const int PICKUP_MARGIN = 8;

	const int SHEET_COLS = 4;
	const int SHEET_ROWS = 4;
	const glm::vec2 CELL(1.f / SHEET_COLS, 1.f / SHEET_ROWS);   // (0.25, 0.25)

	glm::vec2 cellOffset(int index) {
		return glm::vec2((index % SHEET_COLS) * CELL.x,
			(index / SHEET_COLS) * CELL.y);
	}
}

PowerUp::PowerUp(PowerUpType type, const glm::ivec2& tile, TileMap* map, Texture* tex, ShaderProgram& program)
	: type(type), size(map->getTileSize())
{
	sprite.reset(Sprite::createSprite(glm::vec2(size), CELL, tex, &program));
	sprite->setNumberAnimations(1);
	sprite->setAnimationSpeed(0, 1);
	sprite->addKeyframe(0, cellOffset(int(type)));   // cell index = enum value
	sprite->changeAnimation(0);

	pos = map->tile2Pixel(tile);
	sprite->setPosition(glm::vec2(pos));
}

void PowerUp::render(const glm::mat4& view)
{
	sprite->render(view);
}

bool PowerUp::overlaps(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const
{
	const glm::ivec2 p = pos + glm::ivec2(PICKUP_MARGIN);
	const int s = size - 2 * PICKUP_MARGIN;
	return boxPos.x < p.x + s && boxPos.x + boxSize.x > p.x &&
		boxPos.y < p.y + s && boxPos.y + boxSize.y > p.y;
}