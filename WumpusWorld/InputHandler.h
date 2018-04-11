#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Game.h"

class InputHandler {
public:
	InputHandler();
	~InputHandler();
	// handle all key events
	Event handleKeyStates();
	// handles quitting with X on window
	Event handleQuit();
private:
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
};

#endif