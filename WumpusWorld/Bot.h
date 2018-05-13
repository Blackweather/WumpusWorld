#ifndef BOT
#define BOT

#include "Game.h"

#define IMPOSSIBLE -1

struct infoAboutPosition {
	// not visited
	bool unknown = true;
	// only safe when 100% sure or visited
	bool safe = false;
	// probabilty of field being sth
	short probablyPit = 0;
	short probablyMonster = 0;
	short probablyGold = 0;
	bool isMonster = false;
	bool isGold = false;
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