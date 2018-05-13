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
	NEW_MAP,
	PICKUP_GOLD
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

	// no usage for now, maybe later
	// just for fun
	void initTime();

	void gameLoop();
	void update();
	void handleEvents(Event whatToDo);
	void handleEventsFromKeyboard();

	void createMap();
	Map* getMap();
	Player* getPlayer();

	void spawnPlayer();
	void killPlayer();

	void newGame();
	void newMap();
	// change score by a value when interacting with game
	void changeScore(const int value);

	bool getIsMonsterDead();
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

	// used for checking progress
	int score;

	// t1 - starting time, t2 - current time
	int t1, t2;

	// delta=t2-t1
	// worldTime - seconds since program started
	double delta, worldTime;

	bool isMonsterDead;
};

#endif // !GAME_H