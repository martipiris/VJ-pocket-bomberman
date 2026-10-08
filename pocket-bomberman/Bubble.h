#ifndef _BUBBLE_INCLUDE
#define _BUBBLE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Game.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Bubble
{

public:
	Bubble();
	~Bubble();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	TileMap *map;
	Player *player;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;

};


#endif // _BUBBLE_INCLUDE

