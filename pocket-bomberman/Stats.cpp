#include "Stats.h"

namespace {
	const int MAX_BOMB_AMOUNT = 8;
	const int MAX_BOMB_RANGE = 8;
	const int MAX_SPEED_LEVEL = 5;
}

Stats::Stats() : heart(false), lives(3), bombAmount(1), bombRange(1), speedLevel(0) {}

bool Stats::hasHeart() { return heart; }
int Stats::getLives() { return lives; }
int Stats::getBombAmount() { return  bombAmount; }
int Stats::getBombRange() { return bombRange; }


void Stats::setHeart(bool heart) { this->heart = heart; }

void Stats::decreaseLives() { --lives; }
void Stats::resetLives() { lives = 3; }

void Stats::increaseBombAmount() { if (bombAmount < MAX_BOMB_AMOUNT) ++bombAmount; }
void Stats::resetBombAmount() { bombAmount = 1; }

void Stats::increaseBombRange() { if (bombRange < MAX_BOMB_RANGE) ++bombRange; }
void Stats::resetBombRange() { bombRange = 1; }

int Stats::getSpeedLevel() { return speedLevel; }
void Stats::increaseSpeed() { if (speedLevel < MAX_SPEED_LEVEL) ++speedLevel; }
void Stats::resetSpeed() { speedLevel = 0; }


void Stats::resetAll() {
	setHeart(false);
	resetLives();
	resetBombAmount();
	resetBombRange();
	resetSpeed();
}