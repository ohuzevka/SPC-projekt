#pragma once
#include <stdint.h>

enum Dir	// Direction of snake movement
{
	LEFT,
	RIGHT,
	UP,
	DOWN
};

struct Chain	// One chain of snake
{
	uint8_t posX;
	uint8_t posY;
};

class Snake
{
	uint8_t lenght;
	Dir dir;
	Chain chain[];

public:
	void Draw();

	void Move();
};

class Game
{
	bool running;
	unsigned int score;
	unsigned int speed;
	Snake snake;
};