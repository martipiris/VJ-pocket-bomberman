#ifndef MONKEY_INCLUDE
#define MONKEY_INCLUDE

#include "Enemy.h"

class Monkey : public Enemy {
public:
	Monkey();
	~Monkey();

	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) override;
	void updateMovement() override;
};

#endif // MONKEY_INCLUDE