#include "Player.h"
#include "Game.h"

Player::Player(Game* game_) {
	selfGame_ = game_;
	x = 0;
	y = selfGame_->getMap()->getHeight() - 1;
	alive = true;
	rotation = DOWN;
}

Player::~Player() {}

void Player::turn(bool clockwise) {
	switch (rotation) {
	case UP:
		rotation = (clockwise) ? RIGHT : LEFT;
		break;
	case RIGHT:
		rotation = (clockwise) ? DOWN : UP;
		break;
	case DOWN:
		rotation = (clockwise) ? LEFT : RIGHT;
		break;
	case LEFT:
		rotation = (clockwise) ? UP : DOWN;
		break;
	}
	SDL_Delay(40);
}

bool Player::move() {
	int newX = x;
	int newY = y;
	
	// determine direction
	switch (rotation) {
	case UP:
		newY--;
		break;
	case RIGHT:
		newX++;
		break;
	case DOWN:
		newY++;
		break;
	case LEFT:
		newX--;
		break;
	}

	SDL_Delay(40);

	if (selfGame_->getMap()->isInBounds(newX,newY)) {
		x = newX;
		y = newY;
		visitField(x, y);
		return true;
	}

	return false;
}

void Player::visitField(const int x, const int y) {
	selfGame_->getMap()->visitField(x, y);
}

Rotation Player::getRotation() {
	return rotation;
}

void Player::setOnStartPosition() {
	x = 0;
	y = selfGame_->getMap()->getHeight() - 1;
	rotation = DOWN;
}

int Player::getX() {
	return x;
}

int Player::getY() {
	return y;
}