#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "libs/Sprite.h"
#include "libs/TileMap.h"

#define SIZE_PLAYER glm::ivec2(64, 64)

class Player
{
public:
	Player();
	~Player();

public:
	void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render(const glm::mat4 &view);
	
	void setTileMap(TileMap *tileMap);
	void setPosition(const glm::vec2 &pos);
	
	glm::vec2 getPosition() const;
private:
	// Física
	glm::vec2 posPlayerF;    
	glm::ivec2 tileMapDispl; 
	glm::ivec2 posPlayer;    
	float velY;              
	bool  onGround;

	// parámetros
	const float gravity = 0.6f;       
	const float jumpSpeed =  30.0f;  
	const float maxFallSpeed = 18.0f;
	const float horizSpeed = 6.0f;

	// Render/anim
	Texture spritesheet;
	Sprite *sprite;
	TileMap *map;

};

#endif // _PLAYER_INCLUDE


