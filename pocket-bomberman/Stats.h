#ifndef _STATS_INCLUDE
#define _STATS_INCLUDE

class Stats
{
public:
	static Stats& getInstance() {
		static Stats instance;
		return instance;
	}
	
	bool hasHeart();
	int getLives();
	int getBombAmount();
	int getBombRange();

	void setHeart(bool hasHeart);

	void decreaseLives();
	void resetLives();

	void increaseBombAmount();
	void resetBombAmount();

	void increaseBombRange();
	void resetBombRange();

	int getSpeedLevel();
	void increaseSpeed();
	void resetSpeed();

	void resetAll();

private:
	bool heart;

	int lives;
	int bombAmount;
	int bombRange;
	int speedLevel;
	Stats();
};

#endif //_STATS_INCLUDE
