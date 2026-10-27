#include "Stats.h"

Stats::Stats() : heart(false), lives(3), bombAmount(1), bombRange(1) {}

bool Stats::hasHeart() { return heart; }
int Stats::getLives() { return lives; }
int Stats::getBombAmount() { return  bombAmount; }
int Stats::getBombRange() { return bombRange; }


void Stats::setHeart(bool heart) { this->heart = heart; }

void Stats::decreaseLives() { --lives; }
void Stats::resetLives() { lives = 3; }

void Stats::increaseBombAmount() { ++bombAmount; }
void Stats::resetBombAmount() { bombAmount = 1; }

void Stats::increaseBombRange() { ++bombRange; }
void Stats::resetBombRange() { bombRange = 1; }


void Stats::resetAll() {
	setHeart(false);
	resetLives();
	resetBombAmount();
	resetBombRange();
}