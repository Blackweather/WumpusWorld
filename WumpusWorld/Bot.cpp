#include "Bot.h"

Bot::Bot(Game* _game) : game(_game) {
	*map = new infoAboutPosition[game->getMap()->getHeight()];
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
				map[i][j].gold = true;
				goldFound = true;
				map[i][j].safe = true;
			}
			if (map[i][j].probablyMonster >= 2) {
				map[i][j].monster = true;
				monsterFound = true;
			}
			if (map[i][j].probablyMonster == -1 && map[i][j].probablyPit == -1)
				map[i][j].safe = true;
		}
	}
}

void Bot::checkMapForProbabilityDelete() {
	for (int i = 0; i < game->getMap()->getHeight(); i++) {
		for (int j = 0; j < game->getMap()->getWidth(); j++) {
			if (goldFound)
				map[i][j].probablyGold = -1;
			if (monsterFound)
				map[i][j].probablyMonster = -1;
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
		map[y][x].probablyPit = -1;
}

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
		map[y][x].probablyPit = -1;
}

bool Bot::checkForStenchOnField(const int x, const int y) {
	if (game->getMap()->isInBounds(x, y))
		if (game->getMap()->getField()[y][x].visited)
			if (!game->getMap()->getField()[y][x].stench)
				return true;
}

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

}

