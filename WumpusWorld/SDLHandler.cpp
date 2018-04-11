#include "SDLHandler.h"

SDLHandler::SDLHandler() {}
SDLHandler::~SDLHandler() {}

#pragma region INITIALIZERS

bool SDLHandler::initRenderer() {

	if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
		0, &window, &renderer) == -1) {
		SDL_Quit();
		cerr << "SDL_CreateWindowAndRenderer error: " << SDL_GetError() << endl;
		return false;
	}

	// add linear filtering
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	// associate renderer size with window size
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);

	SDL_SetWindowTitle(window, "Wumpus World");

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT,
		32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	if (screen == NULL) {
		SDL_Quit();
		cerr << "SDL_CreateRGBSurface error: " << SDL_GetError() << endl;
		return false;
	}

	screenTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

	if (screenTexture == NULL) {
		SDL_Quit();
		cerr << "SDL_CreateTexture error: " << SDL_GetError() << endl;
		return false;
	}

	//disable mouse cursor in window
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

bool SDLHandler::init() {
	// main SDL init
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cerr << "SDL_Init error: " << SDL_GetError() << endl;
		return false;
	}

	if (!initRenderer()) {
		cerr << "SDLHandler::initRenderer failed\n";
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		cerr << "IMG_Init error: " << IMG_GetError() << endl;
		return false;
	}

	return true;
}

bool SDLHandler::loadTextures() {

#pragma region CHARSETS

	if (!loadSurface(&(charsets.black), "./img/cs8x8_black.png")) {
		cerr << "Cannot load cs8x8_black.png\n";
		return false;
	}

	if (!loadSurface(&(charsets.white), "./img/cs8x8_white.png")) {
		cerr << "Cannot load cs8x8_white.png\n";
		return false;
	}

#pragma endregion

#pragma region BACKGROUNDS

	if (!loadSurface(&background, "./img/background.png")) {
		cerr << "Cannot load background.png\n";
		return false;
	}

	if (!loadSurface(&fieldBackground, "./img/field_background.png")) {
		cerr << "Cannot load field_background.png\n";
		return false;
	}

#pragma endregion

#pragma region FIELD PROPERTIES

	if (!loadSurface(&breezeSprite, "./img/breeze.png")) {
		cerr << "Cannot load breeze.png\n";
		return false;
	}

	if (!loadSurface(&pitSprite, "./img/pit.png")) {
		cerr << "Cannot load pit.png\n";
		return false;
	}

	if (!loadSurface(&stenchSprite, "./img/stench.png")) {
		cerr << "Cannot load stench.png\n";
		return false;
	}

	if (!loadSurface(&monsterSprite, "./img/monster.png")) {
		cerr << "Cannot load monster.png\n";
		return false;
	}

	if (!loadSurface(&goldSprite, "./img/gold.png")) {
		cerr << "Cannot load gold.png\n";
		return false;
	}

	if (!loadSurface(&sparkleSprite, "./img/sparkle.png")) {
		cerr << "Cannot load sparkle.png\n";
		return false;
	}

#pragma endregion

#pragma region PLAYER SPRITES

	if (!loadSurface(&playerUp, "./img/player_top.png")) {
		cerr << "Cannot load player_top.png\n";
		return false;
	}

	if (!loadSurface(&playerDown, "./img/player_down.png")) {
		cerr << "Cannot load player_down.png\n";
		return false;
	}

	if (!loadSurface(&playerLeft, "./img/player_left.png")) {
		cerr << "Cannot load player_left.png\n";
		return false;
	}

	if (!loadSurface(&playerRight, "./img/player_right.png")) {
		cerr << "Cannot load player_right.png\n";
		return false;
	}

#pragma endregion

	return true;
}

void SDLHandler::mapColors() {
	colors[GRAY] = SDL_MapRGB(screen->format, 0xA3, 0xC2, 0xC2);
	colors[BLACK_C] = 0x000000;
}

#pragma endregion

#pragma region HELPERS

void SDLHandler::render() {
	// update everything on texture
	SDL_UpdateTexture(screenTexture, NULL, screen->pixels, screen->pitch);

	// connect the new texture with renderer
	SDL_RenderCopy(renderer, screenTexture, NULL, NULL);

	// renders
	SDL_RenderPresent(renderer);
}

bool SDLHandler::loadSurface(SDL_Surface** surface, char* path) {
	*surface = IMG_Load(path);
	if (*surface == NULL) {
		cerr << "IMG_Load error: " << IMG_GetError() << endl;
		return false;
	}

	return true;
}

