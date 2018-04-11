#include "Game.h"
#include "InputHandler.h"

Game::Game() {
}

Game::~Game() {}

bool Game::init() {
	cout << "Program started\n";
	sdlHandler = new SDLHandler;
	if (!sdlHandler->init()) {
		cerr << "Cannot initialize SDL\n";
		return false;
	}

	if (!sdlHandler->loadTextures()) {
		cerr << "Cannot load required textures\n";
		return false;
	}

	sdlHandler->mapColors();
	initTime();

	createMap();

	spawnPlayer();

	victory = false;
	return true;
}

void Game::initTime() {
	t1 = SDL_GetTicks();

	quit = false;

	worldTime = 0;
}

void Game::freespace() {

}

void Game::gameLoop() {
	while (!quit) {
		update();
	}
}

void Game::update() {
	t2 = SDL_GetTicks();

	if (!victory) {
		delta = (t2 - t1)*0.001;
		// clear time difference
		t1 = t2;

		//increase world time
		worldTime += delta;
	}

	// insert background
	sdlHandler->drawSurface(sdlHandler->getBackground(), 0, 0);

	drawMenu();

	// draw map
	drawMap();

	// draw player
	drawPlayer();

	sdlHandler->render();
	handleEvents();
}

void Game::handleEvents() {
	InputHandler* handler = new InputHandler();
	switch (handler->handleKeyStates()) {
	case MOVE:
		// move character forward
		player_->move();
		break;
	case TURN_LEFT:
		// change player direction 90 deg to left
		player_->turn(false);
		break;
	case TURN_RIGHT:
		// change player direction 90 deg to right
		player_->turn(true);
		break;
	case SHOOT_ARROW:
		// self explanatory
		break;
	case NEW_GAME:
		// start over same map
		initTime();
		break;
	case START_BOT:
		// launch bot
		break;
	case NEW_MAP:
		// generate new map
		initTime();
		break;
	case QUIT:
		quit = true;
		break;
	case NO_EVENT:
		break;
	}

	if (handler->handleQuit() == QUIT) quit = true;

	delete handler;
}

void Game::createMap() {
	gameMap_ = new Map(this);
}

Map* Game::getMap() {
	return gameMap_;
}

void Game::spawnPlayer() {
	player_ = new Player(this);
}

void Game::killPlayer() {
	delete player_;
}

void Game::drawMap() {
	Field** field = gameMap_->getField();
	const int width = gameMap_->getWidth();
	const int height = gameMap_->getHeight();
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// draw element
			Coord dest = mapToWindowCoords(j, i);
			sdlHandler->drawField(field[i][j], dest.x, dest.y);
		}
	}
}

void Game::drawMenu() {
	sdlHandler->setBlackTransparent();
	sdlHandler->drawString(110, 15, "MENU", WHITE);
	sdlHandler->drawString(15, 45, "Left/Right Arrow - Turn", WHITE);
	sdlHandler->drawString(15, 75, "Up Arrow - Move", WHITE);
	sdlHandler->drawString(15, 105, "Space - shoot", WHITE);
	sdlHandler->drawString(15, 135, "N - New Game", WHITE);
	sdlHandler->drawString(15, 165, "P - New Map", WHITE);
	sdlHandler->drawString(15, 195, "1 - Start Bot", WHITE);
	sdlHandler->drawString(15, 225, "ESC - Quit", WHITE);
}

void Game::drawPlayer() {
	Coord dest = mapToWindowCoords(player_->getX(), player_->getY());
	dest.x += 21;
	dest.y += 21;

	Rotation rot = player_->getRotation();
	switch (rot) {
	case UP:
		sdlHandler->drawSurface(sdlHandler->getPlayerUp(), dest.x, dest.y);
		break;
	case RIGHT:
		sdlHandler->drawSurface(sdlHandler->getPlayerRight(), dest.x, dest.y);
		break;
	case DOWN:
		sdlHandler->drawSurface(sdlHandler->getPlayerDown(), dest.x, dest.y);
		break;
	case LEFT:
		sdlHandler->drawSurface(sdlHandler->getPlayerLeft(), dest.x, dest.y);
		break;
	}
}

Coord Game::mapToWindowCoords(const int x, const int y) {
	Coord c;
	c.x = x * 64 + (SCREEN_HEIGHT / 3) + ((768 - (gameMap_->getWidth()) * 64) / 2);
	c.y = y * 64 + ((SCREEN_HEIGHT - (gameMap_->getHeight()) * 64) / 2);
	return c;
}