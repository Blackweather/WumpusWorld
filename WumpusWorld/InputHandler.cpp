#include "InputHandler.h"

InputHandler::InputHandler() {}

InputHandler::~InputHandler() {}
//to jest sciety komentarz
Event InputHandler::handleKeyStates() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_UP) {
				return MOVE;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				return TURN_LEFT;
			}
			else if (event.key.keysym.sym == SDLK_RIGHT) {
				return TURN_RIGHT;
			}
			else if (event.key.keysym.sym == SDLK_SPACE) {
				return SHOOT_ARROW;
			}
			else if (event.key.keysym.sym == SDLK_n) {
				return NEW_GAME;
			}
			else if (event.key.keysym.sym == SDLK_1) {
				return START_BOT;
			}
			else if (event.key.keysym.sym == SDLK_p) {
				return NEW_MAP;
			}
			else if (event.key.keysym.sym == SDLK_x) {
				return PICKUP_GOLD;
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE) {
				return QUIT;
			}
		default:
			return NO_EVENT;
		}
	}
	return NO_EVENT;
}

Event InputHandler::handleQuit() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return QUIT;
			break;
		default:
			return NO_EVENT;
		}
	}
	return NO_EVENT;
}