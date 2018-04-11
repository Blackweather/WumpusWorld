#ifndef SDL_HANDLER_H
#define SDL_HANDLER_H

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>

// to get Field struct
#include "Map.h"

using namespace std;

enum TextColor {
	WHITE,
	BLACK
};

enum Colors {
	GRAY,
	BLACK_C,
	NUMBER_OF_COLORS
};

class SDLHandler {
public:
	SDLHandler();
	~SDLHandler();

	// initializes renderer and window
	bool initRenderer();

	// all initializations
	bool init();

	// render everything on screen
	void render();

	// load charsets
	bool loadTextures();

	// initializes colors with RGB values
	void mapColors();

	// loads a texture to specified surface from specified path
	bool loadSurface(SDL_Surface** surface, char* path);

	// draw a surface sprite on a surface screen in point (x,y)
	void drawSurface(SDL_Surface* sprite, int x, int y);

	// writes text beginning with specified xy
	void drawString(const int x, const int y, const char* text,
		TextColor textColor);

	// makes black transparent
	void setBlackTransparent();

	// composes a field then proceeds to draw it
	void drawField(Field field, const int x, const int y);

	// two below probably not needed

	// draw a single pixel
	void drawPixel(SDL_Surface* surface, int x, int y, Uint32 color);

	// draw a vertical (when dx = 0, dy = 1)
	// or horizontal (dx = 1, dy = 0) line
	void drawLine(SDL_Surface* surface, int x, int y, int l,
		int dx, int dy, Uint32 color);

	SDL_Surface* getBackground();

	SDL_Surface* getPlayerUp();
	SDL_Surface* getPlayerDown();
	SDL_Surface* getPlayerLeft();
	SDL_Surface* getPlayerRight();

	// destroy everything
	void clean();

	// gets rect of objects position on field
	SDL_Rect getPositionOnField(const int objectNo);
private:
	
	SDL_Window* window;
	SDL_Renderer* renderer;

	// final target of rendering
	SDL_Surface* screen;

	SDL_Texture* screenTexture;

	struct {
		SDL_Surface* black;
		SDL_Surface* white;
	} charsets;

	SDL_Surface* background;
	SDL_Surface* fieldBackground;

	SDL_Surface* breezeSprite;
	SDL_Surface* pitSprite;
	SDL_Surface* stenchSprite;
	SDL_Surface* monsterSprite;
	SDL_Surface* goldSprite;
	SDL_Surface* sparkleSprite;

	SDL_Surface* playerUp;
	SDL_Surface* playerDown;
	SDL_Surface* playerLeft;
	SDL_Surface* playerRight;

	int colors[NUMBER_OF_COLORS];
};

#endif // !SDL_HANDLER_H