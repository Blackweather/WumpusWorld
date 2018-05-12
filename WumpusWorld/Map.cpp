#include "Map.h"
#include "Game.h"

Map::Map(Game* game_) {
	selfGame_ = game_;

	createNewMap();
}
Map::~Map() {}

void Map::createNewMap() {
	srand(static_cast<unsigned int> (time(NULL)));

	// get random parameters for map
	width = (rand() % (maximumSize - minimumSize + 1)) + minimumSize;
	height = (rand() % (maximumSize - minimumSize + 1)) + minimumSize;

	// allocate the array for map fields
	allocateMap();

	// randomize the map fields
	fillMap();
}

bool Map::isInBounds(const int x, const int y) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return false;
	}
	return true;
}

Field** Map::getField() {
	return field_;
}

const int Map::getWidth() {
	return width;
}

const int Map::getHeight() {
	return height;
}

void Map::visitField(const int x, const int y) {
	field_[y][x].visited = true;
}

/* makes all fields except starting unvisited */
void Map::hideAllFields() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			field_[i][j].visited = false;
		}
	}

	visitField(0, height - 1);
}

void Map::stealGold(const int x, const int y) {
	field_[y][x].gold = false;

	// the sparkle disappears
	if (isInBounds(x - 1, y)) {
		field_[y][x - 1].sparkle = false;
	}
	if (isInBounds(x + 1, y)) {
		field_[y][x + 1].sparkle = false;
	}
	if (isInBounds(x, y - 1)) {
		field_[y - 1][x].sparkle = false;
	}
	if (isInBounds(x, y + 1)) {
		field_[y + 1][x].sparkle = false;
	}
}

bool Map::checkIfMonster(const int x, const int y) {
	return field_[y][x].monster;
}

void Map::killMonster(const int x, const int y) {
	// [*]
	field_[y][x].monster = false;

	// the stench disappears
	if (isInBounds(x - 1, y)) {
		field_[y][x - 1].stench = false;
	}
	if (isInBounds(x + 1, y)) {
		field_[y][x + 1].stench = false;
	}
	if (isInBounds(x, y - 1)) {
		field_[y - 1][x].stench = false;
	}
	if (isInBounds(x, y + 1)) {
		field_[y + 1][x].stench = false;
	}

}

void Map::allocateMap() {
	field_ = new Field*[height];
	for (int i = 0; i < height; i++) {
		field_[i] = new Field[width];
	}

	// bottom left corner - starting field
	field_[height - 1][0].visited = true;

	// initialize empty field
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			field_[i][j].breeze = false;
			field_[i][j].pit = false;
			field_[i][j].stench = false;
			field_[i][j].monster = false;
			field_[i][j].gold = false;
			field_[i][j].sparkle = false;
		}
	}
}

int Map::getNumberOfAdjacentFields(const int x, const int y) {
	// corners
	if (x == 0 || x == width - 1) {
		if (y == 0 || y == height - 1) {
			return 2;
		}
		else {
			// left and right wall
			return 3;
		}
	}

	if (y == 0 || y == height - 1) {
		if (x > 0 && x < width - 1) {
			// top and down wall
			return 3;
		}
	}

	// any other field
	return 4;
}

int Map::getNumberOfAdjacentPits(const int x, const int y) {
	int result = 0;

	// check all adjacent fields for pits

	// up
	if (isInBounds(x, y - 1)) {
		if (field_[y - 1][x].pit) {
			result++;
		}
	}

	// down
	if (isInBounds(x, y + 1)) {
		if (field_[y + 1][x].pit) {
			result++;
		}
	}

	// left
	if (isInBounds(x - 1, y)) {
		if (field_[y][x - 1].pit) {
			result++;
		}
	}

	// right
	if (isInBounds(x + 1, y)) {
		if (field_[y][x + 1].pit) {
			result++;
		}
	}

	return result;
}

