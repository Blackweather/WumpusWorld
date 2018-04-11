#include "Game.h"

int main(int argc, char* argv[]) {
	Game* game = new Game();
	if (!game->init()) {
		cerr << "Cannot initialize!\n";
		return 1;
	}

	game->gameLoop();

	game->freespace();

	delete game;

	return 0;
}