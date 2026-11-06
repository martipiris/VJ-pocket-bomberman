#include "Bomb.h"

namespace {
	const int FUSE_TIME = 2000;
	const int EXPLOSION_TIME = 300;   // 3 phases x 100 ms
	const int HIT_MARGIN = 6;

	// Sheet: 3 columns (animation frames) x 8 rows (pieces)
	const int SHEET_COLS = 3;
	const int SHEET_ROWS = 8;
	const glm::vec2 CELL(1.f / SHEET_COLS, 1.f / SHEET_ROWS);

	glm::vec2 cell(int col, int row) { return glm::vec2(col * CELL.x, row * CELL.y); }

	const int ROW_BOMB = 0;
	// Blast rows follow BlastPart order: row = 1 + part
	int blastRow(BlastPart p) { return 1 + int(p); }
}

Bomb::Bomb(const glm::ivec2& tile, int range, TileMap* map, Texture* tex, ShaderProgram& program)
	: tile(tile), range(range), map(map), texture(tex), program(&program),
	state(FUSE), timer(FUSE_TIME), solid(false)
{
	// 2x2 sheet: top row = fuse frames, bottom row = explosion frames
	const int ts = map->getTileSize();
	fuseSprite.reset(Sprite::createSprite(glm::vec2(ts), CELL, tex, &program));
	fuseSprite->setNumberAnimations(1);
	fuseSprite->setAnimationSpeed(0, 8);              // 8 frames/s -> one full pulse every 500 ms
	fuseSprite->addKeyframe(0, cell(0, ROW_BOMB));    // sprite 1
	fuseSprite->addKeyframe(0, cell(1, ROW_BOMB));    // sprite 2
	fuseSprite->addKeyframe(0, cell(2, ROW_BOMB));    // sprite 3
	fuseSprite->addKeyframe(0, cell(1, ROW_BOMB));    // sprite 2
	fuseSprite->changeAnimation(0);

	fuseSprite->setPosition(glm::vec2(map->tile2Pixel(tile)));
}

Bomb::~Bomb() {}   // unique_ptrs clean up the sprites; don't touch the map here

bool Bomb::update(int deltaTime)
{
	if (state == DONE) return false;

	timer -= deltaTime;

	if (state == FUSE) {
		fuseSprite->update(deltaTime);
		return timer <= 0;
	}

	for (auto& s : blastSprites) s->update(deltaTime);
	if (timer <= 0) state = DONE;
	return false;
}

void Bomb::detonate(const std::vector<BlastTile>& tiles)
{
	state = EXPLODING;
	timer = EXPLOSION_TIME;

	if (solid) {
		map->setDynamicSolid(tile, false);
		solid = false;
	}

	blastTiles = tiles;
	blastSprites.clear();

	const int ts = map->getTileSize();

	for (const BlastTile& bt : blastTiles) {
		std::unique_ptr<Sprite> s(Sprite::createSprite(glm::vec2(ts), CELL, texture, program));
		s->setNumberAnimations(1);
		s->setAnimationSpeed(0, 10);                       // 10 frames/s -> 100 ms per phase
		s->addKeyframe(0, cell(0, blastRow(bt.part)));    // full explosion
		s->addKeyframe(0, cell(1, blastRow(bt.part)));    // mid fizzling out
		s->addKeyframe(0, cell(2, blastRow(bt.part)));    // almost fizzled out
		s->changeAnimation(0);
		s->setPosition(glm::vec2(map->tile2Pixel(bt.pos)));
		blastSprites.push_back(std::move(s));
	}
}

void Bomb::updateSolidity(const glm::ivec2& boxPos, const glm::ivec2& boxSize)
{
	if (solid || state != FUSE) return;

	const int ts = map->getTileSize();
	const glm::ivec2 tp = map->tile2Pixel(tile);
	const bool overlap = boxPos.x < tp.x + ts && boxPos.x + boxSize.x > tp.x &&
		boxPos.y < tp.y + ts && boxPos.y + boxSize.y > tp.y;

	if (!overlap) {
		solid = true;
		map->setDynamicSolid(tile, true);
	}
}

bool Bomb::hits(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const
{
	if (state != EXPLODING) return false;

	const int ts = map->getTileSize();
	for (const BlastTile& bt : blastTiles) {
		const glm::ivec2 tp = map->tile2Pixel(bt.pos) + glm::ivec2(HIT_MARGIN);
		const int size = ts - 2 * HIT_MARGIN;
		if (boxPos.x < tp.x + size && boxPos.x + boxSize.x > tp.x &&
			boxPos.y < tp.y + size && boxPos.y + boxSize.y > tp.y)
			return true;
	}
	return false;
}

void Bomb::render(const glm::mat4& view)
{
	if (state == FUSE) {
		fuseSprite->render(view);
	}
	else if (state == EXPLODING) {
		for (auto& s : blastSprites) s->render(view);
	}
}