bool Map::isCorrectPitPosition(const int x, const int y) {
	// check if not bottom left position
	if (x == 0 && y == height - 1) {
		return false;
	}

	// check if no monster or gold on same place
	if (field_[y][x].monster || field_[y][x].gold) {
		return false;
	}

	// check if gold on adjacent tiles

	// left
	if (isInBounds(x - 1, y)) {
		if (field_[y][x - 1].gold) {
			if (getNumberOfAdjacentPits(x - 1, y)
				== getNumberOfAdjacentFields(x - 1, y) - 1) {
				return false;
			}
		}
	}

	// right
	if (isInBounds(x + 1, y)) {
		if (field_[y][x + 1].gold) {
			if (getNumberOfAdjacentPits(x + 1, y)
				== getNumberOfAdjacentFields(x + 1, y) - 1) {
				return false;
			}
		}
	}

	// up
	if (isInBounds(x, y - 1)) {
		if (field_[y - 1][x].gold) {
			if (getNumberOfAdjacentPits(x, y - 1)
				== getNumberOfAdjacentFields(x, y - 1) - 1) {
				return false;
			}
		}
	}

	// down
	if (isInBounds(x, y + 1)) {
		if (field_[y + 1][x].gold) {
			if (getNumberOfAdjacentPits(x, y + 1)
				== getNumberOfAdjacentFields(x, y + 1) - 1) {
				return false;
			}
		}
	}

	return true;
}

void Map::fillMap() {
	/*****************************************************
	Fill the fields

	Rules:
	- only one monster
		- monster surrounded with stench
	- only one gold
		- gold surrounded with sparkle
	- pit cannot be on the same field as monster/gold
	- pit cannot be on starting field (bottom left)
	- pits cannot surround gold
		- pit cover 15.6% of field
			(based on google images research)
			- maybe make it input value?
		- pit surrounded with breeze
	******************************************************/

	// guard player starting position
	const int bottomY = height - 1;
	const int bottomX = 0;

	bool correctPosition = false;

	int randomX, randomY;

	/**********************
		monster
	***********************/
	while (!correctPosition) {
		randomX = rand() % width;
		randomY = rand() % height;

		if (randomX != bottomX || randomY != bottomY) {
			correctPosition = true;
		}
	}

	// insert into map
	field_[randomY][randomX].monster = true;

	// insert monster stench on 4 surrounding fields
	// left
	if (isInBounds(randomX - 1, randomY)) {
		field_[randomY][randomX - 1].stench = true;
	}
	// right
	if (isInBounds(randomX + 1, randomY)) {
		field_[randomY][randomX + 1].stench = true;
	}
	// up
	if (isInBounds(randomX, randomY - 1)) {
		field_[randomY - 1][randomX].stench = true;
	}
	// down
	if (isInBounds(randomX, randomY + 1)) {
		field_[randomY + 1][randomX].stench = true;
	}

	correctPosition = false;

	/*********************
		gold
	**********************/
	while (!correctPosition) {
		randomX = rand() % width;
		randomY = rand() % height;

		if (randomX != bottomX || randomY != bottomY) {
			correctPosition = true;
		}
	}

	// insert into map
	field_[randomY][randomX].gold = true;

	// insert gold sparkle on 4 surrounding fields
	// left
	if (isInBounds(randomX - 1, randomY)) {
		field_[randomY][randomX - 1].sparkle = true;
	}
	// right
	if (isInBounds(randomX + 1, randomY)) {
		field_[randomY][randomX + 1].sparkle = true;
	}
	// up
	if (isInBounds(randomX, randomY - 1)) {
		field_[randomY - 1][randomX].sparkle = true;
	}
	// down
	if (isInBounds(randomX, randomY + 1)) {
		field_[randomY + 1][randomX].sparkle = true;
	}

	/********************
		pits
	*********************/

	// thanks to google
	const double percentOfPits = 15.6;
	int numberOfPits = (int)((width*height)*(percentOfPits / 100));

	// generate pits
	for (int i = 0; i < numberOfPits; i++) {
		correctPosition = false;

		do {
			randomX = rand() % width;
			randomY = rand() % height;
		} while (!isCorrectPitPosition(randomX, randomY));

		// insert into map
		field_[randomY][randomX].pit = true;

		// insert pit breeze into 4 surrounding fields
		// left
		if (isInBounds(randomX - 1, randomY)) {
			field_[randomY][randomX - 1].breeze = true;
		}
		// right
		if (isInBounds(randomX + 1, randomY)) {
			field_[randomY][randomX + 1].breeze = true;
		}
		// up
		if (isInBounds(randomX, randomY - 1)) {
			field_[randomY - 1][randomX].breeze = true;
		}
		// down
		if (isInBounds(randomX, randomY + 1)) {
			field_[randomY + 1][randomX].breeze = true;
		}
	}
}

