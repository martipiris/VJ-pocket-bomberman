#ifndef _PACMAN_INCLUDE
#define _PACMAN_INCLUDE

#include "Enemy.h"

class Pacman : public Enemy {
public:
	Pacman();
	~Pacman();

	void init() override;
	
	void updateMovement() override;
private:
	
};

#endif // _PACMAN_INCLUDE