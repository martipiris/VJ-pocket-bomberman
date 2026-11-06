#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "TileMap.h"


namespace {
	const int DESTRUCTIBLE_TILES[] = { 4 }; 
	const int DOOR_CLOSED_TILE = 8;  
	const int DOOR_OPEN_TILE = 16;   
	const int DOOR_MARGIN = 16;       
}


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram &program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);
	return map;
}


TileMap::TileMap(
	const string& levelFile,
	const glm::vec2& minCoords,
	ShaderProgram& program
)
	: vao(0)
	, vbo(0)
	, posLocation(0)
	, texCoordLocation(0)
	, nTiles(0)
	, position(0, 0)
	, mapSize(0, 0)
	, tilesheetSize(0, 0)
	, tileSize(0)
	, blockSize(0)
	, map(nullptr)
	, origin(0, 0)
	, doorTile(-1, -1)
	, doorOpen(false)
{
	if (!loadLevel(levelFile))
	{
		printf("ERROR: Could not load tilemap level file: %s\n", levelFile.c_str());
		return;
	}

	prepareArrays(minCoords, program);
	prepareBackground(minCoords, program);
}

TileMap::~TileMap()
{
	if(map != NULL)
		delete [] map;
	
	backgroundQuad->free();
	delete backgroundQuad;

	free();
}


void TileMap::render() const
{
	// Renderizar fondo
	backgroundQuad->render(backgroundTexture);
	
	// Renderizar tiles
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 6 * nTiles);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::free()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string& levelFile)
{
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	char tile;

	fin.open(levelFile.c_str());

	if (!fin.is_open())
		return false;

	getline(fin, line);

	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;

	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;

	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tileSize >> blockSize;

	if (tileSize <= 0 || blockSize <= 0)
		return false;

	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetFile;

	if (tilesheetFile.empty())
		return false;

	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);

	getline(fin, line);
	sstream.clear();
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;

	if (tilesheetSize.x <= 0 || tilesheetSize.y <= 0)
		return false;

	tileTexSize = glm::vec2(
		1.f / tilesheetSize.x,
		1.f / tilesheetSize.y
	);

	// Cargar textura de fondo
	string backgroundFile = "images/level_background.png";
	
	if (!backgroundFile.empty()) {
		backgroundTexture.loadFromFile(backgroundFile, TEXTURE_PIXEL_FORMAT_RGBA);
		backgroundTexture.setWrapS(GL_REPEAT);
		backgroundTexture.setWrapT(GL_REPEAT);
		backgroundTexture.setMinFilter(GL_NEAREST);
		backgroundTexture.setMagFilter(GL_NEAREST);
	}

	map = new int[mapSize.x * mapSize.y];

	dynamicSolid.assign(mapSize.x * mapSize.y, 0);

	for (int j = 0; j < mapSize.y; ++j)
	{
		for (int i = 0; i < mapSize.x; ++i)
		{
			fin.get(tile);

			if (tile == ' ')
				map[j * mapSize.x + i] = 0;
			else
				map[j * mapSize.x + i] =
				tile >= int('A')
				? tile - int('A') + 10
				: tile - int('0');
		}

		// Consumir el salto de línea.
		fin.get(tile);

#ifndef _WIN32
		fin.get(tile);
#endif
	}
	for (int j = 0; j < mapSize.y; ++j)
		for (int i = 0; i < mapSize.x; ++i)
			if (map[j * mapSize.x + i] == DOOR_CLOSED_TILE)
				doorTile = glm::ivec2(i, j);

	fin.close();

	return true;
}

void TileMap::prepareBackground(const glm::vec2 &minCoords, ShaderProgram &program)
{
	glm::vec2 posPixel = glm::vec2(minCoords.x, minCoords.y);
	glm::ivec2 mapPixelSize = getMapPixelSize();
	
	glm::vec2 geom[2] = {
		glm::vec2(posPixel.x, posPixel.y),
		glm::vec2(posPixel.x + mapPixelSize.x, posPixel.y + mapPixelSize.y)
	};
	
	float repeatX = float(mapPixelSize.x) / tileSize;
	float repeatY = float(mapPixelSize.y) / tileSize;
	
	glm::vec2 texCoords[2] = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(repeatX, repeatY)
	};
	
	backgroundQuad = TexturedQuad::createTexturedQuad(geom, texCoords, program);
}