void SDLHandler::drawSurface(SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	dest.w = (sprite->w + x > screen->w) ? (screen->w - x) : sprite->w;
	dest.h = (sprite->h + y > screen->h) ? (screen->h - y) : screen->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
}

void SDLHandler::drawString(int x, const int y, const char* text,
	TextColor textColor) {
	SDL_Surface* charset;

	// choose the correct charset
	if (textColor == WHITE) {
		charset = charsets.white;
	}
	else if (textColor == BLACK) {
		charset = charsets.black;
	}
	else {
		cerr << "Wrong text color\n";
		return;
	}

	// drawing
	SDL_Rect src, dest;
	int px, py, c;
	src.w = 8;
	src.h = 8;
	dest.w = 8;
	dest.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		src.x = px;
		src.y = py;
		dest.x = x;
		dest.y = y;
		SDL_BlitSurface(charset, &src, screen, &dest);
		x += 8;
		text++;
	}
}

// not working somehow
void SDLHandler::setBlackTransparent() {
	SDL_SetColorKey(screen, SDL_TRUE, colors[BLACK_C]);
}

void SDLHandler::drawField(Field field, const int x, const int y) {
	// output 64x64 field
	SDL_Surface* outField = SDL_CreateRGBSurface(0, 64, 64, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	SDL_BlitSurface(fieldBackground, &fieldBackground->clip_rect,
		outField, &outField->clip_rect);

	SDL_Rect dst;

	// only show visited fields
	if (field.visited) {

		int renderedObjectNumber = 0;
		if (field.breeze) {
			// render breeze
			dst = getPositionOnField(renderedObjectNumber);
			SDL_BlitSurface(breezeSprite, &breezeSprite->clip_rect, outField, &dst);
			renderedObjectNumber++;
		}
		if (field.gold) {
			// render gold
			dst = getPositionOnField(renderedObjectNumber);
			SDL_BlitSurface(goldSprite, &goldSprite->clip_rect, outField, &dst);
			renderedObjectNumber++;
		}
		if (field.monster) {
			// render monster
			dst = getPositionOnField(renderedObjectNumber);
			SDL_BlitSurface(monsterSprite, &monsterSprite->clip_rect, outField, &dst);
			renderedObjectNumber++;
		}
		if (field.pit) {
			// render pit
			dst = getPositionOnField(renderedObjectNumber);
			SDL_BlitSurface(pitSprite, &pitSprite->clip_rect, outField, &dst);
			renderedObjectNumber++;
		}
		if (field.sparkle) {
			// render sparkle
			dst = getPositionOnField(renderedObjectNumber);
			SDL_BlitSurface(sparkleSprite, &sparkleSprite->clip_rect, outField, &dst);
			renderedObjectNumber++;
		}
		if (field.stench) {
			// render stench
			dst = getPositionOnField(renderedObjectNumber);
			SDL_BlitSurface(stenchSprite, &stenchSprite->clip_rect, outField, &dst);
			renderedObjectNumber++;
		}
	}

	// render the field
	drawSurface(outField, x, y);

	// free memory
	SDL_FreeSurface(outField);
}

SDL_Rect SDLHandler::getPositionOnField(const int ObjectNo) {
	SDL_Rect dst;
	dst.w = 21;
	dst.h = dst.w;
	switch (ObjectNo) {
	case 1:
		dst.x = 43;
		dst.y = 0;
		break;
	case 2:
		dst.x = 43;
		dst.y = 43;
		break;
	case 3:
		dst.x = 0;
		dst.y = 43;
		break;
	default:
		dst.x = 0;
		dst.y = 0;
		break;
	}
	return dst;
}

void SDLHandler::drawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
}

void SDLHandler::drawLine(SDL_Surface* surface, int x, int y, int l,
	int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		drawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	}
}

#pragma endregion

#pragma region GETTERS

SDL_Surface* SDLHandler::getBackground() { return background; }

SDL_Surface* SDLHandler::getPlayerUp() { return playerUp; }
SDL_Surface* SDLHandler::getPlayerDown() { return playerDown; }
SDL_Surface* SDLHandler::getPlayerLeft() { return playerLeft; }
SDL_Surface* SDLHandler::getPlayerRight() { return playerRight; }

#pragma endregion


void SDLHandler::clean() {
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(screenTexture);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

