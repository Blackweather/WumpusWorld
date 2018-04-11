#ifndef PLAYER_H
#define PLAYER_H

class Game;

enum Rotation {
	UP,
	RIGHT,
	DOWN,
	LEFT
};

class Player {
public:
	Player(Game* game_);
	~Player();
	bool move();
	void turn(bool clockwise);
	void visitField(const int x, const int y);
	Rotation getRotation();
	int getX();
	int getY();
private:
	Game* selfGame_;
	int x, y;
	bool alive;
	Rotation rotation;
};

#endif // !PLAYER_H