void TileMap::prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program)
{
	origin = minCoords;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	uploadVertices();

	posLocation = program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = program.bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void TileMap::uploadVertices()
{
	std::vector<float> vertices;
	const float b = float(blockSize);
	nTiles = 0;

	for (int j = 0; j < mapSize.y; j++)
	{
		for (int i = 0; i < mapSize.x; i++)
		{
			const int tile = map[j * mapSize.x + i];
			if (tile == 0) continue;

			++nTiles;
			const glm::vec2 p(origin.x + i * tileSize, origin.y + j * tileSize);
			const glm::vec2 t0(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x,
				float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
			const glm::vec2 t1 = t0 + tileTexSize;

			const float quad[24] = {
				p.x,     p.y,     t0.x, t0.y,
				p.x + b, p.y,     t1.x, t0.y,
				p.x + b, p.y + b, t1.x, t1.y,
				p.x,     p.y,     t0.x, t0.y,
				p.x + b, p.y + b, t1.x, t1.y,
				p.x,     p.y + b, t0.x, t1.y };
			vertices.insert(vertices.end(), quad, quad + 24);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

bool TileMap::isDestructible(const glm::ivec2& tile) const
{
	if (tile.x < 0 || tile.x >= mapSize.x || tile.y < 0 || tile.y >= mapSize.y)
		return false;

	const int id = map[tile.y * mapSize.x + tile.x];
	for (int d : DESTRUCTIBLE_TILES)
		if (id == d) return true;
	return false;
}

bool TileMap::destroyTile(const glm::ivec2& tile)
{
	if (!isDestructible(tile)) return false;

	map[tile.y * mapSize.x + tile.x] = 0;   // collisions read this array, so the tile is now walkable
	uploadVertices();                       // re-upload into the same VBO; the VAO keeps working
	return true;
}
// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(solidAt(x, y))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;
	
	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for(int y=y0; y<=y1; y++)
	{
		if(solidAt(x, y))
			return true;
	}
	
	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size, int* posY) const {
	const int x0 = std::max(0, pos.x / tileSize);
	const int x1 = std::min(
		mapSize.x - 1,
		(pos.x + size.x - 1) / tileSize
	);

	if (x0 > x1)
		return false;

	const int bottom = pos.y + size.y;
	const int row = bottom / tileSize;

	if (row < 0 || row >= mapSize.y)
		return false;

	for (int x = x0; x <= x1; ++x)
	{
		if (solidAt(x, row))
		{
			*posY = row * tileSize - size.y;
			return true;
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
    int x0, x1, y;

    x0 = pos.x / tileSize;
    x1 = (pos.x + size.x - 1) / tileSize;
    y = pos.y / tileSize;

    if (y < 0 || y >= mapSize.y)
        return false;

    if (x0 < 0) x0 = 0;
    if (x1 >= mapSize.x) x1 = mapSize.x - 1;

    for (int x = x0; x <= x1; x++)
    {
        if (solidAt(x, y))
        {
            *posY = tileSize * (y + 1);
            return true;
        }
    }

    return false;
}

glm::ivec2 TileMap::getMapSize() const { return mapSize; }

glm::ivec2 TileMap::getMapPixelSize() const { return glm::ivec2(mapSize.x * tileSize, mapSize.y * tileSize); }

glm::ivec2 TileMap::tile2Pixel(const glm::ivec2& tilePos) const { return glm::ivec2(tilePos.x * tileSize, tilePos.y * tileSize); }

glm::ivec2 TileMap::pixel2Tile(const glm::ivec2& pixelPos) const { return glm::ivec2(pixelPos.x / tileSize, pixelPos.y / tileSize); }

bool TileMap::isTileSolid(const glm::ivec2& tilePos) const
{
	glm::ivec2 mapCoords = pixel2Tile(tilePos);
	if (mapCoords.x < 0 || mapCoords.x >= mapSize.x || mapCoords.y < 0 || mapCoords.y >= mapSize.y)
		return false;

	return solidAt(mapCoords.x, mapCoords.y);
}

void TileMap::setDynamicSolid(const glm::ivec2& tile, bool solid)
{
	if (tile.x < 0 || tile.x >= mapSize.x || tile.y < 0 || tile.y >= mapSize.y)
		return;
	dynamicSolid[tile.y * mapSize.x + tile.x] = solid ? 1 : 0;
}

bool TileMap::solidAt(int x, int y) const
{
	if (x < 0 || x >= mapSize.x || y < 0 || y >= mapSize.y)
		return false;
	const int id = map[y * mapSize.x + x];
	return (id != 0 && id != DOOR_OPEN_TILE) || dynamicSolid[y * mapSize.x + x] != 0;
}

void TileMap::openDoor()
{
	if (!hasDoor() || doorOpen) return;
	map[doorTile.y * mapSize.x + doorTile.x] = DOOR_OPEN_TILE;
	doorOpen = true;
	uploadVertices();   // redraws the tile with the open-door art
}

bool TileMap::touchesOpenDoor(const glm::ivec2& boxPos, const glm::ivec2& boxSize) const
{
	if (!doorOpen) return false;

    const glm::ivec2 doorCenter = tile2Pixel(doorTile) + glm::ivec2(tileSize / 2);
	const glm::ivec2 playerCenter = boxPos + boxSize / 2;
	const int centerMargin = tileSize / 4;
	return playerCenter.x >= doorCenter.x - centerMargin && playerCenter.x <= doorCenter.x + centerMargin &&
		playerCenter.y >= doorCenter.y - centerMargin && playerCenter.y <= doorCenter.y + centerMargin;
}