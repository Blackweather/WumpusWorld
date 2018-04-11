#ifndef GAME_H
#define GAME_H

#include "SDLHandler.h"
#include "Map.h"
#include "Player.h"

enum Event {
	QUIT,
	NO_EVENT,
	MOVE,
	TURN_LEFT,
	TURN_RIGHT,
	SHOOT_ARROW,
	NEW_GAME,
	START_BOT,
	NEW_MAP
};

struct Coord {
	int x;
	int y;
};

class Game {
public:
	Game();
	~Game();
	void freespace();

	bool init();
	void initTime();

	void gameLoop();
	void update();
	void handleEvents();

	void createMap();
	Map* getMap();

	void spawnPlayer();
	void killPlayer();
private:
	void drawMap();
	void drawMenu();
	void drawPlayer();
	Coord mapToWindowCoords(const int x, const int y);

	SDLHandler* sdlHandler;

	Map* gameMap_;

	Player* player_;

	// flag used for quitting the game
	bool quit;

	// flag used for progress through current map
	bool victory = false;

	// t1 - starting time, t2 - current time
	int t1, t2;

	// delta=t2-t1
	// worldTime - seconds since program started
	double delta, worldTime;
};

#endif // !GAME_H