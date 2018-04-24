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
	bool pickUpGold();
	bool isVictorious();
	bool isDead();
	bool shootArrow();
	void setHasArrow(const bool value);

	Rotation getRotation();

	void setOnStartPosition();

	int getX();
	int getY();
private:
	bool killedMonster();
	Game* selfGame_;
	int x, y;
	//bool alive;
	bool hasGold;
	bool hasArrow;
	Rotation rotation;
};

#endif // !PLAYER_H
