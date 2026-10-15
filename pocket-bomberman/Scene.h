#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "libs/ShaderProgram.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();

	void init();
	void update(int deltaTime);
	void render();
	void deleteScene();

private:
	virtual void renderObjects() = 0;
	virtual void initObjects() = 0;
	virtual void updateObjects(int deltaTime) = 0;
	virtual void deleteObjects() = 0;

protected:
	ShaderProgram texProgram, simpleProgram;
	float currentTime;
	glm::mat4 projection;

	void initShaders();
};


#endif // _SCENE_INCLUDE

