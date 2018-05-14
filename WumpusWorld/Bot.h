#ifndef BOT
#define BOT

#define IMPOSSIBLE -1

#include "Player.h"
#include <vector>
#include <queue>

using namespace std;

class Game;

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

struct Coords {
	int x;
	int y;
	Coords(int _x, int _y) : x(_x), y(_y) {}
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

	void moveInDirection(Rotation direction);
	void rotateToDirection(Rotation direction);

	void pickGoldAndRun();
	void findPath(Coords coords);
	void findSafePassageToExit();

	void killMonster();
	bool checkIfMonsterInFrontOfUs();

	bool isSafeFieldAvailable();
	Coords findNearestUnvisitedSafeField();
	Coords findNearestLeastDangerousField();
};

#endif