#ifndef MONKEY_INCLUDE
#define MONKEY_INCLUDE

#include "Enemy.h"

class Monkey : public Enemy {
public:
	Monkey();
	~Monkey();

	void init() override;
	void updateMovement() override;
private:
};

#endif // MONKEY_INCLUDE