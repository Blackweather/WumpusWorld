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
	// remaining:
	// isSafeFieldAvailable
	// findPath
	// findNearestLeastDangerousField

	// use findNearestUnvisitedSafeField for most of them
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
	//each step by moveInDirection

	// use BFS from findNearestUnvisitedField
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


	//WARNING: still shit
	/*int x = game->getPlayer()->getX();
	int y = game->getPlayer()->getY();
	Coords c = findNearestUnvisitedSafeField();
	if (c.x != x || c.y != y) {
		return true;
	}
	return false;*/
	return true;
}

Coords Bot::findNearestUnvisitedSafeField() {
	vector<Coords> allSafeFields;
	int x = game->getPlayer()->getX();
	int y = game->getPlayer()->getY();

	for (int i = 0; i < game->getMap()->getHeight(); i++) {
		for (int j = 0; j < game->getMap()->getWidth(); j++) {
			if ((i != y || j != x) && map[i][j].unknown && map[i][j].safe) {
				allSafeFields.push_back(Coords(j, i));
			}
		}
	}

	// find path to each safe unvisited and return with shortest path
	queue<Coords> cQueue;
	vector<int> pathLengths;
	for each (Coords final in allSafeFields)
	{
		// search start location
		cQueue.push(Coords(x, y));
		// starting path length
		int pathLen = 0;

		int** BFSMap;
		BFSMap = new int*[game->getMap()->getHeight()];
		for (int i = 0; i < game->getMap()->getHeight(); i++) {
			BFSMap[i] = new int[game->getMap()->getWidth()];
		}

		for (int i = 0; i < game->getMap()->getHeight(); i++) {
			for (int j = 0; j < game->getMap()->getWidth(); j++) {
				// set all as unvisited by algorithm
				BFSMap[i][j] = -1;
			}
		}
		while (true) {
			// main loop
			// pop location from queue
			Coords location = cQueue.front();
			cQueue.pop();
			// mark current location as visited
			BFSMap[location.y][location.x] = 1;

			// increase path length
			pathLen++;


			// loop through safe neighbours
			// if any is final - done
			// if no final, not visited by algorithm - add to queue 
			// add to queue when is visited and safe

			// up
			if (game->getMap()->isInBounds(location.x, location.y - 1)) {
				if (map[location.x][location.y - 1].safe) {
					if (map[location.x][location.y - 1].unknown) {
						break;
					}
					if (BFSMap[location.x][location.y - 1] == -1) {
						cQueue.push(Coords(location.x, location.y - 1));
					}
				}
			}

			// right
			if (game->getMap()->isInBounds(location.x + 1, location.y)) {
				if (map[location.x + 1][location.y].safe) {
					if (map[location.x + 1][location.y].unknown) {
						break;
					}
					if (BFSMap[location.x + 1][location.y] == -1) {
						cQueue.push(Coords(location.x + 1, location.y));
					}
				}
			}

			// down
			if (game->getMap()->isInBounds(location.x, location.y + 1)) {
				if (map[location.x][location.y + 1].safe) {
					if (map[location.x][location.y + 1].unknown) {
						break;
					}
					if (BFSMap[location.x][location.y + 1] == -1) {
						cQueue.push(Coords(location.x, location.y + 1));
					}
				}
			}

			// left
			if (game->getMap()->isInBounds(location.x - 1, location.y)) {
				if (map[location.x - 1][location.y].safe) {
					if (map[location.x - 1][location.y].unknown) {
						break;
					}
					if (BFSMap[location.x - 1][location.y] == -1) {
						cQueue.push(Coords(location.x - 1, location.y));
					}
				}
			}
		}
		// repeat with the first queued location

		pathLengths.push_back(pathLen);
	}

	//sort safeFields based on pathLengths
	struct Pair {
		Coords location;
		int len;
		Pair(Coords _c, int _len) : location(_c), len(_len) {}
		bool operator<(const Pair& rhs) const
		{
			return len < rhs.len;
		}
	};

	vector<Pair> sortVec;
	for (size_t i = 0; i < pathLengths.size(); i++) {
		sortVec.push_back(Pair(allSafeFields[i], pathLengths[i]));
	}
	sort(sortVec.begin(), sortVec.end());

	return sortVec[0].location;

}

Coords Bot::findNearestLeastDangerousField() {
	//TODO: If there is no safe field available, 
	//find nearest field with lowest risk

	// find all fields with lowest risk

	// determine which has the shortest path to it

	// return it
	return Coords(0, 0);
}
