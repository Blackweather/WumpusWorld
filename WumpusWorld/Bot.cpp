#include "Bot.h"
#include "Game.h"

Bot::Bot(Game* _game) {
	game = _game;
	map = new infoAboutPosition*[game->getMap()->getHeight()];
	for (int i = 0; i < game->getMap()->getHeight(); i++) {
		map[i] = new infoAboutPosition[game->getMap()->getWidth()];
	}
}

void Bot::checkWhatsInField() {
	Field field = game->getMap()->getCurrentPlayersField();
	int x = game->getPlayer()->getX();
	int y = game->getPlayer()->getY();
	if (field.breeze)
		increaseProbabilityForPit(x, y);
	if (field.stench)
		increaseProbabilityForMonster(x, y);
	if (field.sparkle)
		increaseProbabilityForGold(x, y);
	if (!field.breeze && !field.stench)
		markAsSafe(x, y);
	checkMapForSurePositions();
	checkMapForProbabilityDelete();
}

void Bot::increaseProbabilityForPit(const int x, const int y) {
	if (game->getMap()->isInBounds(x + 1, y))
		map[y][x + 1].probablyPit++;
	if (game->getMap()->isInBounds(x - 1, y))
		map[y][x - 1].probablyPit++;
	if (game->getMap()->isInBounds(x, y + 1))
		map[y + 1][x].probablyPit++;
	if (game->getMap()->isInBounds(x, y - 1))
		map[y - 1][x].probablyPit++;
}

void Bot::increaseProbabilityForMonster(const int x, const int y) {
	if (game->getMap()->isInBounds(x + 1, y))
		map[y][x + 1].probablyMonster++;
	if (game->getMap()->isInBounds(x - 1, y))
		map[y][x - 1].probablyMonster++;
	if (game->getMap()->isInBounds(x, y + 1))
		map[y + 1][x].probablyMonster++;
	if (game->getMap()->isInBounds(x, y - 1))
		map[y - 1][x].probablyMonster++;
}

void Bot::increaseProbabilityForGold(const int x, const int y) {
	if (game->getMap()->isInBounds(x + 1, y))
		map[y][x + 1].probablyGold++;
	if (game->getMap()->isInBounds(x - 1, y))
		map[y][x - 1].probablyGold++;
	if (game->getMap()->isInBounds(x, y + 1))
		map[y + 1][x].probablyGold++;
	if (game->getMap()->isInBounds(x, y - 1))
		map[y - 1][x].probablyGold++;
}

short Bot::checkHowManyFieldsAreProbable(const int x, const int y) {
	int i = 0;
	if (checkIfFieldIsProbable(x - 1, y)) i++;
	if (checkIfFieldIsProbable(x + 1, y)) i++;
	if (checkIfFieldIsProbable(x, y - 1)) i++;
	if (checkIfFieldIsProbable(x, y + 1)) i++;
	return i;
}

bool Bot::checkIfFieldIsProbable(const int x, const int y) {
	return (game->getMap()->isInBounds(x, y) && checkIfAnyFieldAroundDeletesProbability(x, y));
}

// commented out
bool Bot::checkIfAnyFieldAroundDeletesProbability(const int x, const int y) {
	if (map[y][x].safe) return true;
	if (game->getMap()->isInBounds(x + 1, y))
		map[y][x + 1].probablyGold++;
	if (game->getMap()->isInBounds(x - 1, y))
		map[y][x - 1].probablyGold++;
	if (game->getMap()->isInBounds(x, y + 1))
		map[y + 1][x].probablyGold++;
	if (game->getMap()->isInBounds(x, y - 1))
		map[y - 1][x].probablyGold++;
}

void Bot::checkMapForSurePositions() {
	for (int i = 0; i < game->getMap()->getHeight(); i++) {
		for (int j = 0; j < game->getMap()->getWidth(); j++) {
			if (map[i][j].probablyGold >= 2) {
				map[i][j].isGold = true;
				goldFound = true;
				map[i][j].safe = true;
			}
			if (map[i][j].probablyMonster >= 2) {
				map[i][j].isMonster = true;
				monsterFound = true;
			}
			if (map[i][j].probablyMonster == IMPOSSIBLE && map[i][j].probablyPit == IMPOSSIBLE)
				map[i][j].safe = true;
		}
	}
}

void Bot::checkMapForProbabilityDelete() {
	for (int i = 0; i < game->getMap()->getHeight(); i++) {
		for (int j = 0; j < game->getMap()->getWidth(); j++) {
			if (goldFound)
				map[i][j].probablyGold = IMPOSSIBLE;
			if (monsterFound)
				map[i][j].probablyMonster = IMPOSSIBLE;
		}
	}
}

void Bot::checkMapForSafePositions() {
	for (int i = 0; i < game->getMap()->getHeight(); i++) {
		for (int j = 0; j < game->getMap()->getWidth(); j++) {
			if (game->getMap()->getField()[i][j].visited) {
				map[i][j].unknown = false;
				if (game->getMap()->getField()[i][j].breeze)
					checkForBreezeAroundField(i, j);
			}
		}
	}
}


