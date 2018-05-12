#ifndef MAP_H
#define MAP_H

#include <cstdlib>
#include <ctime>
#include <SDL.h>

class Game;

struct Field {
	/* parameter that keeps track
	if visited by player */
	bool visited = false;
	bool breeze;
	bool pit;
	bool stench;
	bool monster;
	bool gold;
	bool sparkle;
};

class Map {
public:
	Map(Game* game_);
	~Map();

	void createNewMap();

	bool isInBounds(const int x, const int y);
	
	Field** getField();
	const int getWidth();
	const int getHeight();
	void visitField(const int x, const int y);
	void hideAllFields();
	void stealGold(const int x, const int y);
	bool checkIfMonster(const int x, const int y);
	void killMonster(const int x, const int y);
private:
	void allocateMap();
	int getNumberOfAdjacentFields(const int x, const int y);
	int getNumberOfAdjacentPits(const int x, const int y);
	bool isCorrectPitPosition(const int x, const int y);
	// generate random fields
	void fillMap();

	Game* selfGame_;
	Field** field_;
	const int minimumSize = 3;
	const int maximumSize = 12;
	int width, height;
};

#endif // !MAP_H