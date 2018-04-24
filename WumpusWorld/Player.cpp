#include "Player.h"
#include "Game.h"

Player::Player(Game* game_) {
	selfGame_ = game_;
	x = 0;
	y = selfGame_->getMap()->getHeight() - 1;
	//alive = true;
	hasGold = false;
	hasArrow = true;
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

	if (selfGame_->getMap()->isInBounds(newX, newY)) {
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

bool Player::pickUpGold() {
	if (selfGame_->getMap()->getField()[y][x].gold) {
		hasGold = true;
		return true;
	}
	return false;
}

bool Player::isVictorious() {
	if (hasGold && x == 0 && y == selfGame_->getMap()->getHeight() - 1) {
		hasGold = false;
		return true;
	}
	return false;
}

bool Player::isDead() {
	if (selfGame_->getMap()->getField()[y][x].monster ||
		selfGame_->getMap()->getField()[y][x].pit) {
		return true;
	}
	return false;
}

bool Player::shootArrow() {
	if (hasArrow) {
		// get rid of the arrow
		hasArrow = false;
		// check if arrow hit the monster
		if (killedMonster()) {
			return true;
		}
	}
	return false;
}

void Player::setHasArrow(const bool value) {
	hasArrow = value;
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

bool Player::killedMonster() {
	bool killed = false;

	switch (rotation) {
	case UP:
		// check if not standing on top field
		if (selfGame_->getMap()->isInBounds(x, y - 1)) {
			for (int i = 0; i < y; i++) {
				// check every field up from current position
				if (selfGame_->getMap()->checkIfMonster(x, i)) {
					killed = true;
					selfGame_->getMap()->killMonster(x, i);
					break;
				}
			}
			return killed;
		}
		return false;
		break;
	case RIGHT:
		// check if not standing on right field
		if (selfGame_->getMap()->isInBounds(x + 1, y)) {
			for (int i = selfGame_->getMap()->getWidth() - 1; i > x; i--) {
				// check every field right from current position
				if (selfGame_->getMap()->checkIfMonster(i, y)) {
					killed = true;
					selfGame_->getMap()->killMonster(i, y);
					break;
				}
			}
			return killed;
		}
		break;
	case DOWN:
		// check if not standing on bottom field
		if (selfGame_->getMap()->isInBounds(x, y + 1)) {
			for (int i = y; i < selfGame_->getMap()->getHeight(); i++) {
				// check every field bottom from current position
				if (selfGame_->getMap()->checkIfMonster(x, i)) {
					killed = true;
					selfGame_->getMap()->killMonster(x, i);
					break;
				}
			}
			return killed;
		}
		break;
	case LEFT:
		// check if not standing on left field
		if (selfGame_->getMap()->isInBounds(x - 1, y)) {
			for (int i = 0; i < x; i++) {
				// check every field left from current position
				if (selfGame_->getMap()->checkIfMonster(i, y)) {
					killed = true;
					selfGame_->getMap()->killMonster(i, y);
					break;
				}
			}
			return killed;
		}
		break;
	}

	return false;
}