void Bot::checkForBreezeAroundField(const int x, const int y) {
	if (checkForBreezeOnField(x + 1, y) ||
		checkForBreezeOnField(x - 1, y) ||
		checkForBreezeOnField(x, y - 1) ||
		checkForBreezeOnField(x, y + 1))
		map[y][x].probablyPit = IMPOSSIBLE;
}

// check if field visited and has no breeze
bool Bot::checkForBreezeOnField(const int x, const int y) {
	if (game->getMap()->isInBounds(x, y))
		if (game->getMap()->getField()[y][x].visited)
			if (!game->getMap()->getField()[y][x].breeze)
				return true;
}


void Bot::checkForStenchAroundField(const int x, const int y) {
	if (checkForStenchOnField(x + 1, y) ||
		checkForStenchOnField(x - 1, y) ||
		checkForStenchOnField(x, y - 1) ||
		checkForStenchOnField(x, y + 1))
		map[y][x].probablyMonster = IMPOSSIBLE;
}

// check if field visited and has no stench
bool Bot::checkForStenchOnField(const int x, const int y) {
	if (game->getMap()->isInBounds(x, y))
		if (game->getMap()->getField()[y][x].visited)
			if (!game->getMap()->getField()[y][x].stench)
				return true;
}

// mark all surrounding fields as safe
void Bot::markAsSafe(const int x, const int y) {
	if (game->getMap()->isInBounds(x + 1, y))
		map[y][x + 1].safe = true;
	if (game->getMap()->isInBounds(x - 1, y))
		map[y][x - 1].safe = true;
	if (game->getMap()->isInBounds(x, y + 1))
		map[y + 1][x].safe = true;
	if (game->getMap()->isInBounds(x, y - 1))
		map[y - 1][x].safe = true;
}

void Bot::decideBestMove() {
	checkWhatsInField();
	if (isSafeFieldAvailable())
		findPath(findNearestUnvisitedSafeField());
	else
		findPath(findNearestLeastDangerousField());
	if (game->getMap()->getCurrentPlayersField().gold)
		pickGoldAndRun();
	if (monsterFound)
		killMonster();
}

void Bot::moveInDirection(Rotation direction) {
	rotateToDirection(direction);
	game->handleEvents(MOVE);
}

void Bot::rotateToDirection(Rotation direction) {
	int playerDir = game->getPlayer()->getRotation();
	if (playerDir == direction - 1 || playerDir == direction + 3)
		game->handleEvents(TURN_RIGHT);
	if (playerDir == direction + 1 || playerDir == direction - 3)
		game->handleEvents(TURN_LEFT);
	if (playerDir == direction + 2 || playerDir == direction - 2) {
		game->handleEvents(TURN_LEFT);
		game->handleEvents(TURN_LEFT);
	}
}

void Bot::pickGoldAndRun() {
	game->handleEvents(PICKUP_GOLD);
	findSafePassageToExit();
}

void Bot::findPath(Coords coords) {
	//TODO: Pathfinding alghorithm,
	//that will walk player through,
	//moveInDirection method
}

void Bot::findSafePassageToExit() {
	findPath(Coords(0, game->getMap()->getHeight() - 1));
}

void Bot::killMonster() {
	if (checkIfMonsterInFrontOfUs())
		game->handleEvents(SHOOT_ARROW);
}

bool Bot::checkIfMonsterInFrontOfUs() {
	switch (game->getPlayer()->getRotation()) {
	case UP:
		for (int i = 0; i < game->getPlayer()->getY(); i++) {
			if (map[i][game->getPlayer()->getX()].isMonster)
				return true;
		}
		break;
	case RIGHT:
		for (int i = game->getPlayer()->getX(); i < game->getMap()->getWidth(); i++) {
			if (map[game->getPlayer()->getY()][i].isMonster)
				return true;
		}
		break;
	case DOWN:
		for (int i = game->getPlayer()->getY(); i < game->getMap()->getHeight(); i++) {
			if (map[i][game->getPlayer()->getX()].isMonster)
				return true;
		}
		break;
	case LEFT:
		for (int i = 0; i < game->getPlayer()->getX(); i++) {
			if (map[game->getPlayer()->getY()][i].isMonster)
				return true;
		}
		break;
	}
}

bool Bot::isSafeFieldAvailable() {
	//TODO: Check if there is possibility to get to some safe field,
	//without need to go through not 100% safe fields
	return false;
}

Coords Bot::findNearestUnvisitedSafeField() {
	//TODO: Self Explanatory
	return Coords(0, 0);
}

Coords Bot::findNearestLeastDangerousField() {
	//TODO: If there is no safe field available, 
	//find nearest field with lowest risk
	return Coords(0, 0);
}
