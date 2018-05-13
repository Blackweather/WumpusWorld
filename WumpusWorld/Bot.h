#ifndef BOT
#define BOT

#include "Game.h"

struct infoAboutPosition {
	bool unknown = true;
	bool safe;
	short probablyPit = 0;
	bool monster;
	short probablyMonster = 0;
	bool gold;
	short probablyGold = 0;
};

class Bot {
private:
	Game* game;
	infoAboutPosition** map;
	bool goldFound;
	bool monsterFound;
public:
	Bot(Game*);
	void checkWhatsInField();
	void increaseProbabilityForPit(const int x, const int y);
	void increaseProbabilityForMonster(const int x, const int y);
	void increaseProbabilityForGold(const int x, const int y);
	short checkHowManyFieldsAreProbable(const int x, const int y);
	bool checkIfFieldIsProbable(const int x, const int y);
	bool checkIfAnyFieldAroundDeletesProbability(const int x, const int y);

	void checkMapForSurePositions();
	void checkMapForProbabilityDelete();
	void checkMapForSafePositions();
	void checkForBreezeAroundField(const int x, const int y);
	bool checkForBreezeOnField(const int x, const int y);
	void checkForStenchAroundField(const int x, const int y);
	bool checkForStenchOnField(const int x, const int y);


	void markAsSafe(const int x, const int y);
	void decideBestMove();
};

#